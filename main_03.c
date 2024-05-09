#include <stdio.h>
#include <string.h>
#include "functions.h"

#define INPUT   "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"
#define ANSWER  "Cooking MC's like a pound of bacon"

// Challenge 3: Single-byte XOR cipher
int main() {
    // convert hex string to byte array
    size_t hex_len = strlen(INPUT);
    size_t plaintext_len = hex_len / 2;
    uint8_t* byte_arr = hex2bin(INPUT, hex_len);

    uint8_t* best_guess;
    uint8_t guess_key = crack_single_xor(byte_arr, plaintext_len, &best_guess);
    
    double guess_score = score_plaintext(best_guess, plaintext_len);

    // print results
    print_str   ("\nEncoded hex:      ", INPUT);
    print_xor   ("Best guess:       ", best_guess, plaintext_len);
    print_score ("  With score:     ", guess_score);
    printf      ("  With key:       '%c'\n", guess_key);
    print_str   ("Expected:         ", ANSWER);

    if (strncmp((char*)best_guess, ANSWER, strlen(ANSWER)) == 0) {
        printf(GREEN "Success!\n\n" RESET);
    } else {
        printf(RED "Failure!\n\n" RESET);
    }

    free(best_guess); free(byte_arr);
    return 0;
}