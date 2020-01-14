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
	DataFIFO(size_t bufferSize, size_t maxBlocksCount); //�������� ����� ������ ��������� �������, � ������������ ����������� ������.
	~DataFIFO();//����������, ����������� ������
	void* getFree(size_t size); //�����, ���������� ������� - ���������, ��� �������� ������� size ��������� ����; ���������� ��������� ����� �����
	void addReady(void* data); //�����, ���������� �������-���������, ����������, ��� ����������� ����� ���� �������� � ����� � ��������.
	void* getReady(size_t& size);//�����, ���������� �������-���������, ��� ��������� ��������� �� ��������� ������� ���� ������. ���� �������� ����� ���, �� ������� ���������� null
	void addFree(void* data); //�������-���������, ����������, ��� ���������� ����� ���� ������ ���������, ����� ���������� � ����� �������� �������������� ��������
private:
	std::mutex _lock;

	size_t _bufferSize;//������������ ������ �������
	size_t _maxBlocks;//������������ ���������� ������
	
	void* _data;
	std::map<size_t, Data> dataInFifo;
	std::map<size_t, Data> dataInUse;
	
	size_t _usedBytesCount;
	size_t _leftBorder;
	size_t _rightBorder;
};