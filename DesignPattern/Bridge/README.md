```c++
Abstraction 与 AbstractionImp 为各自独立的抽象类
ConcreteAbstractionImpA 与 ConcreteAbstractionImpB 是 AbstractionImp 的两个实现
RefinedAbstraction 是 Abstraction 的实现，组合 AbstractionImp 对象_imp,将 Operation 行为交给 _imp 实现
```
