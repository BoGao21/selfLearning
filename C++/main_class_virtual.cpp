/**
一、虚函数的基本实现机制
1.1 静态绑定（编译时）与动态绑定（运行时）
（1）静态绑定：非虚函数调用时，编译器根据指针或引用的声明类型决定调用的函数。
（2）动态绑定：虚函数调用时，编译器根据对象的实际类型（动态类型） 决定调用的函数。

1.2 虚函数表（vtable）与虚函数指针（vptr）
（1）每个包含虚函数的类，编译器在编译期为其生成一张静态的虚函数表（vtable），按声明顺序存放该类所有虚函数的地址，这点非常重要，要参看代码第一个例子。
（2）每个该类对象的内存布局中，通常包含一个或多个 vptr（分别对应每个直接基类的虚函数表或自身的虚函数表），其中第一个 vptr 通常位于对象的起始位置，其余 vptr 位于对应基类子对象的起始处（即对象内部偏移位置）。vptr 的大小固定（32 位占 4 字节，64 位占 8 字节）。
（3）虚函数表（vtable）本身不在对象内部，而是存放在只读数据段。对象中只存一个固定大小的 vptr，vptr指向vtable，因此，vtable 的大小完全不参与对象内存布局的计算，只算vptr。

1.3 虚函数调用的底层三步骤
（1）通过对象的 vptr 找到虚函数表（vtable）的起始地址。
（2）在 vtable 中按编译期计算好的固定偏移量取出函数指针。
（3）通过该指针调用函数，并传入正确的 this 指针。

二、构造函数与析构函数中的虚函数行为
2.1 构造函数中的虚函数调用
（1）行为：调用的是当前正在构造的类的版本，绝不会调用派生类的覆盖版本。
（2）底层原因：构造顺序严格从基类到派生类。基类构造时，派生类成员尚未初始化，此时对象的 vptr 被设置为当前正在构造的类的虚函数表地址。随着构造链推进，vptr 会逐步被覆盖为子类的表。
（3）铁律：绝对不要在构造函数中调用虚函数。这会导致代码行为怪异且难以调试，除非你明确知道只会调用本类实现。
（4）构造函数绝对不能是虚函数

2.2 析构函数中的虚函数调用
（1）行为：与构造相反，析构顺序从派生类到基类。vptr 会逐步恢复为基类的虚函数表，因此析构中调用的虚函数也是当前正在析构的类的版本。
（2）基类析构函数必须为虚函数：这是 C++ 内存安全的核心规则。若基类析构函数非虚，delete 基类指针 将只执行基类析构，派生类析构函数不被调用，导致资源泄漏。

三、纯虚函数与抽象类
3.1 纯虚函数与抽象类定义
（1）语法：virtual void func() = 0;
（2）包含至少一个纯虚函数的类即为抽象类，这个类无法实例化。
（3）派生类必须重写所有纯虚函数，否则仍然为抽象类。

3.2 纯虚函数可以有独立实现（类外定义）
（1）语法限制：纯虚函数的定义（函数体）必须在类外部提供，不允许在类内部直接写函数体。
（2）用途：为派生类提供一个“默认/兜底实现”。派生类虽然必须重写该纯虚函数才能实例化，但在其重写版本中可以通过 Base::func() 显式调用基类的默认实现。
（3）纯虚析构函数例外：纯虚析构函数必须提供函数体，因为析构函数在对象生命周期结束时总是会被调用，编译器需要找到一个可执行的函数地址。

四、访问权限与虚函数重写（深水区）
4.1 私有虚函数可以被派生类重写
（1）核心原理：访问权限（private / protected / public）是编译时的静态语法检查，而虚函数重写是运行时的动态行为。两者互不干涉。
（2）派生类重写基类的 private virtual 函数完全合法，且通过基类指针调用时，动态绑定依然会找到派生类的实现。

4.2 重写时访问权限可以改变
派生类重写基类虚函数时，可将其改为 public、protected 或保持 private，完全由派生类决定，编译器允许。但为了遵守里氏替换原则（LSP），通常建议保持与基类一致或更宽松。

4.3 NVI（非虚接口）模式下的访问与重写规则（核心提炼）
设计模式定义：基类提供 public 的非虚函数作为对外接口（管控流程如加锁/日志），内部调用 private/protected 虚函数，由派生类重写注入具体实现。
（1）规则一（继承接口调用）：子类调用继承来的公有接口时，动态绑定会命中子类自身重写的私有虚函数。原因：权限检查在基类编译时已通过，运行时只看对象实际类型（vptr）。
（2）规则二（子类重写接口后调用）：若子类重写该公有接口，并在内部调用该虚函数，则优先在子类作用域找到自身重写版本（编译通过）。若强行写成 Base::func() 试图调用基类版本，则会因 private 权限触发编译报错。
（3）规则三（未重写底层函数）：若子类未重写底层私有虚函数，却在自身接口中调用它，名称查找会回溯到基类并触发私有权限报错。这揭示了本质：子类能调用该函数，是因为它自己重写了，名称查找停在了自身作用域，绕开了基类的私有权限锁。

五、现代 C++ 安全关键字 —— override 与 final
5.1 override 的作用（编译期检查器）
（1）显式声明意图：告诉编译器和阅读者，此函数意图重写基类虚函数。
（2）强制签名匹配检查：若函数签名（参数类型、个数、const 性、返回类型（协变返回除外））与基类任一虚函数不匹配，编译器报错终止。
（3）价值：避免因手误（如漏写 const、参数类型写错）导致“静默隐藏”——即派生类定义了一个与基类无关的新虚函数，而非重写，导致多态失效，这种 bug 极难排查。
（4）最佳实践：所有重写基类虚函数的派生类函数，末尾一律加上 override。

5.2 final 的作用（终结器）
（1）修饰虚函数：virtual void func() final; 表示该虚函数是“最终版本”，禁止派生类进一步重写它。
（2）修饰类：class Derived final : public Base {}; 表示该类禁止被作为基类继承。
（3）与 override 共存：void func() override final; 表示“我是重写，同时也是最终版本，后代不要再动”。
（4）编译期优化加成：当编译器确定通过派生类指针（而非基类指针）调用被 final 修饰的虚函数时，可将其去虚拟化（静态绑定），省去 vptr 查表开销，这是少数能抵消虚函数性能损耗的手段。

六、多继承与虚继承中的内存模型与 thunk 机制（最复杂核心）
6.0 菱形继承的前置澄清
（1）问题：class Base1 : public Grand; class Base2 : public Grand; class Derived : public Base1, Base2; 不使用 virtual 继承，编译能通过吗？
（2）答案：编译完全能通过，不会报错。但会导致对象中包含两份独立的 Grand 子对象（数据冗余），且访问 Grand 成员时会产生二义性（需加 Base1:: 或 Base2:: 限定）。为了解决这两个问题，才引入 virtual 继承共享唯一基类。

6.1 普通多继承（Derived : public Base1, public Base2）—— Thunk 固定偏移
（1）内存布局（64位）

class Base1 { virtual void f1(); int b1; };  // 16字节
class Base2 { virtual void f2(); int b2; };  // 16字节
class Derived : public Base1, public Base2 {
    virtual void f1() override;
    virtual void f2() override;
    virtual void extra();
    int d;
};

Derived 对象内存布局：
+0:  [ vptr1 ]   --------> 指向 Base1 部分虚表 (extra挂在此表末尾)
+8:  [ Base1::b1 ]
+12: [ vptr2 ]   --------> 指向 Base2 部分虚表
+20: [ Base2::b2 ]
+24: [ Derived::d ]

（2）Thunk 机制（普通继承）
- 触发条件：通过 Base2* 指针调用被 Derived 重写的 f2() 时，this 指向 Base2 子对象（+12 偏移处），而 Derived::f2() 需要 this 指向完整对象头部（+0）。
- 解决方案：编译器在 Base2 虚表中放入一段 thunk（汇编调整块），执行 this -= 12（编译期绝对常量），修正指针后跳转至真正的 Derived::f2。普通继承的 thunk 是固定数值加减，效率极高。

6.2 虚继承（菱形继承）—— 引入 vbptr 与动态 Thunk
（1）虚继承使共享虚基类在派生类对象中的位置在编译基类时无法确定（取决于最终派生类大小）。
（2）编译器引入 虚基类指针（vbptr） ，指向虚基类偏移表（vbtable），表中存储共享基类的动态偏移量。
（3）内存布局（GCC/Clang，共享基类放末尾）

+0:  [ vptr1 ]   +8:  [ vbptr1 ]   +16: [ Base1::b1 ]
+20: [ vptr2 ]   +28: [ vbptr2 ]   +36: [ Base2::b2 ]
+40: [ Derived::d ]
+44: [ vptr_Grand ] +52: [ Grand::g ]  (唯一共享)

（3）Thunk 机制（虚继承）
动态查表：thunk 先读取 vbptr 获取共享基类偏移（如 +44），反推完整对象地址，再跳转。需两次寻址（查 vptr + 查 vbptr），性能开销大于普通继承。

**/


