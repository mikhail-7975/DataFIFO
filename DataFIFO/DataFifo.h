#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include <map>
#include <list>
#include <queue>

enum class BlockState {
	READY_FOR_WRITING,
	READY_FOR_READING,
	FREE,
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

struct Block final {
	size_t _size;
	void* _ptr;
	BlockState _state;
	Block(size_t size, void* ptr, BlockState state) : _size(size), _ptr(ptr), _state(state) {};
};

class DataFIFO {
public:
	DataFIFO(size_t bufferSize, size_t maxBlocksCount); //�������� ����� ������ ��������� �������, � ������������ ����������� ������.
	~DataFIFO();//����������, ����������� ������
	void* getFree(size_t size); //�����, ���������� ������� - ���������, ��� �������� ������� size ��������� ����; ���������� ��������� ����� �����
	void addReady(void* data); //�����, ���������� �������-���������, ����������, ��� ����������� ����� ���� �������� � ����� � ��������.
	void* getReady(size_t& size);//�����, ���������� �������-���������, ��� ��������� ��������� �� ��������� ������� ���� ������. ���� �������� ����� ���, �� ������� ���������� null
	void addFree(void* data); //�������-���������, ����������, ��� ���������� ����� ���� ������ ���������, ����� ���������� � ����� �������� �������������� ��������
	bool isBufferEmpty();
	bool isQueueEmpty();
private:
	std::mutex _lock;

	size_t _bufferSize;//������������ ������ �������
	size_t _maxBlocks;//������������ ���������� ������
	
	void* _data;
	
	std::vector<Block> bufferStateVector;
	std::deque<Block> _queue;

	void* foundFreePlace(size_t size);
	//size_t _dataPrtOffset;
};