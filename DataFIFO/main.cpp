#include "DataFifo.h"
#include <iostream>
#include <fstream>

void writerToFifo(DataFIFO& fifo, std::istream& in, bool& isWriterWork) {
	isWriterWork = true;
	//std::cout << "begin writing " << std::endl;
	char* data = nullptr;
	std::string str;
	while (in >> str) {
		while (data == nullptr)
			data = static_cast<char*>(fifo.getFree(str.size() + 1));
		std::copy(str.c_str(), str.c_str() + str.size() + 1, data);
		fifo.addReady(data);
		data = nullptr;
	}
	isWriterWork = false;
	//std::cout << "end writing " << std::endl;
}

void readerFromFifo(DataFIFO& fifo, std::ostream& out, bool& isWriterWork) {
	//std::cout << "begin reading " << std::endl;
	while (!isWriterWork) { std::cout << " wait... "; }
	char* data = nullptr;
	while (!fifo.isQueueEmpty() || isWriterWork == true) {

		size_t size;
		while (data == nullptr) {
			//std::cout << " read " << std::endl;
			data = static_cast<char*>(fifo.getReady(size));
			if (isWriterWork == true)
				break;
		}
		if (data != nullptr) {
			//std::cout << " out " << std::endl;
			out << data << " ";
		}
		fifo.addFree(data);
		data = nullptr;
	}
	//std::cout << "end reading " << std::endl;
}

int main() {
	std::ifstream in("multiTest.txt");
	std::ofstream out("out.txt");
	//(in.is_open());
	DataFIFO fifo(1000, 10);

	bool sync = false;
	std::thread writeToFifo(writerToFifo, std::ref(fifo), std::ref(in), std::ref(sync));
	std::thread readFromFifo(readerFromFifo, std::ref(fifo), std::ref(out), std::ref(sync));
	writeToFifo.join();
	readFromFifo.join();
	out.close();
	in.close();
}