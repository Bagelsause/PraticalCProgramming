#include <stdio.h>
#include <stdlib.h>

int collatzReturn (int number){
	int stoppingTime = 0;
	while (number != 1) {
		if (number % 2 == 0) {
			number /= 2;
			stoppingTime++;
		} else {
			number = (number*3) + 1;
			stoppingTime++;
		}
	}
	return stoppingTime;
}

int main (int argc, char *argv[]){
	int checkUntil = atoi(argv[1]);
	//For the test, checkUntil will be equal to 100,000
	int maxCollatz = 0;
	int maxNum = 0;
	for (int i = 1; i < checkUntil; i++){
		int returnedValue = collatzReturn(i);
		if (returnedValue > maxCollatz){
			maxCollatz = returnedValue;
			maxNum = i;
		}
	}
	printf("%d has a stopping time of %d \n", maxNum, maxCollatz);	

	return 0;

}