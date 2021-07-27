#include <stdio.h>
#include <Windows.h>
#include <winsvc.h>

SC_HANDLE openService(SC_HANDLE handleSCMDB);
SC_HANDLE handleToServiceControlManagerDB();
void startService();
void stopService();
void createService();
void deleteService();


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

SC_HANDLE handleToServiceControlManagerDB()
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

void startService()
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
	system("PAUSE");
}

void stopService()
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
	system("PAUSE");
}

void createService()
{
	//1. handle to the service control manager database
	SC_HANDLE handleToSCMDB = handleToServiceControlManagerDB();
	if (handleToSCMDB == NULL)
	{
		return;
	}

	// Service display name...
	LPCTSTR lpszDisplayName = L"AAAA";

	// Registry Subkey
	LPCTSTR lpszServiceName = L"aaaa";

	//2. CreateService() created a service and returns handle to the created service
	SC_HANDLE handleToService = CreateService(
		handleToSCMDB,			// SCManager database
		lpszServiceName,	// name of service
		lpszDisplayName,	// service name to display
		SERVICE_ALL_ACCESS,	// desired access
		SERVICE_WIN32_OWN_PROCESS,	// service type
		SERVICE_DEMAND_START,		// start type
		SERVICE_ERROR_NORMAL,		// error control type
		L"C:\\Program Files\\Common Files\\McAfee\\ActWiz\\McAWFwk.exe",	// service's binary
		NULL, // no load ordering group
		NULL, // no tag identifier
		NULL, // no dependencies
		NULL, // LocalSystem account
		NULL); // no password

	if (handleToService == NULL)
	{
		puts("\n\tCreateService() function failed");
		printf("\n\tError No\t: %ld\n", GetLastError());
		return;
	}
	else
		printf("\n\tCreateService() function for %ls success\n", lpszServiceName);

	puts("");
	CloseServiceHandle(handleToSCMDB);
	CloseServiceHandle(handleToService);
	system("PAUSE");
}

void deleteService()
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
	system("PAUSE");
}

int main(int argc, char* argv[])
{
	// START AND STOP EXISTING SERVICES
	if (lstrcmpiA(argv[1], "Start") == 0)
	{
		puts(" _______________________________________________ ");
		puts("|\tSTART EXISTING SERVICES\t\t\t|");
		puts("|_______________________________________________|");
		startService();
	}
	else if (lstrcmpiA(argv[1], "Stop") == 0)
	{
		puts(" _______________________________________________ ");
		puts("|\tSTOP EXISTING SERVICES\t\t\t|");
		puts("|_______________________________________________|");
		stopService();
	}
	else if (lstrcmpiA(argv[1], "Create") == 0)
	{
		puts("\n===================================================================================\n\n");
		puts(" _______________________________________________________ ");
		puts("|\tCREATING THE SERVICE USING .exe FILE\t\t|");
		puts("|_______________________________________________________|");
		// CREATING THE SERVICE USING EXISTING .exe FILE
		puts("");
		createService();
	}
	else if (lstrcmpiA(argv[1], "Delete") == 0)
	{
		puts("\n===================================================================================\n\n");
		puts(" _______________________________________ ");
		puts("|\tDELETING THE SERVICE\t\t|");
		puts("|_______________________________________|");
		// CREATING THE SERVICE USING EXISTING .exe FILE
		puts("");
		deleteService();
	}
	return 0;
}