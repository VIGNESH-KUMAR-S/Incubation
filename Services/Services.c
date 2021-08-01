#include <windows.h>
#include <stdio.h>

#define SERVICENAME L"aaaa"
#define SLEEP_TIME 1000

char* filename = "C:\\Users\\Vignesh Kumar S\\Desktop\\VisualStudio\\Services\\Text.txt";
FILE* fp;

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;
void  ServiceMain(int argc, char** argv);
void  ControlHandler(DWORD request);
int InitService();
int WriteToLog(char* str);
SC_HANDLE handleToServiceControlManagerDB(void);
SC_HANDLE openService(SC_HANDLE handleSCMDB);
void __stdcall createService(void);
void __stdcall startService(void);
void __stdcall stopService(void);
void __stdcall deleteService(void);

int WriteToLog(char* str)
{
	FILE* log;
	fopen_s(&log, filename, "a+");
	if (log == NULL)
		return -1;
	fprintf(log, "%s\n", str);
	fclose(log);
	return 0;
}

void ServiceMain(int argc, char** argv)
{
	int error;
	static int count = 1;

	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	hStatus = RegisterServiceCtrlHandler(
		L"aaaa",
		(LPHANDLER_FUNCTION)ControlHandler
	);
	if (hStatus == (SERVICE_STATUS_HANDLE)0)
	{
		WriteToLog("Registering Control Handler failed\n");
		return;
	}
	// Initialize Service 
	error = InitService();
	if (error)
	{
		// Initialization failed
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode = -1;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;
	}
	// We report the running status to SCM. 
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &ServiceStatus);

	// The worker loop of a service
	while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		char buffer[16];
		sprintf_s(buffer, sizeof(buffer), "Count\t: %d\n", count++);
		int result = WriteToLog(buffer);
		if (result)
		{
			ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			ServiceStatus.dwWin32ExitCode = -1;
			SetServiceStatus(hStatus, &ServiceStatus);
			return;
		}
		Sleep(SLEEP_TIME);
	}
	return;
}

int InitService()
{
	int result;
	result = WriteToLog("Service aaaa STARTED.");
	return (result);
}

void ControlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		WriteToLog("SERVICE aaaa stopped.");
		WriteToLog("\n-------------------------------------------------\n");
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
		"aaaa",
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
		SERVICE_DEMAND_START,      // start type
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
	sd.lpDescription = L"This is Sample Service Creation by Me";
	if (!ChangeServiceConfig2(
		schService,                 // handle to service
		SERVICE_CONFIG_DESCRIPTION, // change: description
		&sd))                      // new description
	{
		wprintf(L"\n\tChangeServiceConfig2() failed, error %d\n", GetLastError());
	}
	else
		wprintf(L"\n\tService description updated successfully.\n");

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