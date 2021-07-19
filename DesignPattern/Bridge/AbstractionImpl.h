#pragma once
#include "Abstraction.h"
#include "AbstractionImpl.h"
class Abstraction;

class AbstractionImp : public Abstraction
{
	public:
		virtual ~AbstractionImp();
		virtual void Operation() = 0;

	protected:
		AbstractionImp();
};

class ConcreteAbstractionImpA : public AbstractionImp
{
	public:
		ConcreteAbstractionImpA();
		~ConcreteAbstractionImpA();

		virtual void Operation();
};

class ConcreteAbstractionImpB : public AbstractionImp
{
	public:
		ConcreteAbstractionImpB();
		~ConcreteAbstractionImpB();

		virtual void Operation();
};
