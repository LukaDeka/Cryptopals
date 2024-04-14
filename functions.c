#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "functions.h"

void* xmalloc(size_t size) {
    if (size == 0){
        fprintf(stderr, "Failed to allocate %zu bytes.\n", size);
        exit(-1);
    }
    return malloc(size);
}

// converts hex characters into binary representation
uint8_t hex_char_to_binary(char input) {
    switch (input) {
        case '0': return 0b0000;
        case '1': return 0b0001;
        case '2': return 0b0010;
        case '3': return 0b0011;
        case '4': return 0b0100;
        case '5': return 0b0101;
        case '6': return 0b0110;
        case '7': return 0b0111;
        case '8': return 0b1000;
        case '9': return 0b1001;
        case 'A': case 'a': return 0b1010;
        case 'B': case 'b': return 0b1011;
        case 'C': case 'c': return 0b1100;
        case 'D': case 'd': return 0b1101;
        case 'E': case 'e': return 0b1110;
        case 'F': case 'f': return 0b1111;
        default: fprintf(stderr, "Error: Invalid input '%c' (%d) for hex_char_to_binary.\n", input, input);
        return 0b0000;
    } 
}

uint8_t* hex_to_binary(char* input, size_t input_len) {

    uint8_t* byte_arr = xmalloc((input_len / 2 + 1) * sizeof(uint8_t));
    size_t in_i = 0, bytes_i = 0;

    if (input_len % 2 == 1) { // odd length, save "0b0000 + the rest"
        byte_arr[bytes_i++] = hex_char_to_binary(input[in_i++]);
    }

    while (in_i < input_len - 1) {
        byte_arr[bytes_i++] = (hex_char_to_binary(input[in_i] ) << 4) | (hex_char_to_binary(input[in_i+1]));
        in_i += 2;
    }

    return byte_arr;
}

char* binary_to_hex(uint8_t* byte_arr, size_t byte_amount) {

    const char hexmap[] = "0123456789abcdef";
    size_t hex_i = 0, byte_i = 0;

    char* hex = xmalloc(byte_amount * 2 * sizeof(char));

    while (byte_i < byte_amount) {
        hex[hex_i++] = hexmap[(byte_arr[byte_i] & 0b11110000) >> 4];
        hex[hex_i++] = hexmap[byte_arr[byte_i] & 0b00001111];
        byte_i += 1;
    }

    return hex;
}

char* binary_to_base64(uint8_t* byte_arr, size_t byte_amount) {

    const char base64map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t base64_i = 0, bytes_i = 0;

    size_t base64size = ceil((byte_amount * 4.0f / 3)); // 3 bytes of binary is 4 characters in base64
    base64size += (4 - base64size % 4) % 4; // add size so it's divisible by 4 (memory later used for padding)
    char* base64 = xmalloc((base64size + 1) * sizeof(char));

    while (byte_amount > 2) {
        uint8_t b1 = byte_arr[bytes_i++], b2 = byte_arr[bytes_i++], b3 = byte_arr[bytes_i++]; //fetch next 3 bytes
        base64[base64_i++] = base64map[ (b1 & 0b11111100) >> 2 ];
        base64[base64_i++] = base64map[ ((b1 & 0b00000011) << 4) | ((b2 & 0b11110000) >> 4) ];
        base64[base64_i++] = base64map[ ((b2 & 0b00001111) << 2) | ((b3 & 0b11000000) >> 6) ];
        base64[base64_i++] = base64map[ (b3 & 0b00111111) ];

        byte_amount -= 3;
    }

    // if there are two more bytes
    if (byte_amount == 2) {
        uint8_t b1 = byte_arr[bytes_i++], b2 = byte_arr[bytes_i++]; // fetch last 2 bytes
        base64[base64_i++] = base64map[ (b1 & 0b11111100) >> 2 ];
        base64[base64_i++] = base64map[ ((b1 & 0b00000011) << 4) | ((b2 & 0b11110000) >> 4) ];
        base64[base64_i++] = base64map[ ((b2 & 0b00001111) << 2) | 0b00000000 ];
        base64[base64_i++] = '=';

        byte_amount = 0;
    }

    // if there is one more byte
    if (byte_amount == 1) {
        uint8_t b1 = byte_arr[bytes_i++]; // fetch last byte
        base64[base64_i++] = base64map[ (b1 & 0b11111100) >> 2 ];
        base64[base64_i++] = base64map[ ((b1 & 0b00000011) << 4) | 0b00000000 ];
        base64[base64_i++] = '=';
        base64[base64_i++] = '=';

        byte_amount = 0;
    }

    return base64;
}

