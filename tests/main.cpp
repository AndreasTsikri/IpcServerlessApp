#include <iostream>
// uncomment to disable assert()
// #define NDEBUG
#include <cassert>
 
// Use (void) to silence unused warnings.
#define assertm(exp, msg) assert((void(msg), exp))
 
int main()
{
    assert(2 - 2 == 4);
    std::cout << "Checkpoint #1\n";
};