#include <iostream>
using namespace std;

// 1. Base/Derived 继承链：构造/析构 + NVI + 私有虚函数重写 + override/final 
class Base {
public:
    Base() { show(); }  // 构造中调用虚函数 → Base::show
    virtual ~Base() { show(); } // 析构中调用虚函数 → Base::show（虚析构）
    virtual void show() const { cout << "Base::show" << endl; }

    // NVI 入口（场景一：子类未重写此接口，直接调用）
    void publicInterface() {
        doWork(); // 委托给私有虚函数
    }
    // 场景二：子类可重写此公有虚函数
    virtual void anotherPublic() {
        doWork();
    }
private:
    virtual void doWork() { cout << "Base::doWork" << endl; }
};

class Derived : public Base {
public:
    void show() const override { cout << "Derived::show" << endl; }
    
    // 重写私有虚函数，并改为 public（合法）
    void doWork() override { cout << "Derived::doWork" << endl; }
    
    // 重写公有虚函数（场景二验证）
    void anotherPublic() override {
        cout << "Derived::anotherPublic -> ";
        doWork(); // 优先在自身作用域找到 Derived::doWork
        // Base::doWork(); // 取消注释会报错：Base::doWork 是 private
    }
};

// 2. Shape/Circle 继承链：纯虚函数默认实现
class Shape {
public:
    virtual double area() const = 0; // 纯虚函数
};
// 纯虚函数类外默认实现（C++98起支持）
double Shape::area() const {
    return 0.0;
}

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override {
        // 调用基类默认实现（必须通过对象，this 隐式转换）
        double base = this->Shape::area(); // 或直接 Shape::area() 也可
        return base + 3.14159 * radius * radius;
    }
};

