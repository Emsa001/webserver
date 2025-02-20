#include <gtest/gtest.h>
#include <webserv.h>

int main(int argc, char *argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(demo, test) {
	EXPECT_EQ(testfun(), 42);
}