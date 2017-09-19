#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include <limits.h>

double computePoly (double data[], int degree, double arg);
double computeDerivativePoly(double data1[], int degree1, double arg1);
double getRootByBisect(double data2[], int degree2, double neg, double pos);

void main ()
{
	int i, k, l, m, deg;
	double retvalue, root;
	double y, z, slope, nega, posi, yp, yn; 
	double seed, seedslope, argtminus1, argt, argtplus1, arggood, pofnega, pofposi;
	double *point;
	int count, c, limit, converge;
	
	time_t t;
	
	srand(time(&t));
	printf("Seed: %u. Range of roots allowed around Newton's approximation to 100 iterations: [%d, %d].\n\n", t, -RAND_MAX/2, RAND_MAX/2);
	
	printf("Input degree of polynomial: "); //Store user inputs. 
	scanf("%d", &deg);
	
	
	printf("Input coefficient of\n");
	
	double coeff[deg + 1];
		
	for (i = 0; i <= deg; i++){		//Coefficients stored from highest degree to lowest.
		printf("x^%d: ", deg-i);
		scanf("%lf", &coeff[i]);
	}
	
	printf("Polynomial: [");
	for (m = 0; m <= deg - 1; m++){		
		printf("%.2fx^%d + ", coeff[m], deg-m);
	}
	printf("%.2f]\n\n", coeff[deg]);
	
	point = coeff;
	count = 0;
	while (*(point + count) == 0  && count <= deg) count++;// Check that not all coefficients are 0
	
	if (count  > deg){
		printf("Please enter valid (non-constant) polynomial.\nRestarting...\n\n", deg);
		main();
	}
	
	
	// Get seed with smallest y possible
	z = 0;
	y = pow(10, deg) + 1;
	c = 0;
	argtminus1 = 0;
	slope = 0;
	
	// Remove faulty seeds with values too high or slopes to small (singularities)	
	while ((fabs(y) > pow (10, deg) && c <= 100) || fabs(slope) < 0.01){
		c++;
		seed = (double)(rand() - RAND_MAX/2);
		seedslope = computeDerivativePoly(coeff, deg, seed);
		z = computePoly(coeff, deg, seed);
		// printf("Faulty seed argument %d: %E giving y = %E (with slope %E)\n", c, seed, z, seedslope);
		if (fabs(z) <= fabs(y) || fabs(seedslope) >= fabs(slope)) {
			argtminus1 = seed;
			y = z;
			slope = seedslope;
		}
	}
	
	y = computePoly(coeff, deg, argtminus1);
	slope = computeDerivativePoly(coeff, deg, argtminus1);
	argt = argtminus1 - y/slope;
	
	z = computePoly(coeff, deg, argt);
	
	printf("\nFinal seed: %.3f, with P[%.3f] = %.3E (with slope %E).\n\n", argtminus1, argtminus1, y, slope);
	
	// printf("Newton: P[%E] = %E (convergence score: %d)\n", argt, z, 0);
	
	//  Apply Newton's method to find acceptable range for root
	converge = 0;
	while (abs(converge) <= 1000){
		z = computePoly(coeff, deg, argt);
		slope = computeDerivativePoly(coeff, deg, argt);
		argtplus1 = argt - z/slope;
		//printf("Newton: P[%E] = %E (convergence score: %d)\n", argt, z, converge);
		
		if (fabs(z) < fabs(y)){  
			y = z;
			arggood = argt;
		}
		
		if (fabs(z) < 0.00000000001){    	// If Newton's method good enough, skip rest of steps.
			printf("\nP(x) = ");
			for (k = 0; k <= deg - 1; k++){	
			printf("%.2fx^%d + ", coeff[k], deg-k);
			}
			printf("%.2f\nP(%.10f) = %.10f", coeff[deg], argt, z);
			
			exit(0);
		}
		if(fabs(argtplus1 - argt) <= fabs(argt - argtminus1)){
			converge++;
		}else converge--;
	
		argtminus1 = argt;
		argt = argtplus1;
	}
		
	y = computePoly(coeff, deg, arggood);
	slope = computeDerivativePoly(coeff, deg, arggood);
	printf("Evaluate poly at %f: %f (Derivative: %f)\n", arggood, y, slope);
	
	// Use rand() to obtain arg that gives opposite sign to ~argtplus1 under polynomial function.
	limit = 0;
	
	if (y < 0){  
		nega = arggood;
		posi = 0;
		yp = -1;
		
		while (yp < 0 && limit < 10000){
			posi = nega + (double)(rand() - RAND_MAX/2);
			yp = computePoly(coeff, deg, posi);
			limit++;
		}
	}else if (y > 0){
		posi = arggood;
		nega = 0;
		yn = 1;
		
		while (yn > 0 && limit < 10000){
			nega = posi + (double)(rand() - RAND_MAX/2);
			yn = computePoly(coeff, deg, nega);
			limit++;
		}
	}else{
		main();
	}	
	// Error message
	if (limit == 10000){
		printf("In all likelihood, polynomial's roots are outside of range or do not exist.\nRestarting...\n\n");
		main();
	}
	
	pofnega = computePoly(coeff, deg, nega);
	pofposi = computePoly(coeff, deg, posi);
	
	
	printf("\nTry bracket [P[%.3f] = %.3f (-ve), P[%.3f] = %.3f (+ve)]\n", nega, pofnega, posi, pofposi);
			
	// Call bisect method
	root = getRootByBisect(coeff, deg, nega, posi);
	printf("\nRoot: %.20f\n", root);
	retvalue = computePoly(coeff, deg, root);
	printf("Evaluate poly: %.20f\n\n", retvalue);
	
	printf("P(x) = ");
	for (k = 0; k <= deg - 1; k++){	
	printf("%.2fx^%d + ", coeff[k], deg-k);
	}
	printf("%.2f \nP(%.10f) = %.10f", coeff[deg], root, retvalue);
	
	exit(0);
}

double computePoly (double data[], int degree, double arg)
{
	int j;
	double result = 0;
	for (j = 0; j <= degree; j++){
		result = result + data[degree-j]*pow(arg, j);
		// printf("poly at %f: %f (%E, %E)\n", arg, result, data[degree-j], pow(arg, j));
	}
	// printf("poly at %f: %f\n", arg, result);
	return result;
}

double computeDerivativePoly(double data1[], int degree1, double arg1)// Compute derivative
{
	int p;
	double dresult = 0;
	for (p = 1; p <= degree1; p++){
		dresult = dresult + (double)p*data1[degree1-p]*pow(arg1, p-1);
	}
	// printf("Derivative at %f: %f\n", arg, result);
	return dresult;
}

double getRootByBisect(double data2[], int degree2, double neg, double pos)
{
	double argmid = (neg + pos)/2;
	double ret = computePoly(data2, degree2, argmid);
	//printf("Result: P(%.15f) = %.15f between [P(%.15f) = %.15f, P(%.15f) = %.15f]\n", argmid, ret, neg, computePoly(data2, degree2, neg), pos, computePoly(data2, degree2, pos));
	
	if(fabs(ret) < 0.000000001){
		return argmid;
	}else if(ret <= -0.000000001){
		getRootByBisect(data2, degree2, argmid, pos);
	}else if(ret >= 0.000000001){
		getRootByBisect(data2, degree2, neg, argmid);
	}
}		
