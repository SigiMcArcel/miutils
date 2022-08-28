#include <cstdio>
#include "Test.h"

int main()
{
    Test t;
    t.start(1000);
    while (true)
    {
        usleep(100000);
    }
    return 0;
}