#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "functions.h"

#define VALUE "1c0111001f010100061a024b53535009181c"
#define KEY "686974207468652062756c6c277320657965"
#define ANSWER "746865206b696420646f6e277420706c6179"

int main() {

    size_t hex_len = strlen(VALUE);
    size_t byte_amount = ceil(hex_len * 1.0f / 2);

    // convert hex to binary
    uint8_t* byte_value = hex_to_binary(VALUE, strlen(VALUE));
    uint8_t* byte_key   = hex_to_binary(KEY, strlen(KEY));

    uint8_t* xor = multi_xor(byte_value, byte_key, byte_amount, byte_amount);

    // convert binary back to hex
    char* converted_xor = binary_to_hex(xor, byte_amount);

    puts("");
    print_str("Value:       ", VALUE);
    print_str("Converted:   ", converted_xor);
    print_str("Expected:    ", ANSWER);

    if (strncmp(converted_xor, ANSWER, strlen(ANSWER)) == 0) {
        printf(GREEN "Success!\n\n" RESET);
    } else {
        printf(RED "Failure!\n\n" RESET);
    }

    free(byte_value); free(byte_key); free(xor); free(converted_xor);

    return 0;
}