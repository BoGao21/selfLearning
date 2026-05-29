/**
使用场景：当一个变量只有几种可能的值，然后通过数值来区分，使用枚举
**/

#include <iostream>

enum color
{
  red,     //枚举值中间逗号分隔
  blue,    //枚举值本质是标识符，这里不能是字符串，浮点型，布尔，整型，_1这种标识符可以
  green=87,
  black,
  white
}; //枚举声明大括号 + 分号


int main()
{
    color a = black;  //枚举类型名可直接用enum后自定义的color
    std::cout << a << std::endl;
    enum color b = blue;  //枚举类型名也可以加上enum+自定义的color
    std::cout << b << std::endl;

    color c = (color)9999;   // 枚举本质是整形，强转赋值未定义范围的数值不会报错，可正常编译运行
    std::cout << c << std::endl;

    //color d = yellow;  // 但是赋值未定义的枚举值则会报错
    std::cout << "size of int is " << sizeof(int) << std::endl;
    std::cout << "size of enum is " << sizeof(a) << std::endl;  //枚举和int占用相同的空间大小

    return 0;
}

/**
执行结果：
88
1
9999
size of int is 4
size of enum is 4
**/
