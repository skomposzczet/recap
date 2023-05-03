#include "gtest/gtest.h"
#include "result.hpp"

using namespace rcp;

constexpr char TEST_ERR_STR[]{"test_err_str"};

TEST(ResultTest, createOk_isOkTrue) {
    const auto r = ResultFactory::ok();
    ASSERT_TRUE(r.is_ok());
}

TEST(ResultTest, createOk_getOkIsNull) {
    const auto r = ResultFactory::ok();
    ASSERT_EQ(nullptr, r.get_ok());
}

TEST(ResultTest, createOk_isErrFalse) {
    const auto r = ResultFactory::ok();
    ASSERT_FALSE(r.is_err());
}

TEST(ResultTest, createOkWithValue_getsValue) {
    int value = 99;
    const auto r = ResultFactory::ok<int>(value);
    ASSERT_EQ(value, r.get_ok());
}

TEST(ResultTest, createOk_getErrThrows) {
    int value = 99;
    const auto r = ResultFactory::ok<int>(value);
    ASSERT_THROW(r.get_err(), RcpError);
}

TEST(ResultTest, createErr_isErrTrue) {
    const auto r = ResultFactory::err(TEST_ERR_STR);
    ASSERT_TRUE(r.is_err());
}

TEST(ResultTest, createErr_isOkFalse) {
    const auto r = ResultFactory::err(TEST_ERR_STR);
    ASSERT_FALSE(r.is_ok());
}

TEST(ResultTest, createErr_getErrReturnsErrStr) {
    const auto r = ResultFactory::err(TEST_ERR_STR);
    ASSERT_EQ(TEST_ERR_STR, r.get_err());
}

TEST(ResultTest, createErr_getOkThrows) {
    const auto r = ResultFactory::err(TEST_ERR_STR);
    ASSERT_THROW(r.get_ok(), RcpError);
}
