#include "gtest/gtest.h"
#include "../DataFIFO/DataFifo.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(IntTest, oneUsedBlock) {
	DataFIFO intFifo(sizeof(int) * 1, sizeof(int));
	auto data1 = static_cast<int*>(intFifo.getFree(sizeof(int)));
	*data1 = 100;
	intFifo.addReady(data1);
	size_t size;
	data1 = static_cast<int*>(intFifo.getReady(size));
	ASSERT_NO_THROW(intFifo.addFree(data1));
	EXPECT_EQ(*data1, 100);
}