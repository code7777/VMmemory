/*

Course: CS 30200
Name: Preston Porter
Assignment: 4
*/
/*
     This program accepts commands that cause it to perform virtual memory
     operations. The commands are read from standard input, but it is better
     to put the commands in a "script file" and use the operating system's
     command line to redirect the script file to this program's standard input
     (as in "C:\VMdriver < VMcmds.txt").

     The commands that this program accepts are of the form

     time, vmOp, vmAddress, units, access

     The five parameters have the following meaning:

     time - Seconds to wait after reading the command before performing the VM operation.
     vmOp - Code that represents the VM operation to perform.
     vmAddress - virtual memory address (in hex) where the VM operation is to be performed
     units - The number of units to use in the VM operation.
             For reserving memory, each unit represents 65536 bytes of memory.
             For committing memory, each unit represents 4096 bytes of memory.
     access - Code that represents the access protection.

     The vmOp codes and their meanings are:
     1 - Reserve a region of virtual memory.
     2 - Commit a block of pages.
     3 - Touch pages in a block.
     4 - Lock a block of pages.
     5 - Unlock a block of pages.
     6 - Create a guard page.
     7 - Decommit a block of pages.
     8 - Release a region.

     The access codes and their meaning are:
     1 - PAGE_READONLY
     2 - PAGE_READWRITE
     3 - PAGE_EXECUTE
     4 - PAGE_EXECUTE_READ
     5 - PAGE_EXECUTE_READWRITE
     6 - PAGE_NOACCESS

     Most of the commands are described in the file
        "Virtual Memory from 'Beginning Windows NT Programming' by Julian Templeman.pdf".
     The only command not mentioned there is the "Touch pages in a block" command. This means
     that you should access (read) a memory location from each page in a specified block.

     Be absolutely sure that you check for any errors created by the VM operations
     since you will be trying to cause errors.

     This program should create a process that runs the program VMmapper.exe so that
     you can observe the memory operations as they happen. The program VMmapper takes
     a PID on its command line and then it repeatedly maps and displays (once a second)
     the virtual memory space of the process with that PID. This program should pass on
     the command line its own PID to the VMmapper program.

     When this program has completed all of its operations, it goes into an infinite
     loop.
*/
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <stdlib.h>

#define PAGELIMIT 80            // Number of pages to ask for

LPTSTR lpNxtPage;               // Address of the next page to ask for
DWORD dwPages = 0;              // Count of pages gotten so far
DWORD dwPageSize;               // Page size on this computer

// prototype for the function, defined below, that prints err messages
void printError(char* functionName);

int main(int argc, char *argv[])
{
   int time, vmOp, units, access;
   //LPVOID myVM;
   LPVOID lpvBase;               // Base address of the test memory
   LPVOID vmAddress;
    DWORD myAccess;
    BOOL bSuccess;
   SYSTEM_INFO sSysInfo;         // Useful information about the system
    GetSystemInfo(&sSysInfo);     // Initialize the structure.
    printf("This computer has page size %d.\n", sSysInfo.dwPageSize);
    
    PROCESS_INFORMATION processInfo; /* process currently running on this processor */
    STARTUPINFO startInfo;      
	ZeroMemory(&startInfo, sizeof(startInfo));//fills startInfo with zeros to initialize
	startInfo.cb = sizeof(startInfo); // Set structure size 
   
   // You need to provide the code that starts up the
   // VMmapper.exe program with the PID of this program
   // on the command line. Use the Windows function
   // GetCurrentProcessId() to get this program's PID at
   // runtime.
   char launch[300];
   DWORD pID =  GetCurrentProcessId();
   
   char *programName = "VMmapper.exe";
   sprintf(launch, "%s %d ", programName, pID);
   

 if (!CreateProcess(NULL, launch , NULL, NULL, TRUE,
				HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
				NULL, NULL, &startInfo, &processInfo))
			{
				printError("CreateProcess Error");
             
			}
         
			else
			{
				printf("\n Started program VMapper with pid = %d\n\n",  (int)processInfo.dwProcessId);
            
			}
   Sleep(5000);  // give VMmapper.exe time to start

   // Process loop
   printf("next VM command: ");
   while(scanf("%d%d%p%d%d", &time, &vmOp, &vmAddress, &units, &access) != EOF)
   {
      // wait until it is time to execute the command
      Sleep(time*1000);
      switch (access)
      {
       case 1:
       myAccess= PAGE_READONLY ;
       break;
       case 2:
       myAccess = PAGE_READWRITE;
       break;
       case 3: 
       myAccess = PAGE_EXECUTE ;
       break;
       case 4:
        myAccess = PAGE_EXECUTE_READ ;
        break;
       case 5:
       myAccess = PAGE_EXECUTE_READWRITE;
       break;
       case 6:
       myAccess = PAGE_NOACCESS;
       break;
       
      }
      // Parse the command and execute it
      switch (vmOp)
      {
         case 1:  // Reserve pages in the virtual address space of the process
        lpvBase =  VirtualAlloc
                  (
                  vmAddress, // address
                  units*65536, // Size of allocation
                  MEM_RESERVE, //allocate reserve pages
                  myAccess // access Parameter
                  );
            // provide the code that does this operation
            break;
         case 2:  // Commit a block of pages
          lpvBase = VirtualAlloc
                  (
                  vmAddress,
                  units * 4096, //size of allocation
                  MEM_COMMIT, //commit
                  myAccess
                  );
           // provide the code that does this operation
            break;
         case 3:  // Touch pages in a block
         for 
         (int i = 0; i < units; i++) {

         printf("Touching 0x%p\n", (char *)vmAddress + 4096 * i);

          }
            // provide the code that does this operation
            break;
         case 4:  // Lock a block of pages
            // provide the code that does this operation
           lpvBase = VirtualLock(
                                 vmAddress,
                                 units*dwPageSize
                               );
           break;
          case 5:  // Unlock a block of pages
            // provide the code that does this operationBOOL 
            lpvBase = VirtualUnlock(
                                 vmAddress,
                                 units*dwPageSize
                          );
            
            break;
         case 6:  // Create a guard page
            // provide the code that does this operation
               /*
               PAGE_GUARD
               0x100
               */
               lpvBase =  VirtualAlloc
                  (
                  vmAddress, // selects address
                  units*dwPageSize, // Size of allocation
                  MEM_RESERVE | MEM_COMMIT, 
                  myAccess | PAGE_GUARD // access Parameter
                  );
           break;
         case 7:  // Decommit a block of pages
            // provide the code that does this operation
            bSuccess = VirtualFree(
                              vmAddress, 
                              units * dwPageSize,
                              MEM_DECOMMIT
                             );
            break;
         case 8:  // Release a region
            // provide the code that does this operation
            bSuccess = VirtualFree(
                              vmAddress,
                              units * dwPageSize,
                              MEM_RELEASE
                             );
            break;
      }//switch
      printf("Processed %d %d 0x%p %d %d\n", time, vmOp, vmAddress, units, access);
      printf("next VM command: ");
   }//while*/

   while (1) Sleep(1000); // spin until killed

   return 0;
}//main()



void printError(char* functionName)
{   LPVOID lpMsgBuf;
    int error_no;
    error_no = GetLastError();
    FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
         NULL,
         error_no,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf,
         0,
         NULL
    );
    // Display the string.
    fprintf(stderr, "\n%s failed on error %d: ", functionName, error_no);
    fprintf(stderr, lpMsgBuf);
    MessageBox(NULL, lpMsgBuf, "Error", MB_OK);
    // Free the buffer.
    LocalFree( lpMsgBuf );
}//printError()
