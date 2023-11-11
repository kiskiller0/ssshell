#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	printf("my pid: %d\n", getpid());
	return (0);
}
