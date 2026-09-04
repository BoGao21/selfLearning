/**
1、继承基础
(1)访问权限铁律：private 仅本类可见；protected 本类+子类可见；public 完全公开
子类永远无法直接访问父类的 private 成员（物理上继承了内存空间，但语法上禁止直接调用）。子类只能通过父类提供的 public 或 protected 成员函数间接操作父类的私有数据。

(2)继承方式的过滤本质：
公有继承：基类权限不变（public→public，protected→protected）。
保护继承：基类非私有成员降级为 protected（对外部隐藏，但对孙子类仍可见）。
私有继承：基类非私有成员降级为 private（彻底阻断，连孙子类都不可见）。
区分保护继承和私有继承的关键就看孙子类能否继续访问。


2、菱形继承问题
(1)当一个派生类（如Bat）同时继承自两个中间类（如Mammal、Bird），而这两个中间类又继承自同一个基类（如Animal）时，继承关系图呈现菱形结构。

(2)引发的两大“致命问题”：
数据冗余：Bat对象中会包含两份Animal的数据副本，浪费内存。
访问二义性：访问Animal成员时，编译器不知道到底访问哪一份拷贝，直接编译报错。

(3)解决方案：虚继承
解决方案：在中间层（Mammal和Bird）继承Animal时，使用 virtual 关键字，中层使用 virtual public 继承，最底层派生类直接负责初始化最顶层虚基类。

（4）虚继承下构造/析构绝对顺序：
构造：虚基类 → 普通基类（按声明顺序，编译器会屏蔽普通基类对其基类的构造）→ 成员对象 → 本类函数体。
析构：完全逆序（本类函数体 → 成员对象 → 普通基类（逆序）→ 虚基类）。

**/

#include <iostream>
using namespace std;

// 一、验证访问控制与继承过滤

// 基类：包含三种权限的成员
class Base {
public:
    int pub = 1;
protected:
    int prot = 2;
private:
    int priv = 3;   // 子类无法直接访问，但物理上被继承
public:
    int getPriv() const { return priv; } // 提供公有接口供子类间接访问
};

// 公有继承
class PubDerived : public Base {
public:
    void test() {
        cout << "PubDerived 能访问: pub=" << pub << ", prot=" << prot;
        cout << ", priv(通过接口)=" << getPriv() << endl; // 间接访问私有
        // cout << priv; // 编译报错：private 不可直接访问
    }
};

// 保护继承：pub 和 prot 在子类中变为 protected
class ProtDerived : protected Base {
public:
    void test() {
        cout << "ProtDerived 能访问: pub=" << pub << ", prot=" << prot << endl;
    }
};

// 私有继承：pub 和 prot 在子类中变为 private
class PrivDerived : private Base {
public:
    void test() {
        cout << "PrivDerived 能访问: pub=" << pub << ", prot=" << prot << endl;
    }
};

// 孙子类验证（区分保护继承和私有继承的关键）
// 继承自公有继承类 -> 基类权限保留，孙子类依然可见
class GrandPub : public PubDerived {
public:
    void test() {
        cout << "GrandPub 能访问: pub=" << pub << ", prot=" << prot << endl; //  可见
    }
};

// 继承自保护继承类 -> 基类成员在父类中为 protected，孙子类依然可见
class GrandProt : public ProtDerived {
public:
    void test() {
        cout << "GrandProt 能访问: pub=" << pub << ", prot=" << prot << endl; //  可见
    }
};

// 继承自私有继承类 -> 基类成员在父类中为 private，孙子类彻底不可见
class GrandPriv : public PrivDerived {
public:
    void test() {
        // cout << pub;   // 编译报错：pub 在 PrivDerived 中已是 private，孙子类不可访问
        // cout << prot;  // 编译报错：同上
        cout << "GrandPriv 无法访问 Base 的任何成员（已被私有继承阻断）" << endl;
    }
};

// 二、验证菱形继承（虚继承）+ 构造析构顺序

class Animal {
public:
    int age;
    Animal(int a) : age(a) {
        cout << "  [Animal] 构造函数 (age=" << age << ")" << endl;
    }
    virtual ~Animal() {
        cout << "  [Animal] 析构函数" << endl;
    }
};

