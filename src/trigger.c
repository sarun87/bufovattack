#include <stdio.h>
#include <malloc.h>
#include <string.h>

char * inp;

// Seg Fault Case
void fnCaseE()
{
	char* inp = malloc(sizeof(char) * 68);
	char* name = "Arun";
	strcpy(inp,name);
	int i;
	for(i = 4; i < 63; ++i)
	{
		inp[i] = '\0';
	}
	inp[64] = (char) 0x00;
	inp[65] = (char) 0x00;
	inp[66] = (char) 0x00;
	inp[67] = (char) 0x00;
	//inp[65] = (char) 0x07;
	//fwrte(inp,1,66,stdout);//stdout,inp,66);
	write(1,inp,68);
	fflush(stdout);
	//inp[69] = '\0';
}
void fnCaseD(){

	char* inp = malloc(sizeof(char) * 68);
	char* name = "Arun";
	strcpy(inp,name);
	int i;
	for(i = 4; i < 63; ++i)
	{
		inp[i] = '\0';
	}
	inp[64] = (char) 0x00;
	inp[65] = (char) 0x07;
	inp[66] = (char) 0x40;
	inp[67] = (char) 0x00;
	//inp[65] = (char) 0x07;
	//fwrte(inp,1,66,stdout);//stdout,inp,66);
	write(1,inp,68);
	fflush(stdout);
	//inp[69] = '\0';

}

int main(int argc, char**argv)
{
	switch(argv[1][0])
	{
		case 'd': fnCaseD();
			 break;
		case 'e': fnCaseE();
			 break;
		default: printf("Arun");
			break;
	}
	return 0;
}
