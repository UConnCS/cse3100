#include <stdio.h>

/* Input:
 *    Array 'digit' of 'len' integers between 0 and 9
 *
 * Return value:
 *    1 if the digits in the array pass LUHN-10 validation, 0 otherwise
 * 
 * The first step of the algorithm is to double every second digit, starting from the right, and subtracting 9 if doubling results in a number greater than 9.  For example, if the input number is 
 */

int get_digit(int digit, int i) {
    if (i % 2 != 0)
        return digit;

    if ((digit * 2) >= 9) {
        digit *= 2;
        if (digit - 9 < 0)
            return (digit - 9) * -1;
        return digit - 9;
    }

    return digit * 2;
}

int check_luhn(int digit[], int len) {
    int result = 0;
    for (int i = len - 1; i >= 0; i--) {
        int num = digit[i];
        digit[i] = get_digit(num, i);
        result += digit[i];
    }

    return result % 10 == 0;
}

// DO NOT MODIFY BELOW
#define MAXLEN 1000

int main(void) {
    int buff[MAXLEN];
    int ch, len = 0;

    do {
        ch = getchar();

    if (('0' <= ch) && (ch <= '9')) // store digits
        buff[len++] = ch - '0';
    } while((ch != '\n') && (len < MAXLEN));

    if( check_luhn(buff, len) )
        printf("valid\n");
    else
        printf("invalid\n");

    return 0;
}