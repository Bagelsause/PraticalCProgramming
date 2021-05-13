#include <gmp.h>
#include <stdio.h>
#include <assert.h>

int main(){
	
  mpz_t number;
  mpz_t prevNumber;
  mpz_t tempNumber;
  mpz_init(number);
  mpz_init(prevNumber);
  mpz_init(tempNumber);
  mpz_set_ui(number,1);
  mpz_set_ui(prevNumber,0);
  
  char baseChar;
  int size = gmp_snprintf (&baseChar, 1, "%Zd", number);
  while (size != 1000) {
	mpz_set(tempNumber, number);
    mpz_add(number, number, prevNumber);
	mpz_set(prevNumber, tempNumber);
	size = gmp_snprintf (&baseChar, 1, "%Zd", number);
	if (size % 10 == 0) {
		gmp_printf ("%Zd\n",  number);
	}
  }
  
  gmp_printf ("============================================= %Zd\n",  number); //Just to show which line is the last one
  //Also how in the hell did this work the first try
  
  mpz_clear(number);
  mpz_clear(prevNumber);
  mpz_clear(tempNumber);
}
