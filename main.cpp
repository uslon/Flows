#include "gtest/gtest.h"

int main() {
#define DEBUG
#ifdef DEBUG
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif
}