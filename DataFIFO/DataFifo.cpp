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
		
	
	void* result = nullptr;
	if (_bufferSize - _rightBorder >= size) {
		result = static_cast<char*>(_data) + _rightBorder;
	} else {
		if (_leftBorder > size) {
			result = _data;
			_rightBorder = size;
		}
	}
	Data d(result, BlockState::CLEAR, size);
	dataMap.emplace(size_t(result), d);
	//dataMap.insert(, d);
		
		
		//insert(size_t(result), d);
	return result;
}

void DataFIFO::addReady(void* data) {
	std::lock_guard<std::mutex> guard(_lock);
	dataMap[size_t(data)]._state = BlockState::READY;
}

void* DataFIFO::getReady(size_t& size)
{
	std::lock_guard<std::mutex> guard(_lock);
	void* result = nullptr;
	if (dataMap.begin()->second._state == BlockState::READY) {
		result = dataMap.begin()->second._ptr;
		dataMap.begin()->second._state = BlockState::INUSE;
	}
	return result;
}

void DataFIFO::addFree(void* data)
{
	std::lock_guard<std::mutex> guard(_lock);
	//dataMap[size_t(data)]._state = BlockState::READY;
	_leftBorder += dataMap.begin()->second._size;
	if (_leftBorder == _rightBorder) {
		_leftBorder = 0;
		_rightBorder = 0;
	}
	dataMap.erase(dataMap.begin());
}

