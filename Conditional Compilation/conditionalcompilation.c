#include <stdio.h>
#define ZOHO


int main()
{
    #ifdef ZOHO
        printf("ZOHO is defined using preprocessing directive");
    #else
        printf("ZOHO is not defined using preprocessing directive");
    #endif

    return 0;
}
