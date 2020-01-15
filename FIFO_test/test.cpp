#include "gtest/gtest.h"
#include "../DataFIFO/DataFifo.h"

TEST(SingleThread, oneBlock) {
	DataFIFO intFifo(sizeof(int) * 1, sizeof(int));
	auto data1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	*data1 = 100;
	intFifo.addReady(data1);
	size_t size;

	auto checkdata1 = static_cast<int*>(intFifo.getReady(size));
	intFifo.addFree(data1);
	ASSERT_NO_THROW(intFifo.addFree(data1));
	EXPECT_EQ(*checkdata1, 100);
	EXPECT_EQ(size, sizeof(int));
}

TEST(SingleThread, checkAddres) {
	DataFIFO intFifo(sizeof(int) * 3, sizeof(int));
	auto data1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	intFifo.addReady(data1);
	intFifo.addReady(data2);
	intFifo.addReady(data3);
	size_t size;
	auto checkdata1 = static_cast<int*>(intFifo.getReady(size));
	auto checkdata2 = static_cast<int*>(intFifo.getReady(size));
	auto checkdata3 = static_cast<int*>(intFifo.getReady(size));
	intFifo.addFree(data1);
	ASSERT_NO_THROW(intFifo.addFree(data2));
	(intFifo.addFree(data3));
	EXPECT_EQ(checkdata1, data1);
	EXPECT_EQ(checkdata2, data2);
	EXPECT_EQ(checkdata3, data3);
}

TEST(SingleThread, manyBlocks) {
	DataFIFO intFifo(sizeof(int) * 3, sizeof(int));
	auto data1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	*data1 = 100;
	*data2 = 200;
	*data3 = 300;
	intFifo.addReady(data1);
	intFifo.addReady(data2);
	intFifo.addReady(data3);
	size_t size;
	auto checkdata1 = static_cast<int*>(intFifo.getReady(size));
	auto checkdata2 = static_cast<int*>(intFifo.getReady(size));
	auto checkdata3 = static_cast<int*>(intFifo.getReady(size));
	ASSERT_NO_THROW(intFifo.addFree(data1));
	ASSERT_NO_THROW(intFifo.addFree(data2));
	ASSERT_NO_THROW(intFifo.addFree(data3));
	EXPECT_EQ(*checkdata1, 100);
	EXPECT_EQ(*checkdata2, 200);
	EXPECT_EQ(*checkdata3, 300);
}

TEST(SingleThread, manyBlocks_randomFree) {
	DataFIFO intFifo(sizeof(int) * 3, 3);
	auto data1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	*data1 = 100;
	*data2 = 200;
	*data3 = 300;
	intFifo.addReady(data1);
	intFifo.addReady(data3);
	intFifo.addReady(data2);
	size_t size;
	auto checkdata1 = static_cast<int*>(intFifo.getReady(size));
	auto checkdata2 = static_cast<int*>(intFifo.getReady(size));
	auto checkdata3 = static_cast<int*>(intFifo.getReady(size));
	ASSERT_NO_THROW(intFifo.addFree(data3));
	ASSERT_NO_THROW(intFifo.addFree(data1));
	ASSERT_NO_THROW(intFifo.addFree(data2));
	EXPECT_EQ(*checkdata1, 100);
	EXPECT_EQ(*checkdata2, 200);
	EXPECT_EQ(*checkdata3, 300);
}

