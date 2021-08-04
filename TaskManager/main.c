#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <Psapi.h>
#pragma warning(disable : 4996)

#define SERVICENAME L"aaTaskManager"
#define SLEEP_TIME 10

char* filename = "C:\\Users\\Vignesh Kumar S\\Desktop\\VisualStudio\\TaskManager\\Text.txt";
FILE* fp;

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;
void  ServiceMain(int argc, char** argv);
void  ControlHandler(DWORD request);
int WriteToLog(char* str);
SC_HANDLE handleToServiceControlManagerDB(void);
SC_HANDLE openService(SC_HANDLE handleSCMDB);
void __stdcall createService(void);
void __stdcall startService(void);
void __stdcall stopService(void);
void __stdcall deleteService(void);
void showProcessInformation();

void showProcessInformation()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (hSnapshot) {
		FILE* log;
		fopen_s(&log, filename, "w+");
		if (log == NULL)
			return;
		PROCESSENTRY32 pe32;
		char buffer[9999] = "\0";
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnapshot, &pe32)) {
			do {
				HANDLE pHandle = NULL;
				TCHAR filename[MAX_PATH];
				pHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
				if (pHandle != NULL) {
					if (GetModuleFileNameEx(pHandle, NULL, filename, MAX_PATH) == 0) {
						fprintf(log, "\n\tFailed to get module filename\n\n");
					}
					else {
						sprintf_s(buffer, sizeof(buffer), "%-40ls|  %d  \t| Path : %ls \n\n", pe32.szExeFile, pe32.th32ProcessID, filename);
						fprintf(log, "%s", buffer);
					}
					CloseHandle(pHandle);
				}
				else {
					fprintf(log, "%-40ls|  %ls \n\n", L"Failed to open this process", pe32.szExeFile);
				}
			} while (Process32Next(hSnapshot, &pe32));
		}

		fclose(log);
		CloseHandle(hSnapshot);
	}
}

int WriteToLog(char* str)
{
	FILE* log;
	fopen_s(&log, filename, "w+");
	if (log == NULL)
		return -1;
	fprintf(log, "%s", str);
	fclose(log);
	return 0;
}

void ServiceMain(int argc, char** argv)
{
	static int count = 1;

	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	hStatus = RegisterServiceCtrlHandler(
		SERVICENAME,
		(LPHANDLER_FUNCTION)ControlHandler
	);
	if (hStatus == (SERVICE_STATUS_HANDLE)0)
	{
		WriteToLog("Registering Control Handler failed\n");
		return;
	}
	SetServiceStatus(hStatus, &ServiceStatus);

	// We report the running status to SCM. 
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &ServiceStatus);

	// The worker loop of a service
	while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		showProcessInformation();
		Sleep(SLEEP_TIME);
	}
	return;
}

void ControlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;

	default:
		break;
	}

	// Report current status
	SetServiceStatus(hStatus, &ServiceStatus);

	return;
}

