#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>

using namespace std;

class Test
{
public:
    Test(){ cout << "construct " << endl;}
    Test(Test &&test) //移动构造函数
    {
        std::cout << "Move Constructor" << std::endl;
        m_p=test.m_p;
        test.m_p = nullptr; //修改参数的资源
    }
    Test(const Test& t) {
		cout << "Copy Constructor" << endl;
	}
    Test &operator=(Test &&test) //移动赋值操作符
    {
        std::cout << "Move Assignment operator" << std::endl;
        if (this != &test)
        {
            m_p = test.m_p;
            test.m_p= nullptr; //修改参数资源
        }
        return *this;
    }
    
private:
    int *m_p;
};

int main()
{
    std::vector<Test> vec;
    Test t1;
    vec.push_back(Test()); //移动构造函数
    Test foo = t1; //注意.....这里是拷贝构造函数...但是为什么？？？
    foo = Test();      //移动赋值操作符
    return 0;
}