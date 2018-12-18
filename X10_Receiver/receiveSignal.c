#include "head.h"


int comp_X10(int Set_X10[], int X10_IN[], int start, int end)
{
  int i = start;
  int j = end;
  while(i < j)
    {
      if(X10_IN[i] == Set_X10[i])
        i++;
      else
        i = j + 1;
    }

  if(i == j)
    return 1;
  else
    return 0;
}
