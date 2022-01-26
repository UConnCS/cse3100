#include <stdio.h>

int main() {
  int i=7, j=7;
  if(i == 1)
      if(j == 2)
          printf("%d\n", i = i + j);
  else
      printf("%d\n", i = i - j);
  printf("%d, i-j=%d-%d=%d\n", i, i, j, i-j);

 int x = 3,  y = 8;
 if(!(x > 2 || y < 0))
    printf("true");
 else
    printf("false");
}
