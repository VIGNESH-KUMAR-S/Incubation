#include <stdio.h>
#include <Windows.h>
#include <winsvc.h>

int main()
{
	puts(" _______________________________________________ ");
	puts("|\tSTART AND STOP EXISTING SERVICES\t|");
	puts("|_______________________________________________|");
	// START AND STOP EXISTING SERVICES
	SC_HANDLE hscManager;

	//1. handle to the service control manager database
	hscManager = OpenSCManager(
		NULL, // local machine
		NULL, // SERVICES_ACTIVE_DATABASE database is opened by default
		SC_MANAGER_ALL_ACCESS
	);
	if (hscManager == NULL)
	{
		puts("\n\tOpenSCManager() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		return 0;
	}
	else
		puts("\n\tOpenSCManager() function success - Created handle to SCM DB");

	//2. OpenService() returns handle to the service
	SC_HANDLE openService = OpenServiceA(
		hscManager,	//handle to the service control manager DB
		"AppReadiness",	//lpServiceName
		SERVICE_ALL_ACCESS
	);
	if (openService == NULL)
	{
		puts("\n\tOpenServiceA() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		return 0;
	}
	else
		puts("\n\tOpenServiceA() function success - Created handle to particular service");

	//3. StartService() function used to start existing services
	int startService = StartServiceA(
		openService, //handle to the service
		0,	//Number of Arguments
		NULL	//No Arguments
	);
	if (startService == 0)
	{
		puts("\n\tStartService() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		return 0;
	}
	else
		puts("\n\tStartService() function success - Particular service running");
	puts("");
	system("PAUSE");
	//4. QueryServiceStatusEx() function tells status of the service
	SERVICE_STATUS_PROCESS  serviceStatusProcess;
	DWORD dwBytesNeeded;
	int serviceStatus = QueryServiceStatusEx(
		openService,
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&serviceStatusProcess,
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded
	);
	if (serviceStatus == 0)
	{
		puts("\n\tQueryServiceStatusEx() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		return 0;
	}
	else
	{
		puts("\n\tQueryServiceStatusEx() function success - Able to get the status of given service");
		if (serviceStatusProcess.dwCurrentState == SERVICE_STOP || serviceStatusProcess.dwCurrentState == SERVICE_STOPPED)
		{
			puts("\n\tService Already Stopped");
		}
		else if (serviceStatusProcess.dwCurrentState == SERVICE_STOP_PENDING)
		{
			puts("\n\tService Stop Pending!!!!!");
			return 0;
		}
		else if (serviceStatusProcess.dwCurrentState == SERVICE_RUNNING)
		{
			puts("\n\tService Running");

			//5. Stop the service using ControlService() function
			int controlService = ControlService(
				openService,
				SERVICE_CONTROL_STOP,
				(LPSERVICE_STATUS)&serviceStatusProcess
			);
			if (controlService == 0)
			{
				puts("\n\tControlService() function failed");
				printf("\n\tError No\t: %d\n", GetLastError());
				return 0;
			}
			else
				puts("\n\tControlService() function success - I used this to STOP given service");

			serviceStatus = QueryServiceStatusEx(
				openService,
				SC_STATUS_PROCESS_INFO,
				(LPBYTE)&serviceStatusProcess,
				sizeof(SERVICE_STATUS_PROCESS),
				&dwBytesNeeded
			);
			if (serviceStatus == 0)
			{
				puts("\n\tQueryServiceStatusEx() function failed");
				printf("\n\tError No\t: %d\n", GetLastError());
				return 0;
			}
			else
			{
				puts("\n\tQueryServiceStatusEx() function success - To check status of given service");
				if (serviceStatusProcess.dwCurrentState == SERVICE_STOP_PENDING)
				{
					puts("\n\tService Stop Pending, Wait for more than 2 seconds\n");
					system("PAUSE");
					if (QueryServiceStatusEx(openService, SC_STATUS_PROCESS_INFO, (LPBYTE)&serviceStatusProcess,
						sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
					{
						puts("\n\tQueryServiceStatusEx() function failed");
						printf("\n\tError No\t: %d\n", GetLastError());
						return 0;
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
				else
				{
					puts("\n\tService Not Stopped");
					return 0;
				}
			}
		}
	}
	CloseServiceHandle(hscManager);
	CloseServiceHandle(openService);

	puts("\n===================================================================================\n\n");
	puts(" _______________________________________________________ ");
	puts("|\tCREATING THE SERVICE USING EXISTING .exe FILE\t|");
	puts("|_______________________________________________________|");
	// CREATING THE SERVICE USING EXISTING .exe FILE
	puts("");
	system("PAUSE");

	//1. handle to the service control manager database
	hscManager = OpenSCManager(
		NULL, // local machine
		NULL, // SERVICES_ACTIVE_DATABASE database is opened by default
		SC_MANAGER_ALL_ACCESS
	);
	if (hscManager == NULL)
	{
		puts("\n\tOpenSCManager() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		return 0;
	}
	else
		puts("\n\tOpenSCManager() function success - Created handle to SCM DB");

	// The executable for cmd
	LPCTSTR lpszBinaryPathName = L"%SystemRoot%\\System32\\cmd.exe";

	// Service display name...
	LPCTSTR lpszDisplayName = L"AAAA";

	// Registry Subkey
	LPCTSTR lpszServiceName = L"aaaa";

	//2. CreateService() created a service and returns handle to the created service
	SC_HANDLE createService = CreateService(
		hscManager,			// SCManager database
		lpszServiceName,	// name of service
		lpszDisplayName,	// service name to display
		SERVICE_ALL_ACCESS,	// desired access
		SERVICE_WIN32_OWN_PROCESS,	// service type
		SERVICE_DEMAND_START,		// start type
		SERVICE_ERROR_NORMAL,		// error control type
		lpszBinaryPathName,			// service's binary
		NULL, // no load ordering group
		NULL, // no tag identifier
		NULL, // no dependencies
		NULL, // LocalSystem account
		NULL); // no password

	if (createService == NULL)
	{
		puts("\n\tCreateService() function failed");
		printf("\n\tError No\t: %ld\n", GetLastError());
		return 0;
	}
	else
		printf("\n\tCreateService() for %S success - Return handle to created service\n", lpszServiceName);

	puts("");
	system("PAUSE");

	//3. Delete the particular service
	if (!DeleteService(createService) && createService != NULL)
	{
		puts("\n\tDeleteService() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
	}
	else
		puts("\n\tService deleted successfully\n");

	CloseServiceHandle(hscManager);
	CloseServiceHandle(createService);

	return 0;
}