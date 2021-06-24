#include <stdio.h>
#include <Windows.h>
#include "[PATH OF HEADER FILE]"

int main(void) {
	int a = 3, b = 5;
	printf("add : %d\n\n", add(a, b));
	system("pause");
	printf("mul : %d\n\n", mul(a, b));
}