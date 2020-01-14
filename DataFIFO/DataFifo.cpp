#include "DataFifo.h"

DataFIFO::DataFIFO(size_t bufferSize, size_t maxBlocksCount) {
	_bufferSize = bufferSize;
	_maxBlocks = maxBlocksCount;
	
	_usedBytesCount = 0;
	_leftBorder = 0;
	_rightBorder = 0;

	_data = operator new[](bufferSize);
}

DataFIFO::~DataFIFO()
{
	operator delete[](_data);
}

void* DataFIFO::getFree(size_t size)
{
	if (size > _bufferSize - _usedBytesCount)
		return nullptr;

	std::lock_guard<std::mutex> guard(_lock);
		
	
	void* result;
	if (_bufferSize - _rightBorder >= size) {
		result = static_cast<char*>(_data) + _rightBorder;
		_rightBorder += size;
	} else {
		if (_leftBorder > size) {
			result = _data;
			_rightBorder = size;
		} else {
			return nullptr;
		}
	}
	Data d(result, BlockState::CLEAR, size);
	dataInFifo.emplace(size_t(result), d);
	//dataMap.insert(, d);
		
		
		//insert(size_t(result), d);
	return result;
}

void DataFIFO::addReady(void* data) {
	std::lock_guard<std::mutex> guard(_lock);
	dataInFifo[size_t(data)]._state = BlockState::READY;
}

void* DataFIFO::getReady(size_t& size)
{
	std::lock_guard<std::mutex> guard(_lock);
	void* result = nullptr;
	if (dataInFifo.begin()->second._state == BlockState::READY) {
		dataInUse.emplace(dataInFifo.begin()->first, dataInFifo.begin()->second);
		result = dataInFifo.begin()->second._ptr;
		dataInFifo.begin()->second._state = BlockState::INUSE;
		size = dataInFifo.begin()->second._size;
		dataInFifo.erase(dataInFifo.begin());
	}
	return result;
}

void DataFIFO::addFree(void* data)
{
	std::lock_guard<std::mutex> guard(_lock);
	//dataMap[size_t(data)]._state = BlockState::READY;
	_leftBorder += dataInUse.begin()->second._size;
	if (_leftBorder == _rightBorder) {
		_leftBorder = 0;
		_rightBorder = 0;
	}
	dataInUse.erase(size_t(data));
}

