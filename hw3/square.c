#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*
 * Implement square cipher encoding as described in the problem description
 * Input: NUL terminated string of uppercase characters
 * Return: pointer to a newly allocated NUL terminated string storing the
 *         ciphertext
 */

char* encode(char plaintext[]) {
    int len = strlen(plaintext);
    int dimension = (int) ceil(sqrt(len));
    
    // dimension x dimension matrix
    char** matrix = (char**) malloc(dimension * sizeof(char*));
    for (int i = 0; i < dimension; i++) {
        matrix[i] = (char*) malloc(dimension * sizeof(char));
    }

    // fill matrix with plaintext
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (i * dimension + j < len) {
                matrix[i][j] = plaintext[i * dimension + j];
            } else {
                matrix[i][j] = ' ';
            }
        }
    }

    // if the plaintext is not a square, fill the rest of the matrix with spaces
    if (len < dimension * dimension) {
        int wrap = 0;
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                if (i * dimension + j >= len) {
                    matrix[i][j] = plaintext[wrap];
                    if (wrap + 1 > len) {
                        wrap = 0;
                    } else {
                        wrap++;
                    }
                }
            }
        }
    }

    // go through matrix column by column and fill the result string
    char* result = (char*) malloc(dimension * dimension * sizeof(char));
    int result_index = 0;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            result[result_index] = matrix[j][i];
            result_index++;
        }
    }

    // free matrix
    for (int i = 0; i < dimension; i++) {
        free(matrix[i]);
    }

    free(matrix);
    return result;
}

// DO NOT MODIFY THE CODE BELOW

/*
 * compute polynomial hash code with base 257 for a given array of unsigned chars
 */
#define PRIME_BASE 257u

unsigned int hash(unsigned char s[], int size) {
    unsigned int result = 0;
    
    // compute polynomial hash code using Horner's rule
    for (int i= size - 1; i >= 0; i--)
        result = s[i] + PRIME_BASE * result;

    return result;
}

#define MAXLEN 1000000

int main(void) {
    char buff[MAXLEN];

    // Read input text into the buffer
    // Ignoring all non-alphabetic letters and convert all letters to uppercase

    int ch = getchar(); // get first char
    int len = 0;
    while (ch != EOF && len < MAXLEN - 1) {
        if(islower(ch))
            buff[len++] = toupper(ch);
        else if (isupper(ch))
            buff[len++] = ch;
        ch = getchar(); // get next char
    }

    buff[len] = '\0';  // add NUL terminator

    char* ciphertext = encode(buff);

    if (ciphertext != NULL) {
        int len = strlen(ciphertext);
        if (len <= 100) 
            printf("Got ciphertext %s\n", ciphertext);
        else
            printf("Got ciphertext of length %d with checksum %u\n", 
                len, hash((unsigned char*) ciphertext, len));
    }

    // cleanup
    free(ciphertext);
    return 0;
}