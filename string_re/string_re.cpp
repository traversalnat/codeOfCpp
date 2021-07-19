#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>
#include<regex>

using testing::Test;
class String_Re : public Test {
};

TEST_F(String_Re, string_string) {
	std::string str_1 = "autoString";
}

TEST_F(String_Re, string_rstring) {
	auto str_1 = R"("this is a reg string")";
	ASSERT_STREQ("\"this is a reg string\"", str_1);

	auto str_2 = R"==(R"(xxx)")==";// 原样输出：R"(xxx)"
	ASSERT_STREQ("R\"(xxx)\"", str_2);
}

TEST_F(String_Re, re) {
	using std::regex;
	using std::smatch;
	using std::regex_search;

	auto reg_str {R"(\w{2}\s*\d{5}(-\d{4})?)"};
	std::cout << reg_str << "\n";
	regex pat {reg_str};
    std::string postal_code = "NY10200-1234";
	smatch matcher;
	regex_search(postal_code, matcher, pat);
	ASSERT_EQ(2, matcher.size());
	ASSERT_EQ(postal_code, matcher[0]);
	// 子匹配matchers[1] 为 -1234
	if (matcher.size() > 1 && matcher[1].matched) {
		ASSERT_EQ("-1234", matcher[1]);
	}
}

