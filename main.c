#include <stdio.h>

static int test1(int a)
{
	return a+1;
}

static int test2(int b)
{
	return b+1;
}

int main(int argc, char *argv[])
{
	printf("resultall=%d\n",test1(test2(2)));
	printf("result1=%d\n",test1(2));
	printf("result2=%d\n",test2(2));
	return 0;
}