TEST(SingleThread, differentSize) {
	DataFIFO intFifo(100, 100);
	auto data1 = static_cast<char*>(intFifo.getFree(sizeof(char)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<double*>(intFifo.getFree(sizeof(double)));
	*data1 = 'a';
	*data2 = 200;
	*data3 = 300.;
	intFifo.addReady(data1);
	intFifo.addReady(data3);
	intFifo.addReady(data2);
	size_t size;
	auto checkdata1 =   static_cast<char*>(intFifo.getReady(size));
	auto checkdata2 =    static_cast<int*>(intFifo.getReady(size));
	auto checkdata3 = static_cast<double*>(intFifo.getReady(size));
	ASSERT_NO_THROW(intFifo.addFree(data3));
	ASSERT_NO_THROW(intFifo.addFree(data1));
	ASSERT_NO_THROW(intFifo.addFree(data2));
	EXPECT_EQ(*checkdata1, 'a');
	EXPECT_EQ(*checkdata2, 200);
	EXPECT_EQ(*checkdata3, 300.);
}

TEST(SingleThread, notReadyFirst) {
	DataFIFO intFifo(100, 100);
	auto data1 = static_cast<char*>(intFifo.getFree(sizeof(char)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<double*>(intFifo.getFree(sizeof(double)));
	*data1 = 'a';
	*data2 = 200;
	*data3 = 300.;
	intFifo.addReady(data3);
	intFifo.addReady(data2);
	size_t size;
	auto checkdata1 =  static_cast<char*>(intFifo.getReady(size));
	auto checkdata2 =    static_cast<int*>(intFifo.getReady(size));
	auto checkdata3 = static_cast<double*>(intFifo.getReady(size));
	EXPECT_EQ(checkdata1, nullptr);
	EXPECT_EQ(checkdata2, nullptr);
	EXPECT_EQ(checkdata3, nullptr);
}

TEST(SingleThread, notReadyInMiddle) {
	DataFIFO intFifo(100, 100);
	auto data1 = static_cast<char*>(intFifo.getFree(sizeof(char)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<double*>(intFifo.getFree(sizeof(double)));
	*data1 = 'a';
	*data2 = 200;
	*data3 = 300.;
	intFifo.addReady(data1);
	intFifo.addReady(data3);
	size_t size;
	auto checkdata1 =   static_cast<char*>(intFifo.getReady(size));
	auto checkdata2 =    static_cast<int*>(intFifo.getReady(size));
	auto checkdata3 = static_cast<double*>(intFifo.getReady(size));
	EXPECT_EQ(checkdata1, data1);
	EXPECT_EQ(checkdata2, nullptr);
	EXPECT_EQ(checkdata3, nullptr);
}

TEST(SingleThread, notReadyAtTheEnd) {
	DataFIFO intFifo(100, 100);
	auto data1 = static_cast<char*>(intFifo.getFree(sizeof(char)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<double*>(intFifo.getFree(sizeof(double)));
	*data1 = 'a';
	*data2 = 200;
	*data3 = 300.;
	intFifo.addReady(data1);
	intFifo.addReady(data2);
	size_t size;
	auto checkdata1 =   static_cast<char*>(intFifo.getReady(size));
	auto checkdata2 =    static_cast<int*>(intFifo.getReady(size));
	auto checkdata3 = static_cast<double*>(intFifo.getReady(size));
	EXPECT_EQ(checkdata1, data1);
	EXPECT_EQ(checkdata2, data2);
	EXPECT_EQ(checkdata3, nullptr);
}
TEST(SingleThread, badConsistencyOfOperation) {
	DataFIFO intFifo(100, 100);
	auto data1 = static_cast<char*>(intFifo.getFree(sizeof(char)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<double*>(intFifo.getFree(sizeof(double)));
	*data1 = 'a';
	*data2 = 200;
	*data3 = 300.;
	size_t size;
	intFifo.addReady(data1);
	auto checkdata1 = static_cast<char*>(intFifo.getReady(size));
	ASSERT_NO_THROW(intFifo.addFree(data1));
	intFifo.addReady(data2);
	auto checkdata2 = static_cast<int*>(intFifo.getReady(size));
	ASSERT_NO_THROW(intFifo.addFree(data2));
	intFifo.addReady(data3);
	auto checkdata3 = static_cast<double*>(intFifo.getReady(size));
	ASSERT_NO_THROW(intFifo.addFree(data3));
	EXPECT_EQ(*checkdata1, 'a');
	EXPECT_EQ(*checkdata2, 200);
	EXPECT_EQ(*checkdata3, 300.);
}

TEST(SingleThread, badConsistencyOfOperation_2) {
	DataFIFO intFifo(100, 100);
	auto data1 = static_cast<char*>(intFifo.getFree(sizeof(char)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<double*>(intFifo.getFree(sizeof(double)));
	*data1 = 'a';
	*data2 = 200;
	*data3 = 300.;
	size_t size;
	intFifo.addReady(data1);
	auto checkdata1 = static_cast<char*>(intFifo.getReady(size));
	intFifo.addReady(data2);
	auto checkdata2 = static_cast<int*>(intFifo.getReady(size));
	intFifo.addReady(data3);
	auto checkdata3 = static_cast<double*>(intFifo.getReady(size));
	ASSERT_NO_THROW(intFifo.addFree(data3));
	ASSERT_NO_THROW(intFifo.addFree(data1));
	ASSERT_NO_THROW(intFifo.addFree(data2));
	EXPECT_EQ(*checkdata1, 'a');
	EXPECT_EQ(*checkdata2, 200);
	EXPECT_EQ(*checkdata3, 300.);
}
TEST(SingleThread, bufferOverflow) {
	DataFIFO intFifo(sizeof(int) * 3 - 1, 3);
	auto data1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	EXPECT_EQ(data3, nullptr);
}

TEST(SingleThread, secondUsing) {
	DataFIFO intFifo(14, 14);
	auto data1 = static_cast<char*>(intFifo.getFree(sizeof(char)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<double*>(intFifo.getFree(sizeof(double)));
	*data1 = 'a';
	*data2 = 200;
	*data3 = 300.;
	size_t size;
	intFifo.addReady(data1);
	auto checkdata1 = static_cast<char*>(intFifo.getReady(size));
	intFifo.addReady(data2);
	auto checkdata2 = static_cast<int*>(intFifo.getReady(size));
	intFifo.addReady(data3);
	auto checkdata3 = static_cast<double*>(intFifo.getReady(size));
	//intFifo.addFree(data3);
	(intFifo.addFree(data1));
	(intFifo.addFree(data2));
	EXPECT_EQ(*checkdata1, 'a');
	EXPECT_EQ(*checkdata2, 200);
	EXPECT_EQ(*checkdata3, 300.);
	auto newdata1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	*newdata1 = 10;
	intFifo.addReady(newdata1);
	auto checknewdata1 = static_cast<int*>(intFifo.getReady(size));
	EXPECT_EQ(*checknewdata1, 10);
}

TEST(SingleThread, secondUsing_2) {
	DataFIFO intFifo(14, 14);
	auto data1 = static_cast<char*>(intFifo.getFree(sizeof(char)));
	auto data2 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	auto data3 = static_cast<double*>(intFifo.getFree(sizeof(double)));
	*data1 = 'a';
	*data2 = 200;
	*data3 = 300.;
	size_t size;
	intFifo.addReady(data1);
	auto checkdata1 = static_cast<char*>(intFifo.getReady(size));
	intFifo.addReady(data2);
	auto checkdata2 = static_cast<int*>(intFifo.getReady(size));
	intFifo.addReady(data3);
	auto checkdata3 = static_cast<double*>(intFifo.getReady(size));
	//intFifo.addFree(data3);
	ASSERT_NO_THROW(intFifo.addFree(data1));
	ASSERT_NO_THROW(intFifo.addFree(data2));
	EXPECT_EQ(*checkdata1, 'a');
	EXPECT_EQ(*checkdata2, 200);
	EXPECT_EQ(*checkdata3, 300.);
	auto newdata1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	*newdata1 = 10;
	intFifo.addReady(newdata1);
	auto checknewdata1 = static_cast<int*>(intFifo.getReady(size));
	EXPECT_EQ(*checknewdata1, 10);
}

TEST(SafetyTest, getReadyFromEmpty) {
	DataFIFO fifo(10, 10);
	size_t size;
	auto data = fifo.getReady(size);
	EXPECT_EQ(data, nullptr);
	EXPECT_EQ(size, 0);
}

TEST(SafetyTest, addFreeNotFromFifo) {
	int a;
	DataFIFO fifo(10, 10); 
	EXPECT_NO_THROW(fifo.addFree(&a));
}

TEST(SafetyTest, addFreeAlreadyFree) {
	//int a;
	DataFIFO fifo(10, 10);
	int* a = static_cast<int*>(fifo.getFree(sizeof(int)));
	fifo.addFree(a);
	EXPECT_NO_THROW(fifo.addFree(a));
	//EXPECT_NO_THROW(fifo.addFree(&a));
}

TEST(SafetyTest, addFreeNullptr) {
	DataFIFO fifo(10, 10);
	//int* a = static_cast<int*>(fifo.getFree(sizeof(int)));
	EXPECT_NO_THROW(fifo.addFree(nullptr));
}

#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <thread>
#include <functional>
#include <string>


#include <condition_variable>

/*
class DataFifoMultiTest : public ::testing::Test {
public:
	virtual void SetUp(void) {
		_CrtMemCheckpoint(&startup);
	}
	virtual void TearDown(void) {
		_CrtMemState teardown, diff;
		_CrtMemCheckpoint(&teardown);
		ASSERT_EQ(0, _CrtMemDifference(&diff, &startup, &teardown)) << "Memory leaks detected";
	}
	_CrtMemState startup;
};
*/


void writerToFifo(DataFIFO& fifo, std::istream& in, bool& sync) {

	char* data = nullptr;
	std::string str;
	while (in >> str) {
		while (data == nullptr)
			data = static_cast<char*>(fifo.getFree(str.size() + 1));
		std::copy(str.c_str(), str.c_str() + str.size() + 1, data);
		fifo.addReady(data);
		data = nullptr;
	}
	sync = true;
}

void readerFromFifo(DataFIFO& fifo, std::ostream& out, bool& sync) {

	char* data = nullptr;
	while (!fifo.isEmpty()) {

		size_t size;
		while (data == nullptr)
			data = static_cast<char*>(fifo.getReady(size));
		if (data != nullptr)
			out << data << " ";
		fifo.addFree(data);
		data = nullptr;
	}
}


TEST(DataFifoMultiTest, FishText) {

	std::ifstream in("multiTest.txt");
	std::ofstream out("out.txt");
	ASSERT_TRUE(in.is_open());
	DataFIFO fifo(1000, 10);

	bool sync = false;
	std::thread writeToFifo(writerToFifo, std::ref(fifo), std::ref(in), std::ref(sync));
	writeToFifo.join();
	std::thread readFromFifo(readerFromFifo, std::ref(fifo), std::ref(out), std::ref(sync));
	readFromFifo.join();
	out.close();
	in.close();
	EXPECT_EQ(1, 1);
}


TEST(DataFifoMultiTest, myOwnText) {

	std::istringstream in("My own super informatively text, so, i suppose my fantasy is not enought to do it, but im ready to try again and again");
	std::ostringstream out;

	DataFIFO fifo(1000, 10);

	bool sync = false;
	std::thread read(writerToFifo, std::ref(fifo), std::ref(in), std::ref(sync));
	std::thread write(readerFromFifo, std::ref(fifo), std::ref(out), std::ref(sync));

	read.join();
	write.join();

	ASSERT_TRUE(out.str() == "My own super informatively text, so, i suppose my fantasy is not enought to do it, but im ready to try again and again ");
}


TEST(DataFifoMultiTest, empty) {

	std::istringstream in("");
	std::ostringstream out;

	DataFIFO fifo(1000, 10);

	bool sync = false;
	std::thread read(writerToFifo, std::ref(fifo), std::ref(in), std::ref(sync));
	std::thread write(readerFromFifo, std::ref(fifo), std::ref(out), std::ref(sync));

	read.join();
	write.join();

	ASSERT_TRUE(out.str() == "");
}


TEST(DataFifoMultiTest, fishTest2) {

	std::istringstream in("Nam posuere vulputate est, vitae pellentesque ante. Sed vitae dolor vitae justo hendrerit faucibus laoreet commodo diam. ");
	std::ostringstream out;

	DataFIFO fifo(1000, 10);

	bool sync = false;
	std::thread read(writerToFifo, std::ref(fifo), std::ref(in), std::ref(sync));
	std::thread write(readerFromFifo, std::ref(fifo), std::ref(out), std::ref(sync));

	read.join();
	write.join();

	ASSERT_TRUE(out.str() == "Nam posuere vulputate est, vitae pellentesque ante. Sed vitae dolor vitae justo hendrerit faucibus laoreet commodo diam. ");
}