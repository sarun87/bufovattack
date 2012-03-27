#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ucontext.h>

#define REG_RSP 15
#define REG_RIP 16

extern void readString(char *s, int r);

void signalFunc(int sig, siginfo_t *siginfo, void *ucp)
{
	static unsigned long long recordedAddress = 0;
	static unsigned long long functionStartAddress = 0;
	if (recordedAddress == 0)
	{
		//Entering a function.
		recordedAddress = (unsigned long long)((ucontext_t *) ucp)->uc_mcontext.gregs[REG_RSP];
		functionStartAddress = (unsigned long long)((ucontext_t *) ucp)->uc_mcontext.gregs[REG_RIP];
		//Unprotect the page corresponding to the SIGSEGV
		//functionStartAddress = *(unsigned int *) siginfo->si_addr;
		printf("\nSIGSEV - Entering a function (%u)",functionStartAddress);
		printf("\nRecorded address - %x",recordedAddress);
		printf("\nStack Pointer - %x",(unsigned long long)((ucontext_t *) ucp)->uc_mcontext.gregs[REG_RSP]);
		printf("\nInstruction Pointer - %x",(unsigned long long)((ucontext_t *) ucp)->uc_mcontext.gregs[REG_RIP]);
		printf("\nsiginfo->si_addr - %x",*((unsigned long long *) siginfo->si_addr));
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
	else if (recordedAddress == (unsigned long long)((ucontext_t *) ucp)->uc_mcontext.gregs[REG_RIP])
	{
		//Leaving a function.
		printf("\nSIGSEV - Leaving a function (%u)",functionStartAddress);
		//Unprotect the target page of the return
		mprotect((void*)(recordedAddress & 0xfffffffffffff000), 1, PROT_WRITE | PROT_EXEC | PROT_READ);
		//Protect where the call originated from
		mprotect((void*)(functionStartAddress & 0xfffffffffffff000), 1, PROT_NONE);
		recordedAddress = 0;
	}
	//else
	//{
	//	printf("\nMismatch between recorded and obtained address\n");
	//	exit(0);
	//}
}

//Init the signal handlers here
void init_sandbox()
{
	struct sigaction segfaulthandler;

	sigemptyset(&segfaulthandler.sa_mask);
	segfaulthandler.sa_flags = SA_RESTART | SA_SIGINFO;
	segfaulthandler.sa_handler = signalFunc;
	sigaddset(&segfaulthandler.sa_mask,SIGSEGV);
	sigprocmask(SIG_UNBLOCK,&segfaulthandler.sa_mask,NULL);
	sigaction(SIGSEGV,&segfaulthandler,NULL);

	mprotect((void*)((unsigned int)readString & 0xfffffffffffff000), 1,PROT_READ | PROT_WRITE/* | PROT_EXEC */); // cannot execute !!!
}
