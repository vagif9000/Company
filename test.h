#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <cmath>
#include <string>

#define ASSERT_EQ(val1, val2) \
if (!((val1) == (val2))) { \
std::cerr << "Assertion failed: " << #val1 << " == " << #val2 \
<< " (" << (val1) << " != " << (val2) << ")" << std::endl; \
return false; \
}

#define ASSERT_TRUE(cond) \
if (!(cond)) { \
std::cerr << "Assertion failed: " << #cond << std::endl; \
return false; \
}

#define TEST(suiteName, testName) \
bool suiteName##_##testName()

#define RUN_TEST(suiteName, testName) \
do { \
std::cout << "Running " << #suiteName "." #testName << "... "; \
if (suiteName##_##testName()) { \
std::cout << "PASSED" << std::endl; \
} else { \
std::cout << "FAILED" << std::endl; \
} \
} while (0)

#endif
