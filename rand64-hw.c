#include <immintrin.h>
#include "./rand64-hw.h"
#include <stdlib.h>
#include <time.h>
struct drand48_data buf;
/* Initialize the hardware rand64 implementation.  */
void
hardware_rand64_init (void)
{
}

/* Return a random value, using hardware operations.  */
unsigned long long
hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x)) //Want to be able to switch between this function and mrand48_r
    continue;
  return x;
}

/* Finalize the hardware rand64 implementation.  */
void
hardware_rand64_fini (void)
{
}


/*Initialize the hardware mrand48_r implementation*/
void
hardware_mrand48_rng_init(void){
  //printf("\nA\n");
  long seedval = time(NULL);
  srand48_r(seedval, &buf);
  //printf("\nB\n");
}

/*Return  a random value, using hardware operations*/
unsigned long long mrand48_rng(void){
  //printf("\nC\n");
  long int a, b;
  mrand48_r(&buf, &a);
  mrand48_r(&buf, &b);
  //printf("\nD\n");
  return ((((unsigned long long) a) << 32) | ((unsigned long long) b & 0x00000000FFFFFFFF));
  //printf("\nD\n");
}

/**Finalize the  mrand48_r implementation*/
void mrand48_rng_fini(void){
//printf("\nE\n");
}