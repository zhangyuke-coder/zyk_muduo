 #include <iostream>
 #include <thread>
 #include <sstream>
 using namespace std;
int main() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    int id = std::stoi(ss.str());
    cout << id;
 }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
