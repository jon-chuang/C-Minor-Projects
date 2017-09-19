#include <stdio.h>

int main(void) {
	int d1, d2, d3, ans = 999;
	int brute[90] = {'\0'};
	int greater_brute[100][4] = {'\0'};
	int i, j, k, l, m = 0, n = 0, o = 0, p = 0, q, r, s;

printf("Think of 4 digits, at least one of which is different from the rest.\n"
"Subtract the number made from the digits arranged in ascending order (smallest)\n"
"from the number made drom the digits arranged in descending order (largest).\n\n"
"Enter 3 out of the 4 digits (including all 0s) obtained, separated by spaces:");
// 9 >= a >= b >= c >= d >=0

	scanf("%d %d %d", &d1, &d2, &d3);
	/* compute 999(a-d) + 90(b-c), 1 <= a-d <= 9, 0 <= b-c <= 9 */
	for (i = 1; i <= 9; i++){
		for (j = 0; j <= 9; j++){
			brute [10*(i-1) + j] = 999 * i + 90 * j;
			// printf("%d: %d\n", 10*(i-1) + j + 1, brute [10*(i-1) + j]);
		}
	}
	
	/* Obtain digits in array form */
	for (l = 0; l <= 89; l++){
		for (k = 0; k <= 3; k++){
		 	greater_brute[l][k] = (   brute[l]/(  (int)pow(10, (3-k))  )   )   %10; 
			// stores digits of 4 digit numbers in array
		 }
	}
		
	
	/* brute force check against table */
	for(q = 0; q <= 89; q++){
		answerobtained:
		m=0;
		n=0;
		o=0;
		p=0;
		while(m <= 3){
			n=0;
			o=0;
			p=0;
			while (n <= 3){
				o=0;
				p=0;
				while (o <= 3 && n != m){
					p=0;
					while (p <= 3 && o != n && o != m && n != m){
						if (d1 == greater_brute[q][m] && 
						    d2 == greater_brute[q][n] && 
						    d3 == greater_brute[q][o] && 
						    p != o && p != n && p != m){
						 	ans = greater_brute[q][p];
						 	printf("Success! Digit missing from %d given digits(in position) - " 
							       "%d(%d), %d(%d), %d(%d): %d(%d)\n", 
							       brute[q], d1, m+1, d2, n+1, d3, o+1, ans, p+1);
						 	q++;
						 	goto answerobtained;
						}
						p++;
					}
					o++;
				}
				n++;
			}
			m++;
	
		}
		
	}
	
	if (ans == 999){
		printf("\nInvalid input.\n");
	}else{
		printf("\nThe 4th digit is %d.\n\n", ans);
	}
	main ();
}
