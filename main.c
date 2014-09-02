#include <stdio.h>
#include "impl.h"
#include "prog1.h"

int main (int argc, char const *argv[])
{
  interp(prog());
  return 0;
}