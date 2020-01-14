#include "gtest/gtest.h"
#include "../DataFIFO/DataFifo.h"

TEST(SingleThread, oneBlock) {
	DataFIFO intFifo(sizeof(int) * 1, sizeof(int));
	auto data1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	*data1 = 100;
	intFifo.addReady(data1);
	size_t size;

	auto checkdata1 = static_cast<int*>(intFifo.getReady(size));
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
	ASSERT_NO_THROW(intFifo.addFree(data1));
	ASSERT_NO_THROW(intFifo.addFree(data2));
	ASSERT_NO_THROW(intFifo.addFree(data3));
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
	auto checkdata2 =    static_cast<int*>(intFifo.getFree(size));
	auto checkdata3 = static_cast<double*>(intFifo.getFree(size));
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
	intFifo.addFree(data3);
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