// 中层使用虚继承
class Mammal : virtual public Animal {
private:
    int furType; // 自己的私有数据，正常初始化
public:
    // 注意：这里的 Animal(a) 在虚继承下会被最底层派生类忽略
    Mammal(int a, int f) : Animal(a), furType(f) {
        cout << "  [Mammal] 构造函数 (furType=" << furType << ")" << endl;
    }
    virtual ~Mammal() {
        cout << "  [Mammal] 析构函数" << endl;
    }
};

class Bird : virtual public Animal {
private:
    int wingSpan;
public:
    Bird(int a, int w) : Animal(a), wingSpan(w) {
        cout << "  [Bird] 构造函数 (wingSpan=" << wingSpan << ")" << endl;
    }
    virtual ~Bird() {
        cout << "  [Bird] 析构函数" << endl;
    }
};

// 最底层派生类
class Bat : public Mammal, public Bird {
public:
    // 最底层直接负责初始化虚基类 Animal；中间层对 Animal 的构造调用被自动忽略
    Bat(int a, int f, int w)
        : Animal(a),      // 有效，Bat 负责构造唯一的一份 Animal
          Mammal(a, f),   // 正常传参，但其内部的 Animal(a) 被忽略
          Bird(a, w)      // 正常传参，但其内部的 Animal(a) 被忽略
    {
        cout << "  [Bat] 构造函数 (本类函数体)" << endl;
    }

    void show() {
        // 访问 age 无歧义，因为只有一份 Animal 数据
        cout << "Bat 访问 age: " << age << " (唯一份虚基类数据)" << endl;
    }

    virtual ~Bat() {
        cout << "  [Bat] 析构函数 (本类函数体)" << endl;
    }
};


int main() {
    PubDerived p1;
    ProtDerived p2;
    PrivDerived p3;
    GrandPub g1;
    GrandProt g2;
    GrandPriv g3;

    p1.test();
    p2.test();
    p3.test();
    g1.test();
    g2.test();
    g3.test();

    cout << "\n========== 类外部（对象）访问验证 ==========" << endl;
    cout << "PubDerived 对象访问 pub: " << p1.pub << " (公有继承保留 public)" << endl;
    // cout << p2.pub; // 编译报错：保护继承使 pub 变为 protected，外部不可访问
    // cout << p3.pub; // 编译报错：私有继承使 pub 变为 private，外部不可访问

    cout << "\n========== 菱形继承（虚继承）构造/析构顺序 ==========" << endl;
    cout << ">>> 创建 Bat 对象（观察构造顺序） <<<" << endl;
    {
        Bat bat(10, 3, 45);
        bat.show();
        cout << ">>> Bat 对象即将离开作用域（观察析构顺序） <<<" << endl;
    } // 析构在此发生

    return 0;
}

/**
运行结果及分析：
第一部分：验证继承过滤（类内部访问）
PubDerived 能访问: pub=1, prot=2, priv(通过接口)=3
ProtDerived 能访问: pub=1, prot=2
PrivDerived 能访问: pub=1, prot=2
GrandPub 能访问: pub=1, prot=2
GrandProt 能访问: pub=1, prot=2
GrandPriv 无法访问 Base 的任何成员（已被私有继承阻断）

========== 类外部（对象）访问验证 ==========
PubDerived 对象访问 pub: 1 (公有继承保留 public)

========== 第二部分：菱形继承（虚继承）构造/析构顺序 ==========
>>> 创建 Bat 对象（观察构造顺序） <<<
  [Animal] 构造函数 (age=10)      // 虚基类最先构造
  [Mammal] 构造函数 (furType=3)   // 普通基类（按声明顺序）
  [Bird] 构造函数 (wingSpan=45)   // 普通基类
  [Bat] 构造函数 (本类函数体)      // 最后执行本类函数体
Bat 访问 age: 10 (唯一份虚基类数据)
>>> Bat 对象即将离开作用域（观察析构顺序） <<<
  [Bat] 析构函数 (本类函数体)      // 本类先析构
  [Bird] 析构函数                 // 逆序析构普通基类（Bird 先于 Mammal）
  [Mammal] 析构函数               // 虚基类最后析构
  [Animal] 析构函数               // 虚基类最后析构
**/
