/**
const常用用法
const修饰优先级可以理解为“左结合”，优先修饰左边的类型，左边没有类型，则修饰其右边的类型
**/

#include <iostream>

using namespace std;

//函数与const
//常量参数：函数参数使用const,表示在函数内部不会修改该参数
void func1(const int *p)
{
    // *p = 100; 错误，不能修改
    cout << "*p=" << *p << endl;
}

//常量引用：常量引用常用于函数参数，避免拷贝，同时不允许修改原始数据
void func2(const int &x)
{
    // x = 10; 错误，不能修改
    cout << "x=" << x << endl;
}

//常量返回值：函数返回常量值，通常用于返回指针或引用，防止返回值被修改
const int & func3(const int &a)
{
    return a;
}


//常量成员函数：在成员函数后加const,表示该函数不会修改类的成员变量（除非成员变量被声明为mutable）
//const成员函数本质：const成员函数内部，this指针的类型是 const ClassName* const，const函数内部不能违反这个规则
//不能通过 this 修改任何非 mutable 成员变量,这是 C++ 为了保证调用者通过 const 对象或 const 引用调用该函数时，期望对象状态不会改变。
//const成员函数可以修改局部非const的变量值，mutable的成员变量，以及非指向常量的指针成员，虽然指针指向不能改，但是指针指向的内容可改
class A
{
    int value;
public:
    int getValue() const
    {
        //value = 10; 错误，不能修改成员变量
        return value;
    }
};


//const对象只能调用其自身的const成员函数
class Container {
    int data[10];
public:
    // 用于可修改对象的读写访问
    int& operator[](int idx) {
        return data[idx];
    }

    // 用于 const 对象的只读访问
    const int& operator[](int idx) const {
        return data[idx];
    }
};

void readOnly(const Container& c) {
    int x = c[0];      // 调用 const 版本
    // c[0] = 5;       // 错误，因为返回 const int&，不能赋值
}

void write(Container& c) {
    c[0] = 5;          // 调用非 const 版本，可以修改
}

//类与const
//常量成员变量：必须在构造函数的初始化列表初始化
//原因：因为 const 变量一旦定义（即内存分配完成）就必须立即拥有初始值，并且之后不能再改变;dd
//构造函数体执行时，成员变量已经被“定义”过了（按照初始化列表或默认值），此时再赋值就是修改 const 变量，编译器会报错。
//a.如果类中有const成员都没有提供类内初始化，并且没有显式定义构造函数，
//编译器生成的默认构造函数会尝试默认初始化const成员，这会导致编译错误（因为 const 成员未被初始化）。
//b.因此，只要类中有 const 成员，通常需要自定义构造函数并在初始化列表中初始化它们
//c.拷贝赋值运算符 会被删除（因为不能修改 const 成员），但 拷贝构造函数 可以正常工作（因为拷贝构造是初始化而不是赋值）
class MyClass
{
    const int constMember;
public:
    MyClass(int val) : constMember(val) {}
    int getValue() const
    {
        return constMember;
    }

};

//特殊情况：static const成员可以在类内直接初始化，或者单独在类外定义并初始化。
class Constants {
public:
    static const int MAX_SIZE = 100;   // 整型 static const 可以类内初始化
    static const std::string NAME;     // 非整型需要在类外定义
};
const std::string Constants::NAME = "example";

int main()
{
    //1.使用const 定义常量变量，必须在定义时初始化，且之后不能修改
    const int a = 10;
    //a = 20; 错误，不能修改常量
    cout << "a=" << a << endl;

    //2.指向常量的指针，指针指向的内容是常量，不能通过指针修改内容，但是指针本身可以指向其他地址
    const int * p1; //或者 int const* p
    int b = 5;
    p1 = &b;
    //*p = 10; 错误，不能通过指向常量的指针修改b的值
    cout << "*p1=" << *p1 << endl;
    int bb = 10;
    p1 = &bb; //正确，可以指向其他地址
    cout << "*p1=" << *p1 << endl;


    //3.常量指针，指针本身是常量，初始化后不能指向其他地址，但是可以通过指针修改指向的内容
    int * const p2 = &b;
    cout << "b="<<b<<", *p2=" << *p2 << endl;
    *p2 = 8;
    cout << "b="<<b<<", *p2=" << *p2 << endl;
    //p2 = &b; 错误，不能修改指针的指向

    //4.指向常量的常量指针，指针本身和指向的内容都是常量
    const int * const p3 = &b;
    // *p = 30; 错误，不能修改指向的内容
    // p = &bb; 错误，不能修改指针的指向

    func1(&b);
    func2(bb);
    const int res = func3(b);
    cout << "res=" << res << endl;
    // res = 18; 错误，不能通过res修改

    const MyClass obj(118);
    //5. 常量对象只能调用常量成员函数
    cout << "myclass.getValue=" << obj.getValue() << endl; //正确，getValue是常量成员函数，符合常量对象只能调用常量成员函数


    //6. const_cast类型转换，主要用于移除常量属性
    const int c = 10;
    int *p4 = const_cast<int*>(&c);
    *p4 = 18;
    cout << "*p4=" << *p4 << endl;


    return 0;
}

/**
运行结果：
a=10
*p1=5
*p1=10
b=5, *p2=5
b=8, *p2=8
*p=8
x=10
res=8
myclass.getValue=118
*p4=18
**/
