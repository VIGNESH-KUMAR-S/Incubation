#include <stdio.h>
#include <Windows.h>
#include <winsvc.h>

void closeServiceHandle(SC_HANDLE openService)
{
	CloseServiceHandle(openService);
}

int main(void)
{
	SC_HANDLE hscManager;

	//1. handle to the service control manager database
	hscManager = OpenSCManagerA(
		NULL, // local machine
		NULL, // SERVICES_ACTIVE_DATABASE database is opened by default
		SC_MANAGER_ALL_ACCESS
	);
	if (hscManager == NULL)
	{
		puts("\n\tOpenSCManager() function failed");
		printf("\n\tError No\t: %d\n", GetLastError());
		closeServiceHandle(hscManager);
		return 0;
	}
	else
		puts("\n\tOpenSCManager() function success");

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
		closeServiceHandle(openService);
		return 0;
	}
	else
		puts("\n\tOpenServiceA() function success");

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
		closeServiceHandle(openService);
		return 0;
	}
	else
		puts("\n\tStartService() function success");

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
		closeServiceHandle(openService);
		return 0;
	}
	else
	{
		puts("\n\tQueryServiceStatusEx() function success");
		if (serviceStatusProcess.dwCurrentState == SERVICE_STOP || serviceStatusProcess.dwCurrentState == SERVICE_STOPPED)
		{
			puts("\n\tService Already Stopped");
		}
		else if (serviceStatusProcess.dwCurrentState == SERVICE_STOP_PENDING)
		{
			puts("\n\tService Stop Pending!!!!!");
			closeServiceHandle(openService);
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
				closeServiceHandle(openService);
				return 0;
			}
			else
				puts("\n\tControlService() function success");

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
				closeServiceHandle(openService);
				return 0;
			}
			else
			{
				puts("\n\tQueryServiceStatusEx() function success");
				if (serviceStatusProcess.dwCurrentState == SERVICE_STOP_PENDING)
				{
					puts("\n\tService Stop Pending, Wait for more than 2 seconds");
					system("PAUSE");
					if (QueryServiceStatusEx(openService, SC_STATUS_PROCESS_INFO, (LPBYTE)&serviceStatusProcess,
						sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
					{
						puts("\n\tQueryServiceStatusEx() function failed");
						printf("\n\tError No\t: %d\n", GetLastError());
						closeServiceHandle(openService);
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
					closeServiceHandle(openService);
					return 0;
				}
			}
		}
	}
	closeServiceHandle(openService);
}