#include "gtest/gtest.h"
#include "positional_manager.hpp"
#include "positional_arg.hpp"

using namespace rcp;

constexpr char TEST_ARG_NAME[]{"test_arg_name"};

TEST(PosMgrTest, addArgs_accessByNext) {
    PosArgManager mgr;
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME)
            .get()
    );

    const std::string sec_name = std::string{TEST_ARG_NAME} + "2";
    mgr.add(
        PositionalArgBuilder(sec_name)
            .get()
    );

    auto res = mgr.get_next_after(-1);
    ASSERT_TRUE(res.get_ok()->is_named(TEST_ARG_NAME));

    res = mgr.get_next_after(res.get_ok()->get_order());
    ASSERT_TRUE(res.get_ok()->is_named(sec_name));
}

TEST(PosMgrTest, noArgs_returnsErrOnNext) {
    PosArgManager mgr;

    auto res = mgr.get_next_after(-1);
    ASSERT_TRUE(res.is_err());
}

TEST(PosMgrTest, addArg_getNextAfterReachesEnd) {
    PosArgManager mgr;
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME)
            .get()
    );

    auto res = mgr.get_next_after(-1);
    res = mgr.get_next_after(res.get_ok()->get_order());
    ASSERT_TRUE(res.is_err());
}

TEST(PosMgrTest, addArg_getArgByName) {
    PosArgManager mgr;
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME)
            .get()
    );
    ASSERT_TRUE(mgr.get(TEST_ARG_NAME).has_value());
}

TEST(PosMgrTest, noArg_getReturnsNull) {
    PosArgManager mgr;
    ASSERT_FALSE(mgr.get(TEST_ARG_NAME).has_value());
}

TEST(PosMgrTest, notRequiredArgs_requiredSatisfied) {
    PosArgManager mgr;
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME).get()
    );
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME).get()
    );

    ASSERT_TRUE(mgr.check_required_satisfied().is_ok());
}

TEST(PosMgrTest, requiredArg_requiredNotSatisfied) {
    PosArgManager mgr;
    mgr.add(
        PositionalArgBuilder("any")
            .get()
    );
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME)
            .required()
            .get()
    );

    ASSERT_TRUE(mgr.check_required_satisfied().get_err().find(TEST_ARG_NAME) != std::string::npos);
}

TEST(PosMgrTest, requiredArg_valueSetRequiredSatisfied) {
    PosArgManager mgr;
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME)
            .required()
            .get()
    );

    mgr.get_next_after(-1).get_ok()->set("");

    ASSERT_TRUE(mgr.check_required_satisfied().is_ok());
}

TEST(PosMgrTest, getNextAfter_minusValAlwaysReturnsFirst) {
    PosArgManager mgr;
    mgr.add(
        PositionalArgBuilder("any")
            .get()
    );
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME)
            .get()
    );

    auto res1 = mgr.get_next_after(-1).get_ok();
    auto res2 = mgr.get_next_after(-1).get_ok();
    ASSERT_EQ(res1, res2);
}

TEST(PosMgrTest, addArgsWithSameOrder_addThrows) {
    PosArgManager mgr;
    unsigned ord = 99;
    auto first = PositionalArgBuilder(TEST_ARG_NAME)
        .order(ord)
        .get();
    auto second = PositionalArgBuilder("any")
        .order(ord)
        .get();

    mgr.add(first);
    ASSERT_THROW(mgr.add(second), BuildError);
}
