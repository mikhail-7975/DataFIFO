#include "DataFifo.h"
#include <iostream>

int main() {
	DataFIFO intFifo(sizeof(int) * 3 + 1, 4);
	auto data1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	*data1 = 100;
	*data2 = 200;

	intFifo.addReady(data2);
	intFifo.addReady(data1);
	size_t size;
	data1 = static_cast<int*>(intFifo.getReady(size));
	data1 = static_cast<int*>(intFifo.getReady(size));
	std::cout << *data1 << ' ' << *data2;
	intFifo.addFree(data1);
	intFifo.addFree(data2);
	return 0;
}