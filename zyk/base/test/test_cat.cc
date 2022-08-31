#include <iostream>
using namespace std;

int main() {


    int num = 0x00636261;//用16进制表示32位int，0x61是字符'a'的ASCII码
    cout << num << endl;
	int * pnum = &num;
	char * pstr = static_cast<char *>(pnum);
	cout<<"pnum指针的值: "<<pnum<<endl;
	cout<<"pstr指针的值: "<<static_cast<void *>(pstr)<<endl;//直接输出pstr会输出其指向的字符串，这里的类型转换是为了保证输出pstr的值
	cout<<"pnum指向的内容: "<<hex<<*pnum<<endl;
	cout<<"pstr指向的内容: "<<pstr<<endl;
	return 0;
}