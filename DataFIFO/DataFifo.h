#pragma once
#include <memory>
#include <mutex>
#include <map>

class DataFIFO {
public:
	DataFIFO(size_t bufferSize, size_t maxBlocksCount); //выделяет буфер данных заданного размера, с максимальным количеством блоков.
	~DataFIFO();//деструктор, освобождает память
	void* getFree(size_t size); //метод, вызываемый потоком - писателем, для проверки наличия size свободных байт; возвращает начальный адрес блока
	void addReady(void* data); //метод, вызываемый потоком-писателем, помечающий, что запрошенный ранее блок заполнен и готов к передаче.
	void* getReady(size_t& size);//метод, вызываемый потоком-читателем, для получения указателя на очередной готовый блок данных. Если готового блока нет, то функция возвращает null
	void addFree(void* data); //потоком-читателем, помечающий, что полученный ранее блок данных обработан, место освобождёно и может повторно использоваться очередью
private:
	std::mutex _lock;


	size_t _bufferSize;//максимальный размер буффера
	size_t _maxBlocksCount;//максимальное количество блоков
	
	void* _data;
	std::map<size_t, size_t> _askedBlocksSize;//map с адресом блока и его размером

	
	size_t _freeBytesCount; //количество свободных байт

	size_t _usedBlocksCount;//количество использованных блоков. Нужно для проверки заполненности fifo
	size_t _rightBorderOfUsedBytes;
	size_t _leftBorderOfUsedBytes;
	
	size_t _redyBlockCount;//количество готовых блоков, которые может получить читатель
	size_t _rightBorderOfReadyBytes;
	size_t _leftBorderOfReadyBytes;
};