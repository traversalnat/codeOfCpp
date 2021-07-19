#include "Abstraction.h"

#include "AbstractionImpl.h"
Abstraction::~Abstraction() {}

Abstraction::Abstraction() {}

RefinedAbstraction::RefinedAbstraction(AbstractionImp* imp) {
	_imp = imp;
}

RefinedAbstraction::~RefinedAbstraction() {}

void RefinedAbstraction::Operation() {
	_imp->Operation();
}