// 3. 多继承与虚继承（菱形结构）
class Grand {
public:
    virtual void f() { cout << "Grand::f" << endl; }
    int g;
};
class Base1 : virtual public Grand {
public:
    int b1;
    void f() override { cout << "Base1::f" << endl; }
};
class Base2 : virtual public Grand {
public:
    int b2;
    void f() override { cout << "Base2::f" << endl; }
};
// 菱形虚继承
class MultiDerived : public Base1, public Base2 {
public:
    int d;
    void f() override { cout << "MultiDerived::f (thunk adjusted)" << endl; }
    virtual void extra() { cout << "MultiDerived::extra" << endl; }
};

// 4. final 用法演示 
class NoChild final {};
class Parent { public: virtual void step1() { cout << "Parent::step1" << endl; } };
class Child : public Parent {
public:
    void step1() override final { cout << "Child::step1 (final)" << endl; }
};
// class GrandChild : public Child { void step1() override {} }; // 错误：final

// 主测试
int main() {
    cout << " 1. 构造/析构中的虚函数" << endl;
    Derived* d1 = new Derived(); // 构造输出 Base::show
    delete d1;                   // 析构输出 Base::show

    cout << "\n 2. NVI 场景一：继承接口调用" << endl;
    Base* ptr = new Derived();
    ptr->publicInterface(); // 输出 Derived::doWork（动态绑定）
    delete ptr;

    cout << "\n 3. NVI 场景二：子类重写接口后调用" << endl;
    Derived d2;
    d2.anotherPublic(); // 输出 Derived::anotherPublic -> Derived::doWork

    cout << "\n 4. 纯虚函数默认实现" << endl;
    Circle c(1.0);
    cout << "Area of circle: " << c.area() << endl; // 输出 3.14159

    cout << "\n 5. 多继承 Thunk 调整" << endl;
    MultiDerived md;
    Base2* b2ptr = &md;
    b2ptr->f(); // 输出 MultiDerived::f（经过 thunk 动态调整 this）

    cout << "\n 6. 内存大小对比（64位" << endl;
    cout << "sizeof(MultiDerived): " << sizeof(MultiDerived) << " 字节（含vbptr）" << endl;

    cout << "\n 7. final 演示" << endl;
    Child c2;
    c2.step1(); // 调用 final 函数

    return 0;
}

/**
运行结果：
1. 构造/析构中的虚函数
Base::show  -- 构造过程中，先构造基类，当调用基类构造时，还没有子类，所以这时候使用的基类的vptr，调用的show还是基类的
Base::show

2. NVI 场景一：继承接口调用
Base::show
Derived::doWork -- 构造后，再使用vptr调用函数，调用的就是子类的
Base::show

3. NVI 场景二：子类重写接口后调用
Base::show
Derived::anotherPublic -> Derived::doWork

4. 纯虚函数默认实现
Area of circle: 3.14159

5. 多继承 Thunk 调整 
MultiDerived::f (thunk adjusted)

6. 内存大小对比（64位）
sizeof(MultiDerived): 48 字节（含vbptr）

7. final 演示
Child::step1 (final)
Base::show

**/
