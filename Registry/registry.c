#include <Windows.h>
#include <stdio.h>
#include <winreg.h>

int main(void)
{
    //Local Variable
    HKEY hKey;
    HKEY hDestKey;
    LSTATUS lStatus;
    DWORD dwData = '$';
    unsigned char buffer[1024];
    DWORD type;
    DWORD dataLength = 1024;

    //Regsitry Create Key Function.
    lStatus = RegCreateKeyEx(
        HKEY_LOCAL_MACHINE,             //Registry Key will create under HKEY_LOCAL_MACHINE
        L"SOFTWARE\\aaaa\\aaa\\aa\\a",         //Path for Subkey
        0,                              //Reserved
        NULL,                           //Class of Key
        REG_OPTION_NON_VOLATILE,        //Registry Options
        KEY_ALL_ACCESS | KEY_WOW64_64KEY,        //Access Right
        NULL,                           //Default Security Attributes
        &hKey,                          //Pointer Variable which receives Registry Creations
        NULL                            //Disposition Information NULL
    );
    //Check the Registry Key Created or not.
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\tRegsitry Creation Failed ");
        printf("\n\tError No - %d\n\n", GetLastError());
    }
    else
        puts("\tRegistry Creation Success");

    //Registry Set Value Function.
    lStatus = RegSetValueEx(
        hKey,         //Handle for Open Key
        L"Vicky",     //Registry Value Name
        0,            //Reserved
        REG_DWORD,    //Data type of Regsitry Key
        (LPBYTE)&dwData,//Registry Data
        sizeof(dwData)  //Size of Registry Data
    );
    //Check the Value set or not
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\tRegsitry Set Value Failed ");
        printf("\n\tError No - %d\n\n", GetLastError());
    }
    else
        puts("\n\tRegsitry Set Value Success");

    //Open hKey registry key
    lStatus = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,     //Registry Key will create under HKEY_LOCAL_MACHINE
        L"SOFTWARE\\aaaa", //Path for Subkey
        0,                      //ulOptions
        KEY_ALL_ACCESS | KEY_WOW64_64KEY | KEY_EXECUTE | DELETE,        //Access Right
        &hKey               //Pointer Variable which receives Registry Opens
    );
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\thKey Regsitry Open Failed ");
        printf("\n\tError No - %d\n\n", GetLastError());
    }
    else
        puts("\n\thKey Registry Open Success");

    //Regsitry Create Key Function.
    lStatus = RegCreateKeyEx(
        HKEY_LOCAL_MACHINE,             //Registry Key will create under HKEY_LOCAL_MACHINE
        L"SOFTWARE\\bbbb",         //Path for Subkey
        0,                              //Reserved
        NULL,                           //Class of Key
        REG_OPTION_NON_VOLATILE,        //Registry Options
        KEY_ALL_ACCESS | KEY_WOW64_64KEY | DELETE,        //Access Right
        NULL,                           //Default Security Attributes
        &hDestKey,                          //Pointer Variable which receives Registry Creations
        NULL                            //Disposition Information NULL
    );
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\thDestKey Regsitry Creation Failed ");
        printf("\n\tError No - %d\n\n", GetLastError());
    }
    else
        puts("\n\thDestKey Registry Create Success");

    lStatus = RegCopyTree(
        hKey,
        NULL,
        hDestKey
    );
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\tCopy Registry key not done");
        printf("\t Error no : %d\n", GetLastError());
    }
    else
        puts("\n\tCopy Registry key Done");

    system("PAUSE");

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\aaaa\\aaa\\aa\\a", 0,
        KEY_ALL_ACCESS | KEY_WOW64_64KEY | KEY_EXECUTE | DELETE, &hKey);
    // Read Registry values
    lStatus = RegGetValue(
        hKey,
        NULL,
        L"Vicky",
        RRF_RT_ANY,
        &type,
        (PVOID)&buffer,
        &dataLength
    );
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\tValue not getted");
        printf("\n\tError no\t: %d", GetLastError());
    }
    else
        printf("\n\tValue\t: %s\n", buffer);


    system("PAUSE");

    //Delete subkeys and values recursively
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE", 0,
        KEY_ALL_ACCESS | KEY_WOW64_64KEY | KEY_EXECUTE | DELETE, &hKey);
    lStatus = RegDeleteTree(
        hKey,
        L"aaaa"
    );
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\thKey Regsitry Key delete Failed ");
        printf("\n\tError No - %d\n\n", GetLastError());
    }
    else
        puts("\n\thKey Regsitry Key deleted");

    system("PAUSE");
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE", 0,
        KEY_ALL_ACCESS | KEY_WOW64_64KEY | KEY_EXECUTE | DELETE, &hDestKey);
    lStatus = RegDeleteTree(
        hDestKey,
        L"bbbb"
    );
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\thDestKey Regsitry Key delete Failed ");
        printf("\n\tError No - %d\n\n", GetLastError());
    }
    else
        puts("\n\thDestKey Regsitry Key deleted");

    system("PAUSE");
    //Registry Close Key Function.
    lStatus = RegCloseKey(hKey);
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\thKey Registry key not closed");
        printf("\t Error no : %d\n", GetLastError());
    }
    else
        puts("\n\thKey Registry key closed");

    lStatus = RegCloseKey(hDestKey);
    if (lStatus != ERROR_SUCCESS)
    {
        puts("\n\thDestKey Registry key not closed");
        printf("\tError no : %d\n", GetLastError());
    }
    else
        puts("\n\thDestKey Registry key closed");

    return 0;
}