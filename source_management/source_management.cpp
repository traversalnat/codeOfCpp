#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <iostream>
#include <string>
#include <vector>

using testing::Test;
class Source_Management : public Test {
};

class Vector {
	public:
		explicit Vector(int sz);
		Vector(const Vector &x);
		Vector(Vector &&x);
		Vector& operator=(const Vector &x);
		Vector& operator=(Vector &&x);
		double& operator[](int i);

        int size() {
			return sz;
		}

		~Vector();

	private:
		int sz;
		double *elem;
};

Vector::Vector(int sz) {
	if (sz < 0) {
		throw std::length_error("Vector sz should > 0");
	}
	elem = new double[sz];
	this->sz = sz;
}

Vector::Vector(const Vector &x) : sz{x.sz}, elem{new double[x.sz]} {
    for (int i = 0; i < sz; i++) {
        elem[i] = x.elem[i];
    }
}

Vector::Vector(Vector &&x) : sz{x.sz}, elem{x.elem} {
	x.elem = nullptr;
	x.sz = 0;
}

Vector::~Vector() {
	delete[] elem;
}

Vector& Vector::operator=(const Vector &x) {
	double *p = new double[x.sz];
	for (int i = 0; i < x.sz; i++) {
		p[i] = x.elem[i];
	}

	delete[] elem;
	elem = p;
	sz = x.sz;
	return *this;
}

Vector& Vector::operator=(Vector &&x) {
	elem = x.elem;
	sz = x.sz;
	x.elem = nullptr;
	x.sz = 0;
	return *this;
}

double& Vector::operator[](int i) {
	return elem[i];
}

bool operator==(Vector a, Vector b) {
	if (a.size() != b.size()) {
		return false;
	}
	for (int i = 0; i < a.size(); i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}


TEST_F(Source_Management, copy_assignment) {
	Vector v1(3);
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	Vector v2{v1};
	Vector v3 = v1;
	ASSERT_EQ(v1, v2);
	ASSERT_EQ(v1, v3);
}

TEST_F(Source_Management, move_construct) {
	Vector v1(3);
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	Vector v2{std::move(v1)};
	Vector v3(0);
	ASSERT_EQ(v1, v3);
}


TEST_F(Source_Management, move_assignment) {
	Vector v1(3);
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	Vector v2 = std::move(v1);
	Vector v3(0);
	ASSERT_EQ(v1, v3);
}
