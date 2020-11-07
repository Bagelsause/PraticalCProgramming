#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//POGGING AND CHAMPING THIS WORKED FIRST TRY
size_t my_strspn(const char *s, const char *accept) {
	for (int i = 0; i < strlen(accept); i++) {
		if (s[i] != accept[i]){
			return i;
		}
	}
	
	return strlen(s);
}

//ANOTHER POGCHAMP THIS WORKED
size_t my_strcspn(const char *s, const char *reject) {
	for (int j = 0; j < strlen(reject); j++) {
		for (int k = 0; k < strlen(s); s++) {
			if (reject[j] == s[k]) {
				return k;
			}
		}
	}
	
	return strlen(s);
}

char *my_strtok(char *s, const char *delims) {
    // static variables maintain their value between
    // calls to the function in which they are declared.
    static char *next_token = NULL;
    
    if(s == NULL) {
        // If the first argument is NULL, we're
        // starting from the last token we found.
        s = next_token;
    }
    
    // Strspn finds how many characters satisfy the first delimeter, then increments start of s to be at delim position
    // What is the next line of code doing?
    s += my_strspn(s, delims);
    
    // If we're at the end of the string, we're done
    if(*s == '\0') {
        return NULL;
    }
    
    // next_token is equal to the next place of the delimiter, such that if the string was "hello world" and you looked for " ", you would get the index at 4
    // What is the next line of code doing?
    next_token = s + my_strcspn(s, delims);
    
    // if the next token is not equal to null, sets it equal to null, and changes next_token to the next space to start from
    // What is this if-statement doing and why?
    if(*next_token != '\0') {
        *next_token = '\0';
        next_token++;
    }
    
    return s;
}

int main (int argc, char *argv[]){
	
	// strtok will modify its argument,
	// so we can't use a literal directly.
	char *s = strdup("This is a strtok test.");

	const char *delims = " ";

	// The first call needs the string we're
	// tokenizing.
	char *tok_start = strtok(s, delims);

	while(tok_start != NULL) {
		// strtok will add null bytes to our
		// original string so that the things
		// it returns are effectively substrings
		printf("got a token: \"%s\"\n", tok_start);
		
		// Subsequent calls operating on the
		// same string need to pass NULL for
		// the first argument.
		tok_start = strtok(NULL, delims);
	}

	free(s);
	
	return 0;
	
}