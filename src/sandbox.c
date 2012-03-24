#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <ucontext.h>

extern void readString(char *s, int r);

void signalFunc(int sig, siginfo_t *siginfo, void *ucp)
{
	static ucontext_t *recordedAddress = 0;
	if (recordedAddress == 0)
	{
		//Entering a function.
		recordedAddress = ((ucontext_t *) ucp)->uc_link;//siginfo->si_addr;
		printf("\nSIGSEV - Entering a function");
		//Unprotect the page corresponding to the SIGSEGV
		mprotect((void*)(*(unsigned int *) siginfo->si_addr&0xfffffffffffff000), 1,PROT_READ | PROT_WRITE | PROT_EXEC);
		//Protect the page where the call originated from.
		mprotect((void*)(recordedAddress & 0xfffffffffffff000), 1, PROT_WRITE | PROT_EXEC);
	}
	else// if (recordedAddress == *(unsigned int *) siginfo->si_addr)
	{
		//Leaving a function.
		printf("\nSIGSEV - Leaving a function (%u)",recordedAddress);
		//Unprotect the target page of the return
		mprotect((void*)(recordedAddress & 0xfffffffffffff000), 1,PROT_READ | PROT_WRITE | PROT_EXEC);
		//Protect where the call originated from
		mprotect((void*)(*(unsigned int *) ucp & 0xfffffffffffff000), 1, PROT_WRITE | PROT_EXEC);
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
