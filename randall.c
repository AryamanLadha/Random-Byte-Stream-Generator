/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <cpuid.h>
#include <errno.h>
#include <immintrin.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./output.h"
#include "./options.h"
#include "./rand64-hw.h"
#include "./rand64-sw.h"
/*Lisy of all used global variables*/
/* Hardware implementation.  */

/* Description of the current CPU.  */
struct cpuid { unsigned eax, ebx, ecx, edx; };

/* Return information about the CPU.  See <http://wiki.osdev.org/CPUID>.  */
static struct cpuid
cpuid (unsigned int leaf, unsigned int subleaf)
{
  struct cpuid result;
  asm ("cpuid"
       : "=a" (result.eax), "=b" (result.ebx),
	 "=c" (result.ecx), "=d" (result.edx)
       : "a" (leaf), "c" (subleaf));
  return result;
}

/* Return true if the CPU supports the RDRAND instruction.  */
static _Bool
rdrand_supported (void)
{
  struct cpuid extended = cpuid (1, 0);
  return (extended.ecx & bit_RDRND) != 0;
}


/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  // /*Parse the arguments*/
  struct opts opts;
  read_options(argc, argv, &opts);
  if(!(opts.valid)){
    fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
    return 1; 
  }
  
  /* If there's no work to do, don't worry about which library to use.  */
  if (opts.nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  if(opts.input == RDRAND){
  if (rdrand_supported ())
    {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
    else{
    fprintf (stderr, "rdrand is not supported on this system");
    return 1; 
    }
  }
  else if(opts.input == SLASH_F)
  {
      size_t size = strlen(opts.r_src);
      rsrc = (char*)malloc(size+1);
      strcpy(rsrc, opts.r_src);
      initialize = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
  }
  else if(opts.input == MRAND48_R )
  {
      //printf("\nCorrect\n");
      initialize = hardware_mrand48_rng_init;
      rand64 = mrand48_rng;
      finalize = mrand48_rng_fini;
  }


  initialize ();

  int output_errno = 0;

  if(opts.output == N){
    //printf("B\n");
    int num_size = sizeof(unsigned long long);
    int buffersize = opts.block_size*1024;
    int iterations = buffersize/num_size;
    char*buffer = malloc(buffersize * sizeof(char));
    do
      {
        int outbytes = opts.nbytes < buffersize ? opts.nbytes : buffersize;
        for(int i = 0; i<iterations; i++){
          unsigned long long x = rand64 ();
          memcpy(&buffer[i*num_size],&x, num_size);
        }
        writeN(buffer, outbytes);
        opts.nbytes -= outbytes;
      }
    while (0 < opts.nbytes);
    free(buffer);
  }
  else if(opts.output == STDOUT){
    //printf("A\n");
    int wordsize = sizeof rand64 ();
    do
    {
      unsigned long long x = rand64 ();
      int outbytes = opts.nbytes < wordsize ? opts.nbytes : wordsize;
      if (!writebytes (x, outbytes))
	    {
	      output_errno = errno;
	      break;
	    }
      opts.nbytes -= outbytes;
    }
    while (0 < opts.nbytes);
  }


  else{
    fprintf (stderr, "Invalid output option.");
    return 1; 
  }
  
  //printf("\nHer1\n");


  if (fclose (stdout) != 0){
    output_errno = errno;
    //printf("\nkay\n");
  }

  //printf("\nkay\n");

  if (output_errno)
  {
      errno = output_errno;
      perror ("output");
  }
  
  //printf("\nHer2\n");
  finalize ();
  return !!output_errno;


}
