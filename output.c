#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
bool
writebytes (unsigned long long x, int nbytes)
{
  do
    {
      if (putchar (x) < 0)
	return false;
      x >>= CHAR_BIT;
      nbytes--;
    }
  while (0 < nbytes);

  return true;
}

bool
writeN (char*buffer, int count){
  int result = write(1,buffer,count);
  if(result>0){
    //printf("\nC\n");
    return true;
  }
  else{
  fprintf(stderr, "Unable to write bytes\n");
  exit(1);
  }
}