char* hex_to_base64(char* input, size_t input_len) {

    // convert hex into binary
    uint8_t* byte_arr = hex_to_binary(input, input_len);

    // convert binary to base64
    char* base64 = binary_to_base64(byte_arr, input_len / 2); 

    free(byte_arr);

    return base64;
}

// returns the XOR of two byte arrays, repeating the second if necessary
uint8_t* multi_xor(uint8_t* byte_arr1, uint8_t* byte_arr2, size_t arr1_byte_amount, size_t arr2_byte_amount) {

    int arr1_i = 0, arr2_i = 0;
    uint8_t* xor = xmalloc(arr1_byte_amount * sizeof(uint8_t));

    while (arr1_i < arr1_byte_amount) {
        xor[arr1_i] = byte_arr1[arr1_i] ^ byte_arr2[arr2_i];
        arr1_i++; arr2_i++;

        if (arr2_i >= arr2_byte_amount) { arr2_i = 0; }
    }

    return xor;
}

// returns the XOR of a byte array with a single character
uint8_t* single_xor(uint8_t* byte_arr, uint8_t c, size_t byte_amount) {

    uint8_t key_str[2] = {c, '\0'}; // transform char into string for repeating_key_xor()

    return multi_xor(byte_arr, key_str, byte_amount, 1);
}

// lower score is better
double score_plaintext(uint8_t* input, size_t input_len) {

    if (input == NULL) { return HUGE_VAL; }

    FILE* fp = fopen("ascii_frequencies.txt", "r");

    double ascii_frequencies[256] = {0};

    size_t buffer_len = 32;
    char* buffer = malloc(buffer_len * sizeof(char));

    // load frequencies from file
    while (fgets(buffer, buffer_len, fp) != NULL) {

        int index; double value;
        sscanf(buffer, "%d:%lf", &index, &value );

        ascii_frequencies[index] = value;
    }

    free(buffer);
    fclose(fp);

    // count occurrences of each byte
    int count_array[256] = {0};
    for (int i = 0; i < input_len; i++ ) {
        count_array[input[i]]++;
    }

    // compute chi-squared
    double final_score = 0.0;
    for (int i = 0; i < 256; i++) {

        if (count_array[i] == 0 || ascii_frequencies[i] == 0) { continue; } // skip zeroes and non-printable characters

        double difference = (double)count_array[i] / input_len - ascii_frequencies[i];
        final_score += (difference * difference) / ascii_frequencies[i];
    }

    for (int i = 0; i < 256; i++) { // penalty for each non-printable character
        if (ascii_frequencies[i] == 0 && count_array[i] != 0) {
            final_score += count_array[i] * 2; // 2 is an arbitrary value
        }
    }

    // penalty for small lengths (+100 for 1, +25 for 2, +1 for 10, +0.01 for 100...)
    final_score += 100.0f / (input_len * input_len);

    return final_score;
}

uint8_t crack_single_xor(uint8_t* byte_arr, size_t byte_amount, uint8_t** first_choice, uint8_t** second_choice, uint8_t** third_choice) {

    // generate random key choices as placeholders
    *first_choice =  single_xor(byte_arr, 'a', byte_amount);
    *second_choice = single_xor(byte_arr, 'b', byte_amount);
    *third_choice =  single_xor(byte_arr, 'c', byte_amount);

    uint8_t first_guess_key = 'a';
    for (uint8_t key = ' '; key <= '~'; key++) { // key is a printable char

        uint8_t* current_xor = single_xor(byte_arr, key, byte_amount);

        double current_score =  score_plaintext(current_xor, byte_amount);
        double first_score =    score_plaintext(*first_choice, byte_amount);
        double second_score =   score_plaintext(*second_choice, byte_amount);
        double third_score =    score_plaintext(*third_choice, byte_amount);

        // update the scores
        if (current_score < first_score) {
            first_guess_key = key;
            strncpy((char*) *third_choice,   (char*) *second_choice, byte_amount);
            strncpy((char*) *second_choice,  (char*) *first_choice, byte_amount);
            strncpy((char*) *first_choice,   (char*) current_xor, byte_amount);
        } else if (current_score < second_score) {
            strncpy((char*) *third_choice,   (char*) *second_choice, byte_amount);
            strncpy((char*) *second_choice,  (char*) current_xor, byte_amount);
        } else if (current_score < third_score) {
            strncpy((char*) *third_choice,   (char*) current_xor, byte_amount);
        }

        free(current_xor);
    }
    return first_guess_key;
}
