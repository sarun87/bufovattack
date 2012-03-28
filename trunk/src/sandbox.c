/* Group Info - 
asriram Arun Sriraman
shyamp Shyam Prasad
vineet Vineet Krishnan
*/
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ucontext.h>

#define REG_RSP 15

extern void readString(char *s, int r);

void signalFunc(int sig, siginfo_t *siginfo, void *ucp)
{
	static unsigned long recordedAddress = 0;
	static unsigned long functionStartAddress = 0;
	if (recordedAddress == 0)
	{
		//Entering a function.
		recordedAddress  = (unsigned long) *(unsigned long**) ((ucontext_t *)ucp)->uc_mcontext.gregs[REG_RSP];
		functionStartAddress = (unsigned long) siginfo->si_addr;
		//printf("\nSIGSEV - Entering a function (%x)",functionStartAddress);
		//printf("\nRecorded address - %x",recordedAddress);
		if (mprotect((void*)(functionStartAddress&0xfffffffffffff000), 1,PROT_WRITE | PROT_EXEC | PROT_READ) == -1)
		{
			printf("\nmprotect failed for %u",functionStartAddress);
			exit(0);
		}
		//Protect the page where the call originated from.
		if (mprotect((void*)(recordedAddress & 0xfffffffffffff000), 1, PROT_NONE) == -1)
		{
			printf("\nmprotect failed for %u",recordedAddress);
			exit(0);
		}
	}
	else if (recordedAddress == (unsigned long) *(unsigned long**)((ucontext_t *) ucp)->uc_mcontext.gregs[REG_RSP])
	{
		//Leaving a function.
		printf("\nSIGSEV - Leaving a function (%u)",functionStartAddress);
		//Unprotect the target page of the return
		mprotect((void*)(recordedAddress & 0xfffffffffffff000), 1, PROT_WRITE | PROT_EXEC | PROT_READ);
		//Protect where the call originated from
		mprotect((void*)(functionStartAddress & 0xfffffffffffff000), 1, PROT_NONE);
		recordedAddress = 0;
	}
	else
	{
		printf("malicious buffer overflow detected, will exit!\n");
		exit(0);
	}
}

//Init the signal handlers here
void init_sandbox()
{
	struct sigaction segfaulthandler;

	sigemptyset(&segfaulthandler.sa_mask);
	segfaulthandler.sa_flags = SA_RESTART | SA_SIGINFO;
	segfaulthandler.sa_sigaction = signalFunc;
	sigaddset(&segfaulthandler.sa_mask,SIGSEGV);
	sigprocmask(SIG_UNBLOCK,&segfaulthandler.sa_mask,NULL);
	sigaction(SIGSEGV,&segfaulthandler,NULL);

	mprotect((void*)((unsigned long)readString & 0xfffffffffffff000), 1,PROT_READ | PROT_WRITE/* | PROT_EXEC */); // cannot execute !!!
}
