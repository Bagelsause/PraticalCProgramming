#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double sum(const double* nums, size_t count) {
	double sumValue = 0;
	for (int i = 0; i < count; i++) {
		sumValue += nums[i];
	}
	
	return sumValue;
}

double average(const double* nums, size_t count) {
	double sumTotal = sum(nums, count);
	double returned = (sumTotal) / (count);
	return returned;
}

double std_dev(const double* nums, size_t count) {
	double avgValue = average(nums, count);
	double runningTotal = 0;
	for (int j = 0; j < count; j++) {
		double deviation = nums[j] - avgValue;
		runningTotal += deviation*deviation;
	}
	
	double standardDev = sqrt(runningTotal / count);
	
	return standardDev;
}

int main (int argc, char *argv[]){
	int num;
	printf("How many numbers will you enter? ");
	scanf("%i", &num);
	double* list = malloc(sizeof(double) * num + sizeof(double));
	
	printf("Enter %i numbers, one per line: \n", num);
	for (int i = 0; i < num; i++){
		scanf("%lf", &list[i]);
	}
	
	printf("Sum: %0.3lf", sum(list, num));
	printf("\n Average: %0.3lf", average(list, num));
	printf("\n Standard Deviation: %0.3lf \n", std_dev(list,num));

	return 0;

}
