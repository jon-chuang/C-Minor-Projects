#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

void createFileName(char name[], char ext[]);
int checkPali(char word[]);

FILE *write;
FILE *read;

void main()
{
 	char input[1000], fileNameRead[1000] = {'\0'}, fileNameWrite[1000] = {'\0'};
 	char nnot[4] = {'\0'};
 	int paliBool;
 	int j;
 
	printf("Palindrome Searcher.\n");
 	printf("Enter name of .txt file containing words you want search (w/o .txt): ");
 	createFileName(fileNameRead, "txt");
 	
 	printf("PaliSearch will read from %s\n", fileNameRead);
 	
 	printf("Enter name of file you would like to print to (w/o .txt): ");
 	createFileName(fileNameWrite, "txt");
 	
 	printf("PaliSearch will write to %s\n", fileNameWrite);
 	
 	read = fopen(fileNameRead, "r");
                   
	write = fopen(fileNameWrite, "w");  // Cleans existing write file of old data
	fprintf(write, '\0');
	fclose(write);
	
 	write = fopen(fileNameWrite, "a");
 	
 	while(fgets(input, 1000, read) != NULL){
 		if (checkPali(input) == 1){
 			fprintf (write, input);
 			printf("%s", input);
 		}
 	}
 	fclose(read);
 	fclose(write);
 	
 	printf("Search completed. Your palindromes from %s have been printed to %s.\n", 
	fileNameRead, fileNameWrite);
	
	printf ("\nTo terminate, press enter.");
	char res = getchar();
	if (res == 10) {     //ASCII 10 = \n
	exit(0);
	}
}

/* Define function checkPali, which returns a 1 if word is a pali, and 0 otherwise. 
It also returns 0 if any of the characters are not letters of the alphabet. 
Neglects last character which is assumed to be newline or whitespace.*/
int checkPali(char word[]){
	int i;
 	for (i=0; i <=(strlen(word)-1)/2; i++){
  
		if (word[i]!= word[strlen(word)-2-i] || 
		!(word[i] >= 65 && word[i] <= 90) && !(word[i] >= 97 && word[i] <= 122) && 
		!(word[strlen(word)-1-i] >= 65 && word[strlen(word)-1-i] <= 90) && !(word[strlen(word)-1-i] >= 97 && word[strlen(word)-1-i] <= 122)){
  			return 0;
		}
		
 	}
 	return 1;
}

/* Define function createFileName, which creates the address of a file in the same directory as 
.exe file with file name ~name and file extension .~ext */
void createFileName(char name[], char ext[]){
	char nametemp[strlen(name)+1];
	char exttemp[strlen(ext)+1];

	fgets(name, 100, stdin);
	
	strncpy(nametemp, "./", 2);
	strcat(nametemp, name);
	strncpy(name, nametemp, strlen(nametemp)-1);
	
	strncpy(exttemp, ".", 1);
	strcat(exttemp, ext);

	strcat(name, exttemp);

}
