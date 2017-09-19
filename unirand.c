#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Implicit function declarations
float getMean (int w, int x, int g, int arraym[]);
float getVar (int a, int b, int h, float mu, int arrayv[]);
int unirand(int k, int j);
void createSave (char name[], char ext[]);
void enter();

/* This program generates sequences of uniform pseudorandom integers, and computes various statistical 
quantities characterising those sequences. The user has options to specify random no. range, no. of sequences, 
and the format in which the generated data is printed. */
void main()
{
	int a, b, c, d, e, f, g, h, i, l, n, p, q, r, s, u, w, v, x, y, z;
	int range, lower, upper, noofno, noofseq;
	float meen, vary, stddevmean = 0;
	int printout = 0, print2txt = 0, printstats2txt; // Boolean control variables
	char yn1[3], yn2[3], yn3[3];
	char filename[100] = {'\0'}, filenamestats[100] = {'\0'};
	char *os = "Unknown";
	
	time_t t;
	
	srand(time(&t)); /* Seed rand with time now */
	
	printf("***********************************************************\n");
	printf("Generated seed at time %u \n", (unsigned int)t);
	
	float getCovar (){
	}
	
	/* User inputs: */
	printf("Enter range of random integers you would like to generate.\n"
	"The maximum range is %d.\n\n"
	"Smallest no.: ", RAND_MAX);
	scanf("%d", &lower);
	printf("Largest no.: ");
	scanf("%d", &upper);
	
	/* Restart program if user inputs invalid range */
	if (upper - lower < 0){
		printf("Invalid range. Please restart.\n\n");
		main();
	}
	
	/* Print expected std. dev */
	range = upper - lower + 1;
	meen = (float )lower +  (float )(range-1)/2;
	vary = 0;
	for (l = 0; l <= range - 1; l++){
		vary += powf((float )(lower + l) - meen, 2.0);
	}
	vary = vary/(float )range;
	printf("> Expected value of standard deviation: %.3f (%.3f - analytic)\n\n", sqrt(vary), (float )(range)/sqrtf(12.0));

    /* Further user inputs */	
    
    printf("How many random numbers would you like to generate per sequence? ");
	scanf("%d", &noofno);
	
	printf("How many sequences would you like to generate? ");
	scanf("%d", &noofseq);
	
	// Computations ---------------------------------------------------------------------------
	
	int num[noofseq][noofno];                   // Define appropriate arrays based on inputs
	int count[noofseq][range];
	float mean[noofseq], var[noofseq], stddev[noofseq];
	
	
	for (g = 0; g <= noofseq - 1; g++){
	
		for (h = 0; h <= range - 1; h++){       // Initialise ~count to zeroes. ~count's indices run from 0 to ~range -1. 
			count[g][h] = 0;
		} 	
		
		for (n = 0; n <= noofno - 1; n++){      // Generate sequence of random numbers of length ~noofno.
			num[g][n] = unirand(lower, upper);
			count[g][num[g][n] - lower]++;       // Increment ~count limit; limit for ~lower has index 0.
		}
		
		/* Statistics Computation */
		mean[g] = getMean (noofno, lower, range, count[g]);
		var[g] = getVar (noofno, lower, range, mean[g], count[g]);
		stddev[g] = (float )sqrt((float )var[g]);
		stddevmean += (1/(float )noofseq) * stddev[g];
	}
	// End of Computations-------------------------------------------------------------------
	
	
	// User option - terminal printouts
	if (noofno <= 250){             // Makes printouts available only if no. of no.s requested is not too large.
	printf("\nWould you like terminal printouts of your sequence? (yes/no): ");
	scanf("%s", yn1);

	
		if (!strcmp(yn1, "yes")){
			printout = 1;
			
		}
		else if (!strcmp(yn1, "no")){
			printout = 0;
		}
		else {
			printf("Input not valid. Restarting...\n\n");
			main ();
		}
	}		
	
	// User option - save sequence to file
	printf("Would you like to save your sequences to a text file? (yes/no): "); 
	scanf("%s", yn2);

	if (!strcmp(yn2, "yes")){
		print2txt = 1;

		printf("Please enter your file name (without .txt): ");
		getchar();
		createSave(filename, "txt");
		printf("> Your file will be saved as: %s\n", filename);
		
	}
	else if (!strcmp(yn2, "no")){
		print2txt = 0;
	}
	else {
		printf("Input not valid. Restarting...\n\n");
		main ();
	}
	
	// User option - save stats to file
	printf("Would you like to save your sequence statistics to a text file? (yes/no): "); 
	scanf("%s", yn3);

	if (!strcmp(yn3, "yes")){
		printstats2txt = 1;
		
		printf("Please enter your file name (without .txt): ");
		getchar();
		createSave(filenamestats, "txt");
		printf("> Your file will be saved as: %s\n", filenamestats);
		
	}
	
	else if (!strcmp(yn3, "no")){
		printstats2txt = 0;
	
	}
	else {
		printf("Input not valid. Restarting...\n\n");
		main ();
	}	
	
	printf("\nPress enter to generate numbers >> ");
	enter();
	// End of user input-----------------------------------------------------------------
				
		
	// Printout phase
	FILE *txtfile;
	FILE *txtfilestats;
	
	if (print2txt == 1)	{                     // Cleans existing sequence file of old data
		txtfile = fopen(filename, "w");
		fprintf(txtfile, '\0');
		fclose(txtfile);
	}
	
	if (printstats2txt == 1)	{             // Cleans existing stats file of old data
		txtfilestats = fopen(filenamestats, "w");
		fprintf(txtfilestats, '\0');
		fclose(txtfilestats);
	}
	

	if (print2txt == 1)	{                     // Open sequence file with location ~filename to append new data
		txtfile = fopen(filename, "a");
	}
	
	if (printstats2txt == 1)	{             // Open stats file with location ~filenamestats to append new data
		txtfilestats = fopen(filenamestats, "a");
	}
	
	for (e=0; e <= noofseq - 1; e++){
		
	/* Printouts */
		printf("\n____________________________________\n");
		printf("SEQUENCE %d\n", e + 1);
		printf("------------------------------------\n");
		if (print2txt == 1){
			fprintf(txtfile, "\n____________________________________\n");
			fprintf(txtfile, "SEQUENCE %d\n", e + 1);
		}
		if (printstats2txt == 1){
			fprintf(txtfilestats, "\n____________________________________\n");
			fprintf(txtfilestats, "SEQUENCE %d\n", e + 1);
		}
		
		/* Number printout */
		if (printout == 1){
			
		
			for (q=0; q <= noofno - 1; q++){
				printf("%d. %d\n", q + 1, num[e][q]);
				if (print2txt == 1){
					fprintf(txtfile, "%d. %d\n", q + 1, num[e][q]);
				}
			}
			printf("------------------------------------\n");
		}
		else if (printout == 0 && print2txt ==1){
			printf("------------------------------------\n");
			for (q=0; q <= noofno - 1; q++){
				fprintf(txtfile, "%d. %d\n", q + 1, num[e][q]);
			}
		}
		
		
		/* Statistics terminal printout */
		printf("Statistics collected\n");
		printf("------------------------------------\n");
		printf(" No. Frequency:\n");
		printf("---- -------------------------------");
			
		for (r=0; r <= range - 1; r++){       // print no.s from lower to upper
		
			printf("\n %d   ", r + lower);
		
			if (count[e][r] <= 100){           // Print stars if total no. is manageable
				for (s=1; s <= count[e][r]; s++ ){
					printf("*");
				}
			}
				
			printf("(%d)", count[e][r]) ;			
		}
		
		printf("\n------------------------------------\n");
		
		printf("Mean: %.3f\n", mean[e]);
		printf("Variance: %.3f\n", var[e]);
		printf("Standard Deviation: %.3f\n", stddev[e]);	
		
		
		// Print stats to file
		
			if (printstats2txt == 1){
				
			fprintf(txtfilestats, "------------------------------------\n");
			
			fprintf(txtfilestats, "Statistics collected\n");
			fprintf(txtfilestats, "------------------------------------\n");
			fprintf(txtfilestats, " No. Frequency:\n");
			fprintf(txtfilestats, "---- ------------------------------");
			
			for (r=0; r <= range - 1; r++){       // print no.s from lower to upper
			
				fprintf(txtfilestats, "\n %d   ", r + lower);
			
				if (count[e][r] <= 50){           // Print stars if total no. is manageable
					for (s=1; s <= count[e][r]; s++ ){
					fprintf(txtfilestats, "*");
					}
					
				}
					
				fprintf(txtfilestats, "(%d)", count[e][r]) ;			
			}
			
			fprintf(txtfilestats, "\n------------------------------------\n");
			
			fprintf(txtfilestats, "Mean: %.3f\n", mean[e]);
			fprintf(txtfilestats, "Variance: %.3f\n", var[e]);
			fprintf(txtfilestats, "Standard Deviation: %.3f\n", stddev[e]);	
		
		}
	}
	
		
	if (print2txt == 1){
		fclose(txtfile);   // Close sequence text file
	}
		
	if (printstats2txt == 1){
			fprintf(txtfilestats, "\nExpected value of standard deviation: %.3f (%.3f - analytic)\nMean sample variance: %.3f \n\n", sqrt(vary), (float )(range)/sqrtf(12.0), stddevmean);
		fclose(txtfilestats);   // Close stats text file
	}
	
	printf("------------------------------------\n");

	if (print2txt == 1){
		
		printf("Sequences saved to: %s\n", filename);          // User report on printed files
	}
	
	if(printstats2txt == 1){
		printf("Statistics saved to: %s\n", filenamestats);
	}
	
	printf("\nExpected value of standard deviation: %.3f (%.3f - analytic)\nMean sample variance: %.3f \n", sqrt(vary), (float )(range)/sqrtf(12.0), stddevmean);

	
	// End of printouts ---------------------------------------------
	
	Sleep(3000);
	
	/* Option to restart program */
	printf ("\nTo restart, press enter.");
	char res = getchar();
	if (res == 10) {     //ASCII 10 = \n
		printf("\n\n");
		main();
	}
	else if(res == 27){  //ASCII 27 = esc
		exit(0);
	}
	else{
		exit(0);
	}
	
}

