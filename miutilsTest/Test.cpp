#include "Test.h"
#include <cstdio>
void Test::eventOccured(void* sender, const std::string& name)
{
	printf("Hallo aus %s!\n", "miutilsTest");
}
