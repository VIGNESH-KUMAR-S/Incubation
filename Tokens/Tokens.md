# Session
- Program under execution called process.
- Each process have collection of threads(small program) and other resources relating to the process.
- Each process in Windows belongs to a single user who started that process, and each process also belongs to something called a Session.
- Each Session in Windows hosts a collection of processes, windows, Window Stations, Desktops, and several other resources.
- Each process belongs to exactly 1 Session and each Session has a Session ID which identifies it. You cannot change a process’ Session after the process is started.
- Session 0
    - Process 0.1
    - Process 0.2
    - Process 0.3
    - …
    - Process 0.N
- Session 1
    - Process 1.1
    - Process 1.2
    - Process 1.3
    - …
    - Process 1.N
- …
- Session M
    - Process M.1
    - Process M.2
    - Process M.3
    - …
    - Process M.N

- Each Session contains a collection of Window Stations, a clipboard, and more.
- There is one special Window Station called Winsta0 for every session. 
- WinSta0 is the only Window Station that can display a user interface and receive user input, it uses the keyboard, mouse and display. 
- Other Window stations cannot display graphical user interfaces nor receive user input.
- Each Window Station contains a collection of Desktops.
- Each user that logs on to Windows is placed in a separate session.
- During start-up(OS boot up), Session 0 is created, and additional sessions are created as needed. 
- Services are always run in Session 0.
- However, in Windows XP and earlier versions of Windows, Session 0 can also run user applications.
- With Fast User Switching enabled in Windows XP, Session 0 is assigned to the first logged-on user together with any applications that the user runs in that session. The second user is assigned to Session 1 and so on.
- From later onwards, Session 0 is reserved exclusively for services and other non-interactive user applications. Users who are logged on to Windows and their user applications must run in Session 1 or higher.
- User interfaces in Session 0 are not supported. Processes running in Session 0 have no access to the graphics hardware thus user interfaces cannot be directly displayed on the monitor.
- We can use **CreateProcessAsUser** to create a process in another Session.
- How Sessions worked with 3 logged on users on windows 10:

    ![](https://brianbondy.com/static/img/PostVistaSessions.png)

# Tokens (Access Token)
- Access tokens contains load of information about user's security and privileges.
- Each process have an copy of an access token, so that it'll be helpful to the process to known about current user logged on.
- When a user logs on, Windows creates an access token for this user. It is used to store a user's identity and privileges.
- Such an access token stores the following information
    - The user's SID (security identifier)
    - The SIDs of the groups that the user belonged to when the user was authenticated
    - A logon SID which identifies the current logon session
    - The user's and groups' privileges
    - An owner SID
    - The SID for the primary group
    - The default discretionary access control list (DACL) that is used when the user creates a securable object (without an explicit security descriptor)
    - The source of the access token
    - A flag that indicates if the token is a primary or an impersonation token
    - A list of restricting SIDs (optional)
    - Current impersonation levels
    - Other statistics

## References
- [Session, Windows station, Desktop](https://brianbondy.com/blog/100/understanding-windows-at-a-deeper-level-sessions-window-stations-and-desktops)
- [Session, Windows station, Desktop](https://techcommunity.microsoft.com/t5/ask-the-performance-team/sessions-desktops-and-windows-stations/ba-p/372473)
- [Access Token](https://docs.microsoft.com/en-us/windows/win32/secauthz/access-tokens)