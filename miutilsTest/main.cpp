#include <cstdio>
#include "Test.h"

int main()
{
    Test t1("test1**");
    Test t2("test2*******");
    Test t3("test3***************");
    Test t4("test4*************************");

  //  t1.start(5);
  ///  t2.start(20);
  //  t3.start(200);
    t4.start(1000);

    while (true)
    {
        usleep(100000);
    }
    return 0;
}