#include <stdio.h>

int main(void)
{
	char* filename = "C:\\Users\\Vignesh\\Desktop\\Incubation\\File Operations\\zoho.txt";
	FILE* fp;

	fp = fopen(filename, "w");
	fprintf(fp, "ZOHO INCUBATION\n");
	fclose(fp);

	fp = fopen(filename, "r");
	while (1)
	{
		char ch = fgetc(fp);
		if (ch == EOF)
			break;
		printf("%c", ch);
	}
	fclose(fp);

	fp = fopen(filename, "w");
	fprintf(fp, "My own content, Old content overwritten\n");
	fclose(fp);

	fp = fopen(filename, "a");
	fputs("Changed By Vicky", fp);
	fclose(fp);

	fp = fopen(filename, "r");
	while (1)
	{
		char ch = fgetc(fp);
		if (ch == EOF)
			break;
		printf("%c", ch);
	}
	fclose(fp);
}