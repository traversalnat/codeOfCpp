#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

using testing::Test;
class Class : public Test {
};

class QMain {
	public:
		QMain() {
            std::cout << "QMain"
                      << "\n";
        }
		explicit QMain(QMain *parent = nullptr) {
            std::cout << "QMain -- parent"
                      << "\n";
        }
		~QMain() {}
};

class QWidget : public QMain {
	public:
		explicit QWidget(QMain *parent = nullptr);
		~QWidget();
};

// QA: QWidget 继承 QMain, 可直接调用 QMain 中的构造函数
// 这里调用 QMain(QMain *parent) 这个构造函数
QWidget::QWidget(QMain *parent)
	: QMain(parent) {
    std::cout << "QWidget"
              << "\n";
}

QWidget::~QWidget() {}

TEST_F(Class, class_test) {
	QWidget qwidget;
}

