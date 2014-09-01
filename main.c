#include <stdio.h>
#include "util.h"
#include "slp.h"
#include "prog1.h"

int main (int argc, char const *argv[])
{
  int x = maxargs(prog(), 0);
  printf("%d", x);
  return 0;
}