#include <iostream>
#include <thread>
using namespace std;
int main() {
    cout << typeid(std::this_thread::get_id).name();

}