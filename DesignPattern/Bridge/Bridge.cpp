#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>
#include "Abstraction.h"
#include "AbstractionImpl.h"

using testing::Test;
class Bridge : public Test {
};

TEST_F(Bridge, Bridge_test) {
	AbstractionImp *imp = new ConcreteAbstractionImpA();
	Abstraction *abs = new RefinedAbstraction(imp);
	abs->Operation();
}
