#pragma once
#include <memory>
#include <mutex>
#include <map>

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
	size_t _maxBlocksCount;//������������ ���������� ������
	
	void* _data;
	std::map<size_t, size_t> _askedBlocksSize;//map � ������� ����� � ��� ��������

	
	size_t _freeBytesCount; //���������� ��������� ����

	size_t _usedBlocksCount;//���������� �������������� ������. ����� ��� �������� ������������� fifo
	size_t _rightBorderOfUsedBytes;
	size_t _leftBorderOfUsedBytes;
	
	size_t _redyBlockCount;//���������� ������� ������, ������� ����� �������� ��������
	size_t _rightBorderOfReadyBytes;
	size_t _leftBorderOfReadyBytes;
};