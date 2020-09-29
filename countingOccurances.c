#include <stdio.h>
#include <stdlib.h>

int most_common_element(int nums[], size_t length) {
	int numbers [100] = {0};
	
	for (int i = 0; i < length; i++){
		numbers[nums[i]]++;
	}
	
	int maxCount = 0;
	int lowestIndex = 100;
	
	for (int k = 99; k >= 0; k--){
		if (numbers[k] >= maxCount){
			maxCount = numbers[k];
			lowestIndex = k;
		}
	}
	
	return lowestIndex;
}

int main (int argc, char *argv[]){
	int nums[] = {99, 22, 22, 99, 1};
	size_t n = (sizeof(nums) / sizeof(nums[1]));
	int returnedValue = most_common_element(nums, n);
	
	printf("%d \n", returnedValue);
}