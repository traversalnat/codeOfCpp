#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Recorder {
	public:
		Recorder() {
			std::cout << "inited\n";
		}
		~Recorder() {
			std::cout << "free\n";
		}
};

using testing::Test;
class Free_Pointer : public Test {
	public:
		std::shared_ptr<Recorder> re;
		~Free_Pointer() {
		}
};

TEST_F(Free_Pointer, free_pointer_test) {
	re = std::shared_ptr<Recorder>(new Recorder());
	re = std::shared_ptr<Recorder>(new Recorder());

	double a = 1;
	double b = 3;

	double c = a / b;
	double d = c * b;

	ASSERT_DOUBLE_EQ(d, 1.0);
}

