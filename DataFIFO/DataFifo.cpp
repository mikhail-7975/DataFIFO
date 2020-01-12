#include "DataFifo.h"

DataFIFO::DataFIFO(size_t bufferSize, size_t maxBlocksCount) {
	_bufferSize = bufferSize;
	_maxBlocksCount = maxBlocksCount;
	
	_redyBlockCount = 0;
	_usedBlocksCount = 0;

	_freeBytesCount = bufferSize;

	_leftBorderOfUsedBytes = 0;
	_rightBorderOfUsedBytes = 0;

	_data = operator new[](bufferSize);
}

DataFIFO::~DataFIFO()
{
	operator delete[](_data);
}

void* DataFIFO::getFree(size_t size)
{
	if (size > _freeBytesCount)
		return nullptr;

	std::lock_guard<std::mutex> guard(_lock);

	void* result = nullptr;
	if (_bufferSize - _rightBorderOfUsedBytes >= size) {
		result = static_cast<char*>(_data) + _rightBorderOfUsedBytes;
		_rightBorderOfUsedBytes = _rightBorderOfUsedBytes + size;
		_freeBytesCount -= size;
		_askedBlocksSize.insert(size_t(result), size);
	} else {
		if (size < _leftBorderOfUsedBytes) {
			result = _data;
			//_leftBorderOfUsedBytes = _rightBorderOfUsedBytes;
			_rightBorderOfUsedBytes = size;
			_askedBlocksSize.insert(size_t(result), size);
		}
	}
	return result;
}

void DataFIFO::addReady(void* data) {
	std::lock_guard<std::mutex> guard(_lock);
	
	if (_askedBlocksSize.size() == 0)
		return;

	_rightBorderOfReadyBytes = _leftBorderOfReadyBytes + _askedBlocksSize[size_t(data)];
}

void DataFIFO::addFree(void* data)
{
	std::lock_guard<std::mutex> guard(_lock);
	_leftBorderOfReadyBytes = _rightBorderOfReadyBytes;
}

