#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

}

// 第三题，第十题

int question1()
{
	int pid = fork();

	if (pid > 0) {
		int pid2 = fork();
		if (pid2 > 0) {
			// 父进程
			printf("I am parent. pid: %d, ppid: %d\n", getpid(), getppid());
		}
		else if (!pid2) {
			// 子进程2
			printf("I am child2. pid: %d, ppid: %d\n", getpid(), getppid());
		}
		else {
			printf("Fork failed.\n");
		}
	} 
	else if (!pid) {
		// 子进程1
		printf("I am child1. pid: %d, ppid: %d\n", getpid(), getppid());
	}
	else {
		printf("Fork failed.\n");
	}

	return 0;
}

int question2()
{
	system("ps  -af");
	int pid = fork();
	
	if (pid > 0) {
		// 父进程
		system("ps  -af");
	}
	else if (!pid) {
		// 子进程
		system("ps  -af");
	}
	else {
		printf("Fork failed.\n");
	}
	return 0;
}