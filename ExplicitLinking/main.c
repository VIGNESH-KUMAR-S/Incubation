#include <stdio.h>
#include <Windows.h>

//Declare Function Pointer
typedef int(__cdecl* FunAdd)(int a, int b);
typedef int(__cdecl* FunMul)(int a, int b);

int main() {

	HMODULE hModule;

	//create a handle to load library
	hModule = LoadLibrary(TEXT("C:\\Users\\Vignesh\\Desktop\\VisualStudio\\explicitLinking\\DynamicLibrary\\Debug\\DynamicLibrary.dll"));

	if (NULL == hModule) {
		printf("Load Library Failed\n\n");
		printf("Error No - %d\n\n", GetLastError());
	}
	else {
		printf("Load Library Success\n\n");
	}

	//STEP-2 GetProcAddress - Get The Function Address

	FunAdd AdditionFun = hModule ? (FunAdd)GetProcAddress(hModule, "add") : NULL;
	FunMul MultiplyFun = hModule ? (FunMul)GetProcAddress(hModule, "mul") : NULL;


	//STEP-3 Check the Function Address is Valid or Not
	if (NULL == AdditionFun)
	{
		printf("Addition Function Address is Not Valid\n\n");
		printf("Error No - %d\n", GetLastError());
	}
	else {
		printf("Addition Function Address is Valid\n\n");
		printf("Addition - %d\n", AdditionFun(10, 10));
	}

	if (NULL == MultiplyFun)
	{
		printf("Multiplication Function Address is Not Valid\n\n");
		printf("Error No - %d\n", GetLastError());
	}
	else {
		printf("Multiplication Function Address is Valid\n\n");
		printf("Multiply - %d\n", MultiplyFun(10, 10));
	}


	//STEP-4 Free the DLL Module
	if (NULL != hModule && 0 != hModule) {
		FreeLibrary(hModule);
	}

	// system("PAUSE");

	return 0;
}