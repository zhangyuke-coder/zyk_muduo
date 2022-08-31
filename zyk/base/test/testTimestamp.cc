#include "../Timestamp.h"
#include <iostream>
using namespace std;
using namespace zyk;

int main() {

    cout << Timestamp::now().toFormattedString() << endl;
    cout << Timestamp::now().toString() << endl;
    cout << Timestamp::now().secondsSinceEpoch() << endl;



    return 0;
}