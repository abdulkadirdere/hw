#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

void wc ( FILE *infile, char *inname);

int main (int argc, char *argv[]) {
	FILE *infile;
	char ch;	
}

void wc(FILE *infile, char *inname) {
	FILE *infile;
	char ch;
	int number,nl,nt,nb,nc=0; //intialise number of lines, tabs, blank spaces and characters to 0

	infile=fopen(argv[1],"r");
	while(1)
	{
		ch=fgetc(infile);
		if(ch==EOF)
			break;
			nc++; //to count number of characters
		if(ch==' ')
			nb++; //to count number of blank spaces
		if(ch=='\n')
			nl++; //to count number of lines
		if(ch=='\t')
			nt++; //to count number of tabs
	}
	number = nl+nt+nb+nc;
	fclose(infile);
	printf("Total number of character in file = %d \n",number);
	return 0;
}
