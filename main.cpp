#include <iostream>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int jail(void *args) {
	printf("Hello!! (child) \n");
	return EXIT_SUCCESS;
}

/*  Stack_Memory 
	We are going to provide some arbitrary memory size 65K bytes, 
	after we allocate this memory we are going to return a pointer 
	to the end of the array. This is required because when clone 
	load this process the stack (the memory the process need to run) grows backward.
*/
char* stack_memory() {
	const int stackSize = 65536;
	auto *stack = new (nothrow) char[stackSize];

	if (stack == nullptr) {
		printf("Cannot allocate memory \n");
		exit(EXIT_FAILURE);
	}

	return stack + stackSize;
}

int main(int argc, char** argv) {
	printf("Hello, World! (parent) \n");

	/*
	The first parameter is our entry point function, 
	second parameter is our function to allocate memory, 
	third (SIGCHLD) this flag tells the process to emit a signal when finished 
	and the fourth and last one is only necessary if we want to pass arguments 
	to the jail function, in this case we pass just 0
	*/
	clone(jail, stack_memory(), SIGCHLD, 0);

	return EXIT_SUCCESS;
}