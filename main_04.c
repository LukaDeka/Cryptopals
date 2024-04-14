#include <math.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"

#define MAX_LEN 60 // every string (except one) is 60 chars
#define ANSWER "Now that the party is jumping\\n"
#define ANSWER_LEN 29 // without the newline

// Challenge 4: Detect single-character XOR
int main() {

    FILE *file = fopen("challenge_04.txt", "r"); // also try "test_04.txt"

    uint8_t first_choice[MAX_LEN + 1], second_choice[MAX_LEN + 1], third_choice[MAX_LEN + 1];
    double first_score = HUGE_VAL, second_score = HUGE_VAL, third_score = HUGE_VAL; // worst scores so they get replaced
    uint8_t key_1 = ' ', key_2 = ' ', key_3 = ' ';
    size_t len_hex_1 = 0, len_hex_2 = 0, len_hex_3 = 0;
    size_t curr_line_num = 1, line_num_1 = -1, line_num_2 = -1, line_num_3 = -1;

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
        uint8_t *curr_guess = NULL, *second_guess = NULL, *third_guess = NULL; // other two guesses are ignored in this program since input is large
        uint8_t curr_key = crack_single_xor(byte_arr, chars_read / 2, &curr_guess, &second_guess, &third_guess);
        double curr_score = score_plaintext(curr_guess, chars_read / 2);

        free(byte_arr); free(second_guess); free(third_guess); free(curr_hex);

        if (curr_score < first_score) {
            strncpy((char*)third_choice, (char*)second_choice, MAX_LEN);
            strncpy((char*)second_choice, (char*)first_choice, MAX_LEN);
            strncpy((char*)first_choice, (char*)curr_guess, MAX_LEN);

            third_score = second_score;
            second_score = first_score;
            first_score = curr_score;

            key_3 = key_2;
            key_2 = key_1;
            key_1 = curr_key;

            len_hex_3 = len_hex_2;
            len_hex_2 = len_hex_1;
            len_hex_1 = chars_read;

            line_num_3 = line_num_2;
            line_num_2 = line_num_1;
            line_num_1 = curr_line_num;
        } else if (curr_score < second_score) {
            strncpy((char*)third_choice, (char*)second_choice, MAX_LEN);
            strncpy((char*)second_choice, (char*)curr_guess, MAX_LEN);

            third_score = second_score;
            second_score = curr_score;
            
            key_3 = key_2;
            key_2 = curr_key;
            
            len_hex_3 = len_hex_2;
            len_hex_2 = chars_read;

            line_num_3 = line_num_2;
            line_num_2 = curr_line_num;
        } else if (curr_score < third_score) {
            strncpy((char*)third_choice, (char*)curr_guess, MAX_LEN);

            third_score = curr_score;
            key_3 = curr_key;
            len_hex_3 = chars_read;
            line_num_3 = curr_line_num;
        }

        free(curr_guess);
        curr_line_num++;
    }

    fclose(file);

    puts("");
    print_xor   ("1st choice:       ",          first_choice, len_hex_1 / 2);
    print_score ("  With score:     ",          first_score);
    printf      ("  With key:       '%c'\n",    key_1);
    printf      ("  On line:        %zu\n",     line_num_1);
    print_xor   ("2nd choice:       ",          second_choice, len_hex_2 / 2);
    print_score ("  With score:     ",          second_score);
    printf      ("  With key:       '%c'\n",    key_2);
    printf      ("  On line:        %zu\n",     line_num_2);
    print_xor   ("3rd choice:       ",          third_choice, len_hex_3 / 2);
    print_score ("  With score:     ",          third_score);
    printf      ("  With key:       '%c'\n",    key_3);
    printf      ("  On line:        %zu\n",     line_num_3);
    print_str   ("Expected:         ",          ANSWER);

    if (strncmp((char*)first_choice, ANSWER, ANSWER_LEN) == 0) {
        printf(GREEN "Success!\n\n" RESET);
    } else {
        printf(RED "Failure!\n\n" RESET);
    }


    // free(first_choice); free(second_choice); free(third_choice); free(byte_arr);

    return 0;
}