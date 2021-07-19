#include <stdio.h>
#include <wchar.h>
#include <uchar.h>
#include <locale.h>

int main()
{
	setlocale(LC_ALL, "en_US.UTF-8");

	puts("\n\tASCII Character\n");
	char c1 = 'A';	// or char8_t c1 = 'A'; (But it is not working)
	printf("\tCharacter is\t: %c\n\n", c1);
	printf("\tSize\t: %lu\n\n", sizeof(c1));
	puts("___________________________________________________");

	wchar_t c2 = L'A';
	puts("\n\tUNICODE character - Wide character\n");
	wprintf(L"\tWide character\t: %lc\n\n", c2);
	wprintf(L"\tSize\t: %lu\n\n", sizeof(c2));
	puts("___________________________________________________");

	char16_t c3 = U'A';
	puts("\n\tUNICODE character - 16 bit character\n");
	printf("\t16 bit character\t: %lc\n\n", c3);
	printf("\tSize\t: %lu\n\n", sizeof(c3));
	puts("___________________________________________________");

	char32_t c4 = 0x0583;
	puts("\n\tUNICODE character - 32 bit character\n");
	printf("\t32 bit character\t: %lc\n\n", c4);
	printf("\tSize\t: %lu\n\n", sizeof(c4));
	puts("___________________________________________________");


	// c1 is a single byte character
	c1 = 120;
	puts("\n\tConverting Single byte to Multi - byte character\n");
	printf("\tBefore Conversion, Character is\t: %c\n\n", c1);
	printf("\tSizeof single byte character\t: %lu\n\n", sizeof(c1));
	// c4 is a char32_t type
	c4 = btowc(c1);
	puts("\tConverted\n");
	printf("\tAfter Conversion, Character is\t: %lc\n\n", c4);
	printf("\tSizeof multi byte character\t: %lu\n\n", sizeof(c4));
	puts("___________________________________________________");

	// c4 is a char32_t type
	c4 = 0x057B;
	puts("\n\tConverting Multi byte to single byte character\n");
	printf("\tBefore Conversion, Character is\t: %lc\n\n", c4);
	printf("\tSizeof multi byte character\t: %lu\n\n", sizeof(c4));
	puts("\tConverted\n");
	// c1 is char type
	c1 = (c4) << 24 >> 24;
	printf("\tAfter Conversion, Character is\t: %c\n\n", c1);
	printf("\tSizeof single byte character\t: %lu\n\n", sizeof(c1));
	puts("___________________________________________________");

	puts("\n\tWide Character Arrays");
	wchar_t arr[5] = { L'V',L'i',L'c',L'k',L'y' };
	printf("\n\tArray Elements\t: ");
	for (int i = 0; i < 5; i++)
	{
		wprintf(L"%c ", arr[i]);
	}
	wprintf(L"\n\tSize of array\t: %lu\n\n", sizeof(arr));
	puts("___________________________________________________");

	puts("\n\tWide character string\n");
	wchar_t str[] = L"Vicky";
	printf("\tString\t: %ls\n\n", str);

	/*
	for (unsigned int i = 0; i < wcslen(str); i++)
	{
		wprintf(L"%c", str[i]);
	}
	*/

	wprintf(L"\tSize\t: %lu\n\n", sizeof(str));
	puts("___________________________________________________");


	c4 = 195060;
	printf("\n\tInteger Value\t: %d\n", c4);
	printf("\n\tCharacter\t: %lc\n\n", c4);
	puts("___________________________________________________");

	return 0;
}

