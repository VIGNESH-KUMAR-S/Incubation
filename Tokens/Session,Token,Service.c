#include <windows.h>
#include <stdio.h>
#include <WtsApi32.h>

#define SERVICENAME L"aaaa"
#define MSGTITLE L"Service aaaa"
#define MSGBODY L"Hi ZOHO Incubation"
#pragma comment(lib, "WtsApi32.lib")
char* filename = "C:\\Users\\Vignesh Kumar S\\Desktop\\VisualStudio\\Session,Token,Service\\Text.txt";
FILE* fp;

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;
HANDLE hProcess;	HANDLE hThread;
void  ServiceMain(int argc, char** argv);
void  ControlHandler(DWORD request);
SC_HANDLE handleToServiceControlManagerDB(void);
SC_HANDLE openService(SC_HANDLE handleSCMDB);
void __stdcall createService(void);
void __stdcall startService(void);
void __stdcall stopService(void);
void __stdcall deleteService(void);
int callNotepad();

int callNotepad()
{
	HANDLE hToken = 0;
	if (!WTSQueryUserToken(WTSGetActiveConsoleSessionId(), &hToken))
	{
		fopen_s(&fp, filename, "a+");
		fp != 0 && fprintf(fp, "\n\tWTSQueryUserToken() Failed\n\n");
		fp != 0 && fclose(fp);
		return -1;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	int processAsUser = CreateProcessAsUser(
		hToken,
		L"C:\\Windows\\notepad.exe",
		NULL, NULL, NULL, 0, 0, NULL, NULL, &si, &pi
	);

	if (processAsUser == 0)
	{
		fopen_s(&fp, filename, "w+");
		fp != 0 && fprintf(fp, "\n\tCreation of Process as User Failed\n\n");
		fp != 0 && fclose(fp);
		return -1;
	}
	hProcess = pi.hProcess;
	hThread = pi.hThread;

	CloseHandle(hToken);
	return 0;
}

void ServiceMain(int argc, char** argv)
{
	ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
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
		fopen_s(&fp, filename, "a+");
		fp != 0 && fprintf(fp, "\n\tRegisterServiceControlHandler() Failed\n\n");
		fp != 0 && fclose(fp);
		return;
	}
	SetServiceStatus(hStatus, &ServiceStatus);

	// Calling Notepad.exe from service
	int result = callNotepad();
	if (!result)
	{
		// We report the running status to SCM. 
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus(hStatus, &ServiceStatus);
	}

	// Message Box
	DWORD response;
	WTSSendMessage(WTS_CURRENT_SERVER_HANDLE, WTSGetActiveConsoleSessionId(),
		MSGTITLE, (wcslen(MSGTITLE) + 1) * sizeof(wchar_t), MSGBODY, (wcslen(MSGBODY) + 1) * sizeof(wchar_t),
		0, 0, &response, FALSE);

	// The worker loop of a service
	// Perform any tasks with this loop
	// This loop should execute untill we stop our service otherwise process terminated

	while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		Sleep(1000);
	}
	return;
}

void ControlHandler(DWORD request)
{
	DWORD exitCodeProcess;
	DWORD exitCodeThread;
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		if (GetExitCodeProcess(hProcess, &exitCodeProcess) != 0)
		{
			TerminateProcess(hProcess, exitCodeProcess);
			CloseHandle(hProcess);
		}
		else
		{
			fopen_s(&fp, filename, "a+");
			fp != 0 && fprintf(fp, "\n\tGetExitCodeProcess for hProcess Failed in ControlHandler()\n\n");
			fp != 0 && fclose(fp);
		}
		if (GetExitCodeProcess(hThread, &exitCodeThread) != 0)
		{
			TerminateProcess(hThread, exitCodeThread);
			CloseHandle(hThread);
		}
		else
		{
			fopen_s(&fp, filename, "a+");
			fp != 0 && fprintf(fp, "\n\tGetExitCodeProcess for hThread Failed in ControlHandler()\n\n");
			fp != 0 && fclose(fp);
		}

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

	// Get current process .exe file
	if (!GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		wprintf(L"\n\tUnable to get path, error %u\n", GetLastError());
		return;
	}
	else
		wprintf(L"\n\tGot binary file path successfully!\n");

	// Get a handle to the SCM database
	schSCManager = handleToServiceControlManagerDB();

	// Create the service
	schService = CreateService(
		schSCManager,              // SCM database
		SERVICENAME,                   // name of service
		SERVICENAME,                   // service name to display
		SERVICE_ALL_ACCESS,        // desired access
		SERVICE_WIN32_OWN_PROCESS, // service type
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
		wprintf(L"\n\tCreateService() is SUCCESS!\n");

	SERVICE_DESCRIPTION sd;
	sd.lpDescription = L"This is Sample Service Creation by Vicky";
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
		L"SYSTEM\\CurrentControlSet\\Services\\aaaa", //Path for Subkey
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
		{ L"", (LPSERVICE_MAIN_FUNCTION)ServiceMain},
		{ NULL, NULL }
	};

	StartServiceCtrlDispatcher(DispatchTable);
	return 0;
}