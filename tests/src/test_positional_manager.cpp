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

    auto it = mgr.next();
    ASSERT_NE(it, mgr.end());
    ASSERT_TRUE((*it).second->is_named(TEST_ARG_NAME));

    it = mgr.next();
    ASSERT_NE(it, mgr.end());
    ASSERT_TRUE((*it).second->is_named(sec_name));
}

TEST(PosMgrTest, addArgs_iteratingResetsOnAdd) {
    PosArgManager mgr;
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME)
            .get()
    );

    auto it = mgr.next();
    ASSERT_NE(it, mgr.end());
    ASSERT_TRUE((*it).second->is_named(TEST_ARG_NAME));

    const std::string sec_name = std::string{TEST_ARG_NAME} + "2";
    mgr.add(
        PositionalArgBuilder(sec_name)
            .get()
    );

    it = mgr.next();
    ASSERT_NE(it, mgr.end());
    ASSERT_TRUE((*it).second->is_named(TEST_ARG_NAME));

    it = mgr.next();
    ASSERT_NE(it, mgr.end());
    ASSERT_TRUE((*it).second->is_named(sec_name));
}

TEST(PosMgrTest, addArg_nextReachesEnd) {
    PosArgManager mgr;
    mgr.add(
        PositionalArgBuilder(TEST_ARG_NAME)
            .get()
    );

    mgr.next();
    ASSERT_EQ(mgr.end(), mgr.next());
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

TEST(PosMgrTest, copyCtor_itPointsToNewMap) {
    PosArgManager mgr1;
    PosArgManager mgr2 = mgr1;
    
    ASSERT_NE(mgr1.next(), mgr2.next());
}