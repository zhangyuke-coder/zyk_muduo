#include "../noncopyable.h"
#include <string>
#include <iostream>


class obj1 : zyk::noncopyable {



public:
    
    std::string name;
    
};


class obj2 : zyk::noncopyable{
public:

    obj2(const std::string& s) :
            name(s){
                std::cout << "std::string&" << std::endl;
            }
    obj2(std::string&& s) :
            name(s) {
                std::cout << "std::string&&" << std::endl;
            }
    std::string name;

};

int main() {
    std::string s = "asdf";
    obj2 p1(std::move(s));
    obj2 p2(p1);
    std::cout << p2.name << std::endl;
}