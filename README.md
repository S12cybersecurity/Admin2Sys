# Admin2Sys
Admin2Sys it's a C++ malware to escalate privileges from Administrator account to NT AUTHORITY SYSTEM. NT AUTHORITY SYSTEM it's the "user" account with most permisions in Windows OS

# Usage
Let's use and prove it:

First of all i open a Windows 10 Home, with Antivirus (Kaspersky) and Windows Defender activated.

Now i transfer the malicious binary to execute it:

![image](https://user-images.githubusercontent.com/79543461/235515992-f2535ab8-045c-4d78-8d56-12e8a7af0b36.png)

And now i execute it:

![image](https://user-images.githubusercontent.com/79543461/235516023-c61fdf56-fa7d-4862-ad3b-124ccc3e3be0.png)

In this case i execute a CMD.

![image](https://user-images.githubusercontent.com/79543461/235516085-01538fd4-1875-4764-83da-c9dd0d468496.png)

And this is a popped CMD, now i execute the whoami command:

![image](https://user-images.githubusercontent.com/79543461/235516137-a3dcdc6a-da3d-4b86-9a84-a8a35202a8e5.png)

# Code
This code is a C++ program that attempts to run a specified application with SYSTEM-level privileges on a Windows system by finding a process running with SYSTEM-level privileges and then duplicating its access token to create a new process with the same privileges.

Here is a brief explanation of each function in the code:

getToken: This function takes a process ID (pid) and returns a handle to the process's access token (cToken). It uses the OpenProcess function to open a handle to the process and then uses the OpenProcessToken function to get the process's access token.

createProcess: This function takes a handle to an access token (token) and the path of an executable file (app) and creates a new process with the same privileges as the process associated with the access token. It uses the DuplicateTokenEx function to create a duplicate of the access token, and then uses the CreateProcessWithTokenW function to create the new process.

GetProcessUserName: This function takes a process ID (pid) and returns the username associated with the process's access token. It uses the OpenProcess function to open a handle to the process, and then uses the OpenProcessToken function to get the process's access token. It then uses the GetTokenInformation function to get the user SID associated with the access token, and the LookupAccountSid function to convert the SID to a username.

main: This function is the entry point of the program. It first prompts the user to enter the path of the application they want to run as SYSTEM. It then loops through all the running processes on the system using the CreateToolhelp32Snapshot function and the Process32First and Process32Next functions. For each process, it checks the username associated with the process's access token using the GetProcessUserName function. If the username is blank or "NT AUTHORITY/SYSTEM", it tries to create a new process with the same privileges as the current process using the getToken and createProcess functions. If it is successful in creating the new process, the loop is exited and the program terminates.
