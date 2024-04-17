#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char* hex_str;
    uint8_t* byte_arr;

    // cracking repeating char XOR predictions
    uint8_t *first_choice, *second_choice, *third_choice;
    double first_score, second_score, third_score;

    uint8_t key_of_best_guess;
} Xor_t;

// colors for pretty printing
#define RESET       "\033[0m"
#define BOLDWHITE   "\033[1m\033[37m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"

// print a string with non-printable characters as '�'
static inline void print_xor(char* message, uint8_t* string, size_t input_len) {

    printf(BOLDWHITE "%s" RESET, message);

    for (int i = 0; i < input_len; i++) {
        char ch = string[i];
        if (ch >= ' ' && ch <= '~') // ascii char in printable range
            printf("%c", string[i]);
        else if (ch == '\n') // answer contains newline
            printf(BOLDWHITE "\\n" RESET);
        else 
            printf("� ");
    }

    printf("\n");
}

// pretty-print a string with a preceding message
static inline void print_str(char* message, char* string) {
    printf(BOLDWHITE "%s" RESET "%s\n", message, string);
}

// pretty-print a score with a preceding messege
static inline void print_score(char* message, double score) {
    printf("%s%.2f\n", message, score);
}

// malloc that exits on failure
void* xmalloc(size_t size);

// converts hex string into byte array
uint8_t* hex_to_binary(char* input, size_t input_len);

char* binary_to_hex(uint8_t* byte_arr, size_t byte_amount);

char* binary_to_base64(uint8_t* byte_arr, size_t byte_amount);

// uint8_t* base64_to_binary(char* input, size_t input_len);

char* hex_to_base64(char* input, size_t input_len);

// returns the XOR of two byte arrays
uint8_t* xor_byte_arrays(uint8_t* binary1, uint8_t* binary2, size_t byte_amount);

// returns the XOR of two byte arrays, repeating the second if necessary
uint8_t* multi_xor(uint8_t* byte_arr1, uint8_t* byte_arr2, size_t arr1_byte_amount, size_t arr2_byte_amount);

// returns the XOR of a byte array and a char
uint8_t* single_xor(uint8_t* byte_arr, uint8_t c, size_t byte_amount);

// returns chi squared score of byte array (lower score is better)
double score_plaintext(uint8_t* input, size_t input_len);

// finds the best guess for encrypted byte array
// returns the key (char) of best_guess
uint8_t crack_single_xor(uint8_t* byte_arr, size_t byte_amount, uint8_t** best_guess);

// computes the hamming (edit) distance between two strings
// called using pointer arithmetic
size_t hamming_distance(uint8_t* str_1, uint8_t* str2, size_t byte_amount);

#endif