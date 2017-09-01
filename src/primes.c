#include <math.h>

#include "primes.h"

int is_prime(const int x){
  int i;
  
  if(x < 2) // 1, 0 or negative number == ERROR
    return -1;
  if(x < 4) // it is 2 or 3
    return 1;
  if(x % 2 == 0) // it is even
    return 0;

  for(i = 3; i < floor(sqrt(x)); i += 2)
    if(x % i == 0)
      return 0;

  return 1;
}


int next_prime(const int x){
  int i = x;
  while(is_prime(i) != 1)
    i++;
  
  return i;
}
