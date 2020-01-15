#include "DataFifo.h"

#define EMPTY false 
#define FILLED true 

DataFIFO::DataFIFO(size_t bufferSize, size_t maxBlocksCount) {
	_bufferSize = bufferSize;
	_maxBlocks = maxBlocksCount;
	
	_dataPrtOffset = 0;

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
	/*if (dataInFifo.begin()->second._state == BlockState::READY) {
		dataInUse.emplace(dataInFifo.begin()->first, dataInFifo.begin()->second);
		result = dataInFifo.begin()->second._ptr;
		size = dataInFifo.begin()->second._size;
		dataInFifo.erase(dataInFifo.begin());
	}*/
	if (_queue.front()._state == BlockState::READY_FOR_READING) {
		result = _queue.front()._ptr;
	}
	_queue.pop_front();
	return result;
}

void DataFIFO::addFree(void* data)
{
	std::lock_guard<std::mutex> guard(_lock);
	size_t i = 0;
	auto it = bufferStateVector.begin();
	for (; i != bufferState.size(); i++, it++) {
		if (bufferStateVector[i]._ptr = data)
			bufferStateVector[i]._state = BlockState::FREE;
			break;
	}
	i--;
	if (bufferStateVector[i]._state == BlockState::FREE) {
		while (bufferStateVector[i + 1]._state == BlockState::FREE) {
			bufferStateVector[i]._size += bufferStateVector[i + 1]._size;
			it = bufferStateVector.erase(it);
		}
	}
}

void* DataFIFO::foundFreePlace(size_t size)
{
	void* result = nullptr;
	size_t offset = 0;
	for (auto it = bufferStateVector.begin(); it != bufferStateVector.end(); it++) {
		if (it->_size >= size && it->_state == BlockState::FREE) {
			result = static_cast<char*>(_data) + offset;
			bufferStateVector.insert(it, Block::Block(size, result, BlockState::INUSE));
			//bufferStateVector. //.insert(it, Block::Block(size, result, BlockState::INUSE));
			it->_ptr = static_cast<char*>(_data) + offset + it->_size;
			it->_size -= size;
			if (it->_size == 0)
				bufferStateVector.erase(it);
			break;
		}
		offset += it->_size;
	}
	return result;
	/*size_t freeSpaceCount = 0;
	//size_t ptrOffset = 0;
	size_t i = _dataPrtOffset;
	
	while(i < _bufferSize && i + 1 != _dataPrtOffset) {
		if (byteState[i] == EMPTY) 
			break;
		i = (i + 1) % _bufferSize;
	}
	
	freeSpaceCount++;
	byteState[i + freeSpaceCount - 1] = FILLED;
	while (i + freeSpaceCount < _bufferSize && i + freeSpaceCount + 1 != _dataPrtOffset) {
		if (byteState[i + freeSpaceCount] == FILLED)
			break;
		if (freeSpaceCount == size)
			break;
		byteState[i + freeSpaceCount] = FILLED;
		freeSpaceCount++;
	}

	if (freeSpaceCount >= size) {
		return static_cast<char*>(_data) + i;
	} else {
		return nullptr;
	}*/

}

