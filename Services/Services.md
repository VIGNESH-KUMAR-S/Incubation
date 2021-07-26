# Services in WINDOWS
- A Windows service is a computer program that operates in the background.
- It is similar in concept to a Unix daemon.
- Windows Services are managed via the **Services Control Manager panel**.
- Services can be deleted by a user with administrative privileges, but as doing so can render the operating system unstable, it should be done only when necessary and with caution.
- Windows services are the right thing to use for something that should run all of the time, whether or not a user is logged in.
- If you need something to run without an active user logged in, you need to use a windows service.
- The following are some characteristics of Windows services:
    - Always running
    - No UI
    - Run in a separate Windows session, so are available to all users
    - Offer recovery actions
- Services has several internal states in its lifetime,
    1. Running
    2. Stopped
    3. Paused
    4. Pending
- The service control manager (SCM) maintains a database of installed services and driver services, and provides a unified and secure means of controlling them. 
- The database includes information on how each service or driver service should be started. 
- It also enables system administrators to customize security requirements for each service and thereby control access to the service.