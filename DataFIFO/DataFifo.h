#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include <map>

enum class BlockState {
	CLEAR,
	FILLED,
	READY,
	INUSE
};

struct Data final {
	Data() = default;
	Data(Data& copy) : _ptr(copy._ptr), _size(copy._size), _state(copy._state) {};
	Data(void* ptr, BlockState state, size_t size) : _ptr(ptr), _state(state), _size(size) {};
	void* _ptr;
	BlockState _state;
	size_t _size;
};

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
	size_t _maxBlocks;//максимальное количество блоков
	
	void* _data;
	std::map<size_t, Data> dataInFifo;
	std::map<size_t, Data> dataInUse;
	
	size_t _usedBytesCount;
	size_t _leftBorder;
	size_t _rightBorder;
};