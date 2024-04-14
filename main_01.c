#include <stdio.h>
#include <string.h>
#include "functions.h"

#define INPUT "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"
#define ANSWER "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"

// Challenge 1: Convert hex to base64
int main() {

    char* converted_base64 = hex_to_base64(INPUT, strlen(INPUT));
    
    puts("");
    printf(BOLDWHITE "Input:        " RESET "%s\n", INPUT);
    printf(BOLDWHITE "Converted:    " RESET "%s\n", converted_base64);
    printf(BOLDWHITE "Expected:     " RESET "%s\n", ANSWER);

    if (strncmp(converted_base64, ANSWER, strlen(ANSWER)) == 0) {
        printf(GREEN "Success!\n\n" RESET);
    } else {
        printf(RED "Failure!\n\n" RESET);
    }

    free(converted_base64);

    return 0;
}