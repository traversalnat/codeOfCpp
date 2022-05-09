#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "BaseConv.h"

using testing::Test;

class Base_Convert : public Test {};

TEST_F(Base_Convert, base_convert_test) {
    // 16 => 10
    BaseConv conv{0x10, 16};
    ASSERT_EQ("16", conv.get(10));

    // 8 => 10
    conv.reset(017, 8);
    ASSERT_EQ("15", conv.get(10));

    ASSERT_EQ(0, BaseConv::convertBin2Dec("0"));
    ASSERT_EQ(469, BaseConv::convertBin2Dec("111010101"));
    // max support 63 bit
    ASSERT_EQ(
        9223372036854775807,
        BaseConv::convertBin2Dec(
            "111111111111111111111111111111111111111111111111111111111111111"));

    ASSERT_EQ("0", BaseConv::convertDec2Bin(0));
    ASSERT_EQ("111010101", BaseConv::convertDec2Bin(469));
    // max support 63 bit
    ASSERT_EQ("111111111111111111111111111111111111111111111111111111111111111",
              BaseConv::convertDec2Bin(9223372036854775807));

    // 10 => 2
    conv.reset(469, 10);
    ASSERT_EQ("111010101", conv.get(2));

    // 16 => 2
    conv.reset(0x10, 16);
    ASSERT_EQ("10000", conv.get(2));

    // 8 => 2
    conv.reset(020, 16);
    ASSERT_EQ("10000", conv.get(2));

    // 8 => 16
    conv.reset(017, 8);
    ASSERT_EQ("f", conv.get(16));

    // 16 => 8
    conv.reset(0xf, 16);
    ASSERT_EQ("17", conv.get(8));

    // 16 => ascii
    ASSERT_EQ("rld", BaseConv::convertHex2Ascii(0x646C72));
    // ascii => 16
    ASSERT_EQ("726c64", BaseConv::convertAsciiToHex("rld"));
}
