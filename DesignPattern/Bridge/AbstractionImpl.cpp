#include "AbstractionImpl.h"

#include <iostream>
using std::cout;

ConcreteAbstractionImpB::ConcreteAbstractionImpB() {}

ConcreteAbstractionImpB::~ConcreteAbstractionImpB() {}

void ConcreteAbstractionImpB::Operation() {
    cout << "AbstractionImplB...imp..." << std::endl;
}

ConcreteAbstractionImpA::ConcreteAbstractionImpA() {}

ConcreteAbstractionImpA::~ConcreteAbstractionImpA() {}

void ConcreteAbstractionImpA::Operation() {
    cout << "AbstractionImplA...imp..." << std::endl;
}
AbstractionImp::~AbstractionImp() {}

AbstractionImp::AbstractionImp() {}

void AbstractionImp::Operation() {
	cout << "AbstractionImp...imp..." << std::endl;
}
