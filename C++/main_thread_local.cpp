/*
thread_local 是 C++11 为线程安全引进的变量声明符
thread_local 是一个存储器指定符

所谓存储器指定符，其作用类似命名空间，指定了变量名的存储期以及链接方式

同类型的关键字还有：
auto：自动存储期(已废弃)
register：自动存储期，提示编译器将此变量置于寄存器中（已废弃）
static：静态或线程存储期，同时提示是内部链接
extern：静态或线程存储期，同时提示是外部链接
thread_local：线程存储期
mutable：不影响存储期或链接

线程存储期: 对象的存储在线程开始时分配，而在线程结束时解分配，每个线程拥有其自身的对象实例，可做到在线程内“全局”
唯有声明为 thread_local 的对象拥有此存储期
*/
#include <iostream>
#include <thread>

using namespace std;

void thread_func_show_1(const string &name)
{
    for (int i = 0; i < 3; i++)
    {
        thread_local int x = 1; //每个线程独立，自动static，只初始化一次
        int y = 1;
        x++;
        y++;
        cout << "thread[" << name << "]: x = " << x << endl;
        cout << "thread[" << name << "]: y = " << y << endl;
    }
    // x = 10; 但是作用域没有变化，作用域外访问不到
    return;
}

//定义类
class A {
public:
    A() {
        cout << "create A" << endl;
    }

    ~A() {
        cout << "destroy A" << endl;
    }

    int counter = 0;
    int get_value() {
        return counter++;
    }
};

A* creatA() {
    return new A();
}

void loopin_func_show_2(const string& thread_name) {
    thread_local A* a = creatA(); //thread_local变量只会初始化一次，所以只会构造一个对象
    cout << "thread[" << thread_name << "]: a.counter:" << a->get_value() << endl;
    return;
}

void thread_func_show_2(const std::string& thread_name) {
    for (int i = 0; i < 3; ++i) {
        loopin_func_show_2(thread_name);
    }
    return;
}


void loopin_func_show_3(const string& thread_name) {
    thread_local A* a; //thread_local初始化只有一次
    a = creatA();      //但是赋值操作可以多次，所以每次会构造一个新的对象赋值给a，也间接造成了内存泄漏
    cout << "thread[" << thread_name << "]: a.counter:" << a->get_value() << endl;
    return;
}

void thread_func_show_3(const std::string& thread_name) {
    for (int i = 0; i < 3; ++i) {
        loopin_func_show_3(thread_name);
    }
    return;
}

class B {
public:
    B() {
        std::cout << "create B" << std::endl;
    }
    ~B() {}
    thread_local static int b_key; //thread_local修饰类成员变量必须是static，并且仍然具有线程独立特性
    //thread_local int b_key;
    int b_value = 24;
    static int b_static;
};

thread_local int B::b_key = 12;
int B::b_static = 36;

void thread_func_show_4(const string& thread_name) {
    B b;
    for (int i = 0; i < 3; ++i) {
        b.b_key--;
        b.b_value--;
        b.b_static--;
        cout << "thread[" << thread_name << "]: b_key:" << b.b_key << ", b_value:" << b.b_value << ", b_static:" << b.b_static << endl;
        cout << "thread[" << thread_name << "]: B::key:" << B::b_key << ", b_value:" << b.b_value << ", b_static: " << B::b_static << endl;
    }

    return;
}

int main()
{
    /*example 1*/
    thread t11(thread_func_show_1, "t1");
    thread t12(thread_func_show_1, "t2");
    t11.join();
    t12.join();

    /*example 2*/
    thread t21(thread_func_show_2, "t1");
    thread t22(thread_func_show_2, "t2");
    t21.join();
    t22.join();


    /*example 3*/
    thread t31(thread_func_show_3, "t1");
    thread t32(thread_func_show_3, "t2");
    t31.join();
    t32.join();

    /*example 4*/
    thread t41(thread_func_show_4, "t1");
    thread t42(thread_func_show_4, "t2");
    t41.join();
    t42.join();

    return 0;
}

/**
example 1 output:
thread[t1]: x = 2
thread[t1]: y = 2
thread[t1]: x = 3
thread[t1]: y = 2
thread[t1]: x = 4
thread[t1]: y = 2
thread[t2]: x = 2
thread[t2]: y = 2
thread[t2]: x = 3
thread[t2]: y = 2
thread[t2]: x = 4
thread[t2]: y = 2


example 2 output:
create A
thread[t1]: a.counter:0
thread[t1]: a.counter:1
thread[t1]: a.counter:2
create A
thread[t2]: a.counter:0
thread[t2]: a.counter:1
thread[t2]: a.counter:2


example 3 output:
create A
thread[t1]: a.counter:0
create A
thread[t1]: a.counter:0
create A
thread[t1]: a.counter:0
create A
thread[t2]: a.counter:0
create A
thread[t2]: a.counter:0
create A
thread[t2]: a.counter:0


example 4 output:
create B
thread[t2]: b_key:11, b_value:23, b_static:35
thread[t2]: B::key:11, b_value:23, b_static: 35
thread[t2]: b_key:10, b_value:22, b_static:34
thread[t2]: B::key:10, b_value:22, b_static: 34
thread[t2]: b_key:9, b_value:21, b_static:33
thread[t2]: B::key:9, b_value:21, b_static: 33
create B
thread[t1]: b_key:11, b_value:23, b_static:32
thread[t1]: B::key:11, b_value:23, b_static: 32
thread[t1]: b_key:10, b_value:22, b_static:31
thread[t1]: B::key:10, b_value:22, b_static: 31
thread[t1]: b_key:9, b_value:21, b_static:30
thread[t1]: B::key:9, b_value:21, b_static: 30
**/