SC_HANDLE handleToServiceControlManagerDB(void)
{
	SC_HANDLE handle = OpenSCManager(
		NULL, // local machine
		NULL, // SERVICES_ACTIVE_DATABASE database is opened by default
		SC_MANAGER_ALL_ACCESS
	);
	if (handle == NULL)
	{
		puts("\n\tOpenSCManager() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		return NULL;
	}
	else
	{
		puts("\n\tOpenSCManager() function success - Created handle to SCM DB");
		return handle;
	}
}

SC_HANDLE openService(SC_HANDLE handleToSCMDB)
{
	SC_HANDLE handleToService = OpenServiceA(
		handleToSCMDB,
		"aaTaskManager",
		SERVICE_ALL_ACCESS
	);
	if (handleToService == NULL)
	{
		puts("\n\tOpenServiceA() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		return NULL;
	}
	else
	{
		printf("\n\tOpenServiceA() function success - Created handle to given service\n");
		return handleToService;
	}
}

void __stdcall createService(void)
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	WCHAR szPath[MAX_PATH];

	if (!GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		wprintf(L"\n\tCannot install service, error %u\n", GetLastError());
		return;
	}
	else
		wprintf(L"\n\tService was installed successfully!\n");

	// Get a handle to the SCM database
	schSCManager = handleToServiceControlManagerDB();

	// Create the service
	schService = CreateService(
		schSCManager,              // SCM database
		SERVICENAME,                   // name of service
		SERVICENAME,                   // service name to display
		SERVICE_ALL_ACCESS,        // desired access
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type
		SERVICE_AUTO_START,      // start type
		SERVICE_ERROR_NORMAL,      // error control type
		szPath,                    // path to service's binary
		NULL,                      // no load ordering group
		NULL,                      // no tag identifier
		NULL,                      // no dependencies
		NULL,                      // LocalSystem account
		NULL);                     // no password

	if (schService == NULL)
	{
		wprintf(L"\n\tCreateService() failed, error %u\n", GetLastError());
		CloseServiceHandle(schSCManager);
		return;
	}
	else
		wprintf(L"\n\tCreateService() is pretty fine!\n");

	SERVICE_DESCRIPTION sd;
	sd.lpDescription = L"This is my own Task manager";
	if (!ChangeServiceConfig2(
		schService,                 // handle to service
		SERVICE_CONFIG_DESCRIPTION, // change: description
		&sd))                      // new description
	{
		wprintf(L"\n\tChangeServiceConfig2() failed, error %d\n", GetLastError());
	}
	else
		wprintf(L"\n\tService description updated successfully.\n");

	// Set the path of executable with "" in registry
	HKEY hKey;
	LSTATUS lStatus = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,     //Registry Key will open under HKEY_LOCAL_MACHINE
		L"SYSTEM\\CurrentControlSet\\Services\\aaTaskManager", //Path for Subkey
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
		puts("\n\thKey Registry Open Success - Set path with \"\"");

	char buffer[999];
	sprintf_s(buffer, sizeof(buffer), "\"%ls\"", szPath);
	int szBuffer = strlen(buffer);

	// Registry Set Value Function.
	lStatus = RegSetValueExA(
		hKey,         //Handle for Open Key
		(LPCSTR)"ImagePath",     //Registry Value Name
		0,            //Reserved
		REG_EXPAND_SZ,    //Data type of Regsitry Key
		&buffer,//Registry Data
		szBuffer  //Size of Registry Data
	);
	//Check the Value set or not
	if (lStatus != ERROR_SUCCESS)
	{
		puts("\n\tRegsitry Set Value Failed ");
		printf("\n\tError No - %d\n\n", GetLastError());
	}
	else
		puts("\n\tRegsitry Set Value Success");

	startService();
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
}

void __stdcall startService()
{
	//1. handle to the service control manager database
	SC_HANDLE handleToSCMDB = handleToServiceControlManagerDB();
	if (handleToSCMDB == NULL)
	{
		return;
	}

	//2. OpenService() returns handle to the service
	SC_HANDLE handleToService = openService(handleToSCMDB);
	if (handleToService == NULL)
	{
		return;
	}

	//3. StartService() function used to start existing services
	int startService = StartServiceA(
		handleToService,	//handle to the service
		0,					//Number of Arguments
		NULL				//No Arguments
	);
	if (startService == 0)
	{
		puts("\n\tStartService() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		return;
	}
	else
		puts("\n\tStartService() function success - Particular service running");
	puts("");
	CloseServiceHandle(handleToSCMDB);
	CloseServiceHandle(handleToService);
}

void __stdcall stopService()
{
	// QueryServiceStatusEx() function tells status of the service

	//1. handle to the service control manager database
	SC_HANDLE handleToSCMDB = handleToServiceControlManagerDB();
	if (handleToSCMDB == NULL)
	{
		return;
	}

	//2. OpenService() returns handle to the service
	SC_HANDLE handleToService = openService(handleToSCMDB);
	if (handleToService == NULL)
	{
		return;
	}

	SERVICE_STATUS_PROCESS  serviceStatusProcess;
	DWORD dwBytesNeeded;
	int serviceStatus = QueryServiceStatusEx(
		handleToService,
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&serviceStatusProcess,
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded
	);
	if (serviceStatus == 0)
	{
		puts("\n\tQueryServiceStatusEx() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		return;
	}
	else
	{
		puts("\n\tQueryServiceStatusEx() function success - Able to get the status of given service handle");
		if (serviceStatusProcess.dwCurrentState == SERVICE_STOP || serviceStatusProcess.dwCurrentState == SERVICE_STOPPED)
		{
			puts("\n\tService Already Stopped");
		}
		else if (serviceStatusProcess.dwCurrentState == SERVICE_STOP_PENDING)
		{
			puts("\n\tService Stop Pending!!!!!");
			return;
		}
		else if (serviceStatusProcess.dwCurrentState == SERVICE_RUNNING)
		{
			puts("\n\tService Running");

			//5. Stop the service using ControlService() function
			int controlService = ControlService(
				handleToService,
				SERVICE_CONTROL_STOP,
				(LPSERVICE_STATUS)&serviceStatusProcess
			);
			if (controlService == 0)
			{
				puts("\n\tControlService() function failed");
				printf("\n\tError No\t: %d\n", GetLastError());
				return;
			}
			else
				puts("\n\tControlService() function success - I used this to STOP given service handle");

			serviceStatus = QueryServiceStatusEx(
				handleToService,
				SC_STATUS_PROCESS_INFO,
				(LPBYTE)&serviceStatusProcess,
				sizeof(SERVICE_STATUS_PROCESS),
				&dwBytesNeeded
			);
			if (serviceStatus == 0)
			{
				puts("\n\tQueryServiceStatusEx() function failed");
				printf("\n\tError No\t: %d\n", GetLastError());
				return;
			}
			else
			{
				puts("\n\tQueryServiceStatusEx() function success - To check status of given service handle");
				if (serviceStatusProcess.dwCurrentState == SERVICE_STOP_PENDING)
				{
					puts("\n\tService Stop Pending, Wait for more than 2 seconds\n");
					system("PAUSE");
					if (QueryServiceStatusEx(handleToService, SC_STATUS_PROCESS_INFO, (LPBYTE)&serviceStatusProcess,
						sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
					{
						puts("\n\tQueryServiceStatusEx() function failed");
						printf("\n\tError No\t: %d\n", GetLastError());
						return;
					}
					else
					{
						puts("\n\tQueryServiceStatusEx() function success");
						if (serviceStatusProcess.dwCurrentState == SERVICE_STOP ||
							serviceStatusProcess.dwCurrentState == SERVICE_STOPPED)
						{
							puts("\n\tService Stopped");
						}
					}
				}
				else if (serviceStatusProcess.dwCurrentState == SERVICE_STOP || serviceStatusProcess.dwCurrentState == SERVICE_STOPPED)
				{
					puts("\n\tService Stopped");
				}
				else
				{
					puts("\n\tService Not Stopped");
					return;
				}
			}
		}
		else
		{
			puts("\n\tService Not Stopped\n");
			return;
		}
	}
	CloseServiceHandle(handleToSCMDB);
	CloseServiceHandle(handleToService);
}

void __stdcall deleteService(void)
{
	// Delete the particular service

	//1. handle to the service control manager database
	SC_HANDLE handleToSCMDB = handleToServiceControlManagerDB();
	if (handleToSCMDB == NULL)
	{
		return;
	}

	//2. OpenService() returns handle to the service
	SC_HANDLE handleToService = openService(handleToSCMDB);
	if (handleToService == NULL)
	{
		return;
	}
	if (!DeleteService(handleToService) && handleToService != NULL)
	{
		puts("\n\tDeleteService() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
	}
	else
		puts("\n\tService deleted successfully\n");

	CloseServiceHandle(handleToSCMDB);
	CloseServiceHandle(handleToService);
}


int wmain(int argc, WCHAR* argv[])
{
	if (lstrcmpi(argv[1], L"Start") == 0)
	{
		puts(" _______________________________________________ ");
		puts("|\tSTART EXISTING SERVICES\t\t\t|");
		puts("|_______________________________________________|");
		startService();
	}
	else if (lstrcmpi(argv[1], L"Stop") == 0)
	{
		puts(" _______________________________________________ ");
		puts("|\tSTOP EXISTING SERVICES\t\t\t|");
		puts("|_______________________________________________|");
		stopService();
	}
	else if (lstrcmpi(argv[1], L"Create") == 0)
	{
		puts("\n===================================================================================\n\n");
		puts(" _______________________________________________________ ");
		puts("|\tCREATING THE SERVICE USING .exe FILE\t\t|");
		puts("|_______________________________________________________|");
		// CREATING THE SERVICE USING EXISTING .exe FILE
		puts("");
		createService();
	}
	else if (lstrcmpi(argv[1], L"Delete") == 0)
	{
		puts("\n===================================================================================\n\n");
		puts(" _______________________________________ ");
		puts("|\tDELETING THE SERVICE\t\t|");
		puts("|_______________________________________|");
		// CREATING THE SERVICE USING EXISTING .exe FILE
		puts("");
		deleteService();
		return 0;
	}

	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		{ SERVICENAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	StartServiceCtrlDispatcher(DispatchTable);
	return 0;
}