#include "DataFifo.h"

#define EMPTY false 
#define FILLED true 

DataFIFO::DataFIFO(size_t bufferSize, size_t maxBlocksCount) {
	_bufferSize = bufferSize;
	_maxBlocks = maxBlocksCount;
	_data = operator new[](bufferSize);
	bufferStateVector.push_back(Block::Block(bufferSize, _data, BlockState::FREE));
}

DataFIFO::~DataFIFO()
{
	operator delete[](_data);
}

void* DataFIFO::getFree(size_t size)
{
	std::lock_guard<std::mutex> guard(_lock);
	void* result = foundFreePlace(size);
	if (result != nullptr) {
		_queue.push_back(Block::Block(size, result, BlockState::READY_FOR_WRITING));
	}
	return result;
}

void DataFIFO::addReady(void* data) {
	std::lock_guard<std::mutex> guard(_lock);
	for (auto it = _queue.begin(); it != _queue.end(); it++) {
		if (it->_ptr == data) {
			it->_state = BlockState::READY_FOR_READING;
			break;
		}
	}
}

void* DataFIFO::getReady(size_t& size)
{
	std::lock_guard<std::mutex> guard(_lock);
	void* result = nullptr;
	size = 0;
	if (!_queue.empty()) {
		if (_queue.front()._state == BlockState::READY_FOR_READING) {
			result = _queue.front()._ptr;
			size = _queue.front()._size;
			_queue.pop_front();
		}
	}
	return result;
}

void DataFIFO::addFree(void* data)
{
	std::lock_guard<std::mutex> guard(_lock);
	
	for (size_t i = 0; i < bufferStateVector.size(); i++) {
		if (bufferStateVector[i]._ptr == data) {
			bufferStateVector[i]._state = BlockState::FREE;
			break;
		}
	}
	if (isBufferEmpty())
		return;

	auto it = bufferStateVector.begin();
	for (size_t i = 0; i < bufferStateVector.size(); i++) {
		if (i + 1 == bufferStateVector.size()) {
			if (bufferStateVector[i - 1]._state == BlockState::FREE) {
				if (bufferStateVector[i]._state == BlockState::FREE) {
					i--;
					bufferStateVector[i]._size += bufferStateVector[i + 1]._size;
					it = bufferStateVector.erase(it);
				}
			}
			break;
		}
		it++;
		if (bufferStateVector[i]._state == BlockState::FREE) {
			if (bufferStateVector[i + 1]._state == BlockState::FREE) {
				bufferStateVector[i]._size += bufferStateVector[i + 1]._size;
				it = bufferStateVector.erase(it);
			}
		}
	}
}

bool DataFIFO::isBufferEmpty()
{
	if (bufferStateVector.size() == 1 && bufferStateVector.front()._state == BlockState::FREE)
		return true;
	else
		return false;
}

bool DataFIFO::isQueueEmpty()
{
	return _queue.empty();
}

void* DataFIFO::foundFreePlace(size_t size)
{
	void* result = nullptr;
	size_t offset = 0;
	for (auto it = bufferStateVector.begin(); it != bufferStateVector.end(); it++) {
		if (it->_size >= size && it->_state == BlockState::FREE) {
			result = static_cast<char*>(_data) + offset;
			it = bufferStateVector.insert(it, Block::Block(size, result, BlockState::INUSE));
			it++;
			it->_ptr = static_cast<char*>(_data) + offset + size;
			it->_size -= size;
			if (it->_size == 0)
				bufferStateVector.erase(it);
			break;
		}
		offset += it->_size;
	}
	return result;
}

