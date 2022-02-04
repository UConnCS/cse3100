#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Input:
 *    NUL terminated string 'phrase' which consists of lower-case letters,
 *    spaces, and punctuation marks
 *
 * Return value:
 *    1 if 'phrase' is an isogram, and 0 otherwise.
 */

int is_isogram(char phrase[]) {
    int len = strlen(phrase);
    int count[27]; 
    for (int i = 0; i < 27; i++)
        count[i] = 0;

    for (int i = 0; i < len; i++) {
        char c = phrase[i];
        if (c == '-')
            continue;

        int letter = c & 31;
        count[letter] += 1;
    }

    int equal;
    for (int i = 0; i < 27; i++) {
        if (count[i] == 0)
            continue;

        equal = count[i];
        break;
    }

    for (int i = 0; i < 27; i++) {
        int cur = count[i];
        if (cur == 0)
            continue;

        if (cur != equal)
            return 0;
    }

    return 1;
}

// DO NOT MODIFY THE CODE BELOW

#define MAXLEN 1000

int main( void )
{
  int i;

  char buff[MAXLEN], phrase[MAXLEN];

  fgets(buff, sizeof buff, stdin);         // reads at most MAXLEN-1 chars to avoid buffer overflow

  for(i=0; buff[i] != '\n' && buff[i] != '\0'; i++) {
    if( isupper(buff[i]) )
      phrase[i] = tolower( buff[i] ); // convert uppercase letters to lowercase
    else
      phrase[i] = buff[i];            // copy everything else unchanged
  }

  phrase[i] = '\0';                   // add NUL terminator

  if( is_isogram(phrase) )
    printf("isogram\n");
  else
    printf("not isogram\n");

  return 0;
}