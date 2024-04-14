#include <stdio.h>
#include <string.h>
#include "functions.h"

#define INPUT "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"
#define ANSWER "Cooking MC's like a pound of bacon"

int main() {

    size_t hex_len = strlen(INPUT);
    size_t plaintext_len = hex_len / 2;
    uint8_t* byte_arr = hex_to_binary(INPUT, hex_len);

    uint8_t* first_choice   = NULL;
    uint8_t* second_choice  = NULL;
    uint8_t* third_choice   = NULL;

    uint8_t key_of_best_guess = crack_single_xor(byte_arr, plaintext_len, &first_choice, &second_choice, &third_choice);
    
    double first_score = score_plaintext(first_choice, plaintext_len);
    double second_score = score_plaintext(second_choice, plaintext_len);
    double third_score = score_plaintext(third_choice, plaintext_len);


    puts("");
    print_str   ("Encoded hex:      ", INPUT);
    print_xor   ("1st choice:       ", first_choice, plaintext_len);
    print_score ("  With score:     ", first_score);
    printf      ("  With key:       '%c'\n", key_of_best_guess);
    print_xor   ("2nd choice:       ", second_choice, plaintext_len);
    print_score ("  With score:     ", second_score);
    print_xor   ("3rd choice:       ", third_choice, plaintext_len);
    print_score ("  With score:     ", third_score);
    print_str   ("Expected:         ", ANSWER);

    if (strncmp((char*)first_choice, ANSWER, strlen(ANSWER)) == 0) {
        printf(GREEN "Success!\n\n" RESET);
    } else {
        printf(RED "Failure!\n\n" RESET);
    }

    free(first_choice); free(second_choice); free(third_choice); free(byte_arr);

    return 0;
}