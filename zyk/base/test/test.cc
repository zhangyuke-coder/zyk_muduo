#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <stdio.h>
using namespace std;

class Test
{
public:
    Test(int n)
		: i(n)
		{ cout << "construst" << endl; };
    Test(Test &&test) //移动构造函数
    {
        std::cout << "Move Constructor" << std::endl;
        m_p=test.m_p;
        test.m_p = nullptr; //修改参数的资源
    }
	Test(const Test& t) {
		cout << "Copy Constructor" << endl;
	}
	Test &operator=(const Test &test) //移动赋值操作符
    {
        std::cout << "Assignment operator" << std::endl;
       
        return *this;
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
    ~Test() {
		cout << "destruct" << endl;
	}
	void f() {
		++i;
		cout << i << endl;
	}
private:
    int *m_p;
	int i; 
};

int main()
{
	FILE* fp = ::fopen("./test.txt", "rb");
	char buf[2];
	ssize_t nr = ::fgets(buf, 1, 2, fp);
	cout << buf << endl;

	
}