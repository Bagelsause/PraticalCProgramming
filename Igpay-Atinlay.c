#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int isVowel(char check) {
	if (check == 'a' || check == 'e' || check == 'i' || check == 'o' || check == 'u' || check == 'A' || check == 'E' || check == 'I' || check == 'O' || check == 'U') {
		return 1;
	} else if (check == 'y') {
		return 2;
	} else {
		return 0;
	}
}

char *word_to_pig_latin(const char *word) {
	char *editedWord = malloc(strlen(word)+1);
	char *ending = malloc(strlen(word)+4);
	for (int i = 0; i < strlen(word); i++){
		strcpy(&editedWord[i], &word[i]);
	}
	
	editedWord[0] = tolower(word[0]);
	
	if (isVowel(word[0]) == 1) {
		strcpy(ending, "way");
		strcat(editedWord, ending);
		if (islower(word[0]) == 0){
			editedWord[0] = toupper(editedWord[0]);
		}
	} else {
		int index = 0;
		for (int i = 0; i < strlen(editedWord); i++) {
			if (isVowel(word[i]) == 1){
				index = i;
			}
			if (i > 0) {
				if (isVowel(word[i]) == 2) {
					index = i;
					
				}
			}
		}
		
		char *malleable = malloc(strlen(word)+1);
		
		for (int i = 0; i < strlen(editedWord); i++) {
			if (isVowel(word[i]) != 1 || (i == 0 && isVowel(word[i]) == 2)){
				for (int j = 0; j < strlen(editedWord)-1; j++){
					malleable[j] = editedWord[j+1];
				}
			}
		}
		
		char save = editedWord[0];
		
		for (int i = 0; i < strlen(malleable) +1; i++){
			editedWord[i] = malleable[i];
		}
		
		editedWord[strlen(editedWord)] = save;
		strcpy(ending, "ay");
		strcat(editedWord, ending);
		if (islower(word[0]) == 0){ 
			editedWord[0] = toupper(editedWord[0]);
		}
	}
	
	return editedWord;
}

void test_pig_latin() {
    const char *cases[][2] = {
        { "exit", "exitway" },
        { "Yellow", "Ellowyay" },
        { "scram", "amscray" },
        { "blue", "ueblay" },
        { "Type", "Ypetay" },
        { "igloo", "iglooway" },
        { "Float", "Oatflay" },
        { "bandanda",  "andandabay" },
        { "Apple", "Appleway" },
        
        // feel free to add more cases above this line,
        // but leave the { NULL, NULL } line intact.
        { NULL, NULL }
    };
    
    size_t i = 0;
    int failed_cases = 0, passed_casses = 0;
    while(1) {
        const char *original = cases[i][0];
        const char *expected_latin = cases[i][1];
        
        if(!original || !expected_latin) {
            break;
        }
        
        char *actual_latin = word_to_pig_latin(original);
        
        if(!actual_latin || strcmp(expected_latin, actual_latin) != 0) {
            printf("Test case failed: expected '%s' -> '%s', but got '%s'\n", original, expected_latin, actual_latin);
            
            failed_cases++;
        }
        else {
            passed_casses++;
        }
        
        free(actual_latin);
        
        i++;
    }
    
    printf("%d/%d cases passed\n", passed_casses, failed_cases + passed_casses);
}

int main(int argc, char *argv[]) {
    test_pig_latin();
    
    return 0;
}