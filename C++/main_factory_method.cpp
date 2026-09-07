/**
1. 简单工厂模式（Simple Factory）—— 不属于GoF 23种，但常用
（1）定义：一个工厂类，根据传入的参数决定创建哪一种具体产品。适用于产品种类少且不常变化的场景。

（2）要点
一个工厂类，包含创建逻辑（通常用if-else或switch）。
一个抽象产品接口，多个具体产品类。
工厂方法通常设为static。


**/

/**
简单工厂模式code

优点：客户端只需传参数，无需关心类名；可通过配置文件+反射/注册表动态扩展。
致命缺点：违反开闭原则（OCP）—— 新增Triangle必须修改ShapeFactory，牵一发而动全身。
**/
#include <iostream>
#include <memory>   // std::unique_ptr
#include <string>
using namespace std;

// 抽象产品
class Shape {
public:
    virtual void draw() const = 0; // const表示只读操作
    virtual ~Shape() = default;
};

// 具体产品
class Circle : public Shape {
public:
    void draw() const override { cout << "⚪ Draw a Circle." << endl; }
};

class Square : public Shape {
public:
    void draw() const override { cout << "⬜ Draw a Square." << endl; }
};

// 简单工厂（唯一变化的地方）
class ShapeFactory {
public:
    // 返回智能指针，自动管理内存
    static unique_ptr<Shape> createShape(const string& type) {
        if (type == "circle") return make_unique<Circle>();
        if (type == "square") return make_unique<Square>();
        return nullptr;
    }
};

// 客户端
int main() {
    auto shape1 = ShapeFactory::createShape("circle");
    auto shape2 = ShapeFactory::createShape("square");
    if (shape1) shape1->draw();
    if (shape2) shape2->draw();
    // 无需 delete，自动释放
    return 0;
}


/**
2. 工厂方法模式（Factory Method）—— 解决扩展难题
（1）定义：定义一个用于创建对象的抽象接口，但让子类决定实例化哪一个具体类。将实例化操作延迟到子类中。
（2）核心变化：把“工厂”从具体类变成抽象接口，每个产品对应一个工厂子类。
（3）为什么需要它？（场景）：
    假设你正在开发一个跨平台UI框架：一个Dialog（对话框）基类，不同操作系统（Windows / Mac）需要渲染不同的Button（按钮）;
    创建按钮的逻辑必须由操作系统子类自己决定，而不是写在父类里。
**/

/**
工厂方法模式 code
体现“子类决策”

优点：新增LinuxButton只需加LinuxDialog，完全不用改已有代码，完美符合开闭原则。
缺点：每新增一个产品，就要新增一个工厂子类，类数量急剧膨胀。
**/
#include <iostream>
#include <memory>
using namespace std;

// -------- 产品层次 --------
class Button {
public:
    virtual void render() const = 0;
    virtual ~Button() = default;
};

class WindowsButton : public Button {
public:
    void render() const override { cout << "[Windows] 渲染经典灰色按钮." << endl; }
};

class MacButton : public Button {
public:
    void render() const override { cout << "[MacOS] 渲染圆角磨砂按钮." << endl; }
};

// -------- 工厂层次（抽象+具体） --------
class Dialog {
public:
    // 核心工厂方法：子类必须重写
    virtual unique_ptr<Button> createButton() = 0;
    virtual ~Dialog() = default;

    // 业务逻辑依赖工厂方法（模板方法模式）
    void renderWindow() const {
        auto btn = createButton(); // 多态调用，具体创建哪个由子类决定
        btn->render();
        cout << "窗口渲染完毕." << endl;
    }
};

class WindowsDialog : public Dialog {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<WindowsButton>();
    }
};

class MacDialog : public Dialog {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<MacButton>();
    }
};

// 客户端根据配置选择子类
int main() {
    unique_ptr<Dialog> dialog;
    
    // 模拟配置文件读取
    string os = "Mac"; 
    if (os == "Windows") dialog = make_unique<WindowsDialog>();
    else dialog = make_unique<MacDialog>();

    dialog->renderWindow(); // 输出对应风格的按钮
    return 0;
}


/**
3. 抽象工厂模式（Abstract Factory）—— 创建产品族
（1）定义：提供一个接口，用于创建一系列相关或相互依赖的对象（产品族），而无需指定具体类。
（2）关键区别：工厂方法处理一个产品等级结构（如只造按钮），抽象工厂处理多个产品等级结构（如既造按钮、又造复选框），且它们必须属于同一风格（产品族）。
（3）典型场景
     更换整个系统的UI风格（例如从“深色模式”切换到“浅色模式”），需要同时替换所有UI控件。不能出现“浅色按钮+深色对话框”这种混搭。
**/

/**
抽象工厂模式code

优点：保证产品族的一致性；新增产品族（例如“高对比度风格”）非常方便，无需修改客户端。
缺点：新增产品等级结构（即新增一种控件类型，如Slider）极其困难，需要修改抽象工厂接口和所有具体工厂，违反开闭原则。
**/
#include <iostream>
#include <memory>
using namespace std;

// -------- 产品等级1：按钮 --------
class Button {
public:
    virtual void paint() const = 0;
    virtual ~Button() = default;
};

class DarkButton : public Button {
public:
    void paint() const override { cout << "🎨 绘制深色按钮" << endl; }
};

class LightButton : public Button {
public:
    void paint() const override { cout << "🎨 绘制浅色按钮" << endl; }
};

// -------- 产品等级2：复选框 --------
class Checkbox {
public:
    virtual void check() const = 0;
    virtual ~Checkbox() = default;
};

class DarkCheckbox : public Checkbox {
public:
    void check() const override { cout << "✅ 深色复选框被选中" << endl; }
};

class LightCheckbox : public Checkbox {
public:
    void check() const override { cout << "✅ 浅色复选框被选中" << endl; }
};

// -------- 抽象工厂（定义产品族） --------
class GUIFactory {
public:
    virtual unique_ptr<Button> createButton() = 0;
    virtual unique_ptr<Checkbox> createCheckbox() = 0;
    virtual ~GUIFactory() = default;
};

// -------- 具体工厂：深色风格（产品族A） --------
class DarkThemeFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<DarkButton>();
    }
    unique_ptr<Checkbox> createCheckbox() override {
        return make_unique<DarkCheckbox>();
    }
};

// -------- 具体工厂：浅色风格（产品族B） --------
class LightThemeFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<LightButton>();
    }
    unique_ptr<Checkbox> createCheckbox() override {
        return make_unique<LightCheckbox>();
    }
};

// 客户端使用
class Application {
private:
    unique_ptr<Button> btn;
    unique_ptr<Checkbox> cb;
public:
    Application(unique_ptr<GUIFactory> factory) {
        btn = factory->createButton();
        cb = factory->createCheckbox();
    }
    void render() {
        btn->paint();
        cb->check();
    }
};

int main() {
    // 只需要切换工厂，整套UI风格全部替换
    auto factory = make_unique<LightThemeFactory>(); 
    Application app(move(factory));
    app.render();

    // 如果需要换深色，只改这一行即可
    auto darkFactory = make_unique<DarkThemeFactory>();
    Application darkApp(move(darkFactory));
    darkApp.render();
    return 0;
}

/**
三大模式对比总结

维度	         简单工厂	            工厂方法	                   抽象工厂
关注点	         创建单一种类产品	    创建单一种类产品	           创建一组相关（多种类）产品
扩展方式	       修改工厂类（不灵活）	  增加工厂子类（灵活）	       增加工厂子类（灵活）
产品变化维度	   只横向扩展（新产品）	  只横向扩展（新产品）	       横向扩展（新产品） + 纵向扩展（新风格）
违背开闭原则？	 违背（修改已有代码）	  符合（新增子类）	           对新增产品族符合，对新增产品种类违背
典型应用	       解析配置文件生成对象	  日志记录器、跨平台UI单控件	 整套UI框架、数据库连接池切换

**/
