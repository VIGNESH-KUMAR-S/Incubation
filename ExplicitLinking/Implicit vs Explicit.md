|Implicit linking|Explicit linking|
|---|---|
|**Load-Time Dynamic Linking**|**Run-Time Dynamic Linking**|
|**Static Load**|**Dynamic Load**|
|By using #include "[HEADER FILE]"|By using on demand call|
|At loader, there is no work for Implicit linking|At loader, the demand function is called explicitly|
|At Linker, All the references of object files of DLL is appended one by one in executable code|At Linker, it just checks the usage of variables,functions,etc,. with particular declarations|
|If Some function deleted in DLL then it gets an error|If some function deleted still we are able to process the code|
|Execution time is fast, because there is no loading and un-loading|Execution time is less, because of load and un-load|
|DLL and Main files are physically linked with reference in Main executable file|No linkage between DLL and Main executable file|
|Recompilation of main program not required|Here also|
|DLL will be freed only when application ends|It's possible to freed the DLL after we using it|
|Before running our application our DLL and application must be link|When main application wants to call our DLL, that time only it link and serve the request|

### REFERENCE
[Explicit Linking](http://awinsyspro.com/45-explicit-linking-of-dll-windows/)
