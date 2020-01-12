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

	void* result;
	if (_leftBorderOfUsedBytes > _rightBorderOfUsedBytes) {
		result = static_cast<char*>(_data) + _rightBorderOfUsedBytes;
		_rightBorderOfUsedBytes = _rightBorderOfUsedBytes + size;
		_freeBytesCount -= size;
	} else {
		//подумать 
	}
	return result;
}