/* Define function unirand, a generator of uniform pseudorandom integers in range j, k: */
int unirand(int j, int k)
{
	int rng;
	
	rng = k - j + 1;
	if (rng > RAND_MAX){
		printf("Range too large. Please restart.\n\n");
		main();
	}
	
	int remainder = (int)(RAND_MAX % rng);
	int n = rand();
	
	if (n >= RAND_MAX - remainder){
		unirand(j, k);
	}
	else {
		return n % rng + j;
	}
}

/* Define function getMean, which computes the mean from an array of sampled frequencies. 
w ~noofno, x ~lower, g ~range i.e. sizeof arraym, arraym ~count. */
float getMean (int w, int x, int g, int arraym[])
{
	float summean = 0.0;
	int y;
	for (y = 1; y <= g; y++){
		summean += (float )(x + y - 1) * (float )arraym[y-1];   // Standard formula for mean. Probability = (freq)/(total no).
	}
	return summean/(float )w;
	
}

/* Define function getVar, which computes the variance of an an array of sampled frequencies.
	a ~noofno, b ~lower, h ~range, mu ~mean, arrayv ~count. */
float getVar (int a, int b, int h, float mu, int arrayv[])
{
	float sumvar = 0.0;
	int d;
	for (d=1; d <= h; d++){
		sumvar += powf((float )(b + d - 1), 2.0) * (float )arrayv[d-1]/(float )a;
	}
	return sumvar - mu*mu;
}


/* Define function createSave, which creates the address of a file in the same directory as 
.exe file with file name ~name and file extension .~ext. argument for name[] must have buffer space in array to fit additional characters. */
void createSave(char name[], char ext[])
{
	char namey[strlen(name)+1];
	char exty[strlen(ext)+1];

	fgets(name, 100, stdin);
	
	strncpy(namey, "./", 2);
	strcat(namey, name);
	strncpy(name, namey, strlen(namey)-1);
	
	strncpy(exty, ".", 1);
	strcat(exty, ext);

	strcat(name, exty);

}

/* Define function enter, which calls itself if any other key but enter is pressed */
void enter()
{
	char start = getc(stdin);
	if (!(start == 10)){
		enter();
	}
}
		

