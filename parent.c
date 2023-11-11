#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{

	printf("pid: %d\n", getpid());
	execve("process", NULL, NULL);
	return (0);
}
