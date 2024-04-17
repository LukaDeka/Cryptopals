#include <math.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"

#define ANSWER      "Now that the party is jumping\\n" // answer contains newline
#define ANSWER_LEN  29 // without the newline
#define MAX_LEN     60 // every string (except one) is 60 chars

// Challenge 4: Detect single-character XOR
int main() {

    FILE *file = fopen("challenge_04.txt", "r"); // also try "challenge_04_test.txt"

    uint8_t best_guess[MAX_LEN + 1];
    uint8_t guess_key = ' ';
    double  guess_score = INFINITY; // worst score as placeholder
    size_t  guess_hex_len = 0;
    size_t  guess_line_num = -1, curr_line_num = 1;

    for (;;) {

        // convert newline-terminated line into null-terminated string
        char* curr_hex = NULL;
        size_t buffer_len = MAX_LEN + 1;
        size_t chars_read = getline(&curr_hex, &buffer_len, file); // curr_hex is buffer
        if (chars_read > 0 && curr_hex[chars_read - 1] == '\n') {
            curr_hex[chars_read - 1] = '\0'; // replace '\n' with '\0'
        }
        
        if (chars_read == -1) { 
            free(curr_hex);
            break; 
        }

        uint8_t* byte_arr = hex_to_binary(curr_hex, chars_read - 1);
        uint8_t *curr_guess = NULL;
        uint8_t curr_key = crack_single_xor(byte_arr, chars_read / 2, &curr_guess);
        double curr_score = score_plaintext(curr_guess, chars_read / 2);

        free(byte_arr); free(curr_hex);

        if (curr_score < guess_score) {
            strncpy((char*)best_guess, (char*)curr_guess, MAX_LEN);

            guess_score = curr_score;
            guess_key = curr_key;
            guess_hex_len = chars_read;
            guess_line_num = curr_line_num;
        }

        free(curr_guess);
        curr_line_num++;
    }

    fclose(file);

    // print results
    puts("");
    print_xor   ("Best guess:       ",          best_guess, guess_hex_len / 2);
    print_score ("  With score:     ",          guess_score);
    printf      ("  With key:       '%c'\n",    guess_key);
    printf      ("  On line:        %zu\n",     guess_line_num);
    print_str   ("Expected:         ",          ANSWER);

    if (strncmp((char*)best_guess, ANSWER, ANSWER_LEN) == 0) {
        printf(GREEN "Success!\n\n" RESET);
    } else {
        printf(RED "Failure!\n\n" RESET);
    }

    return 0;
}