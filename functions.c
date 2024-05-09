#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "functions.h"

void* xmalloc(size_t size) {
    if (size == 0){
        fprintf(stderr, "Failed to allocate %zu bytes.\n", size);
        exit(1);
    }
    return malloc(size);
}

// converts hex characters into binary representation
uint8_t hexchar2bin(char ch) {
    switch (ch) {
        case '0' ... '9': return ch - '0';
        case 'a' ... 'f': return ch - 'a' + 10;
        case 'A' ... 'F': return ch - 'A' + 10;
        default: fprintf(stderr, "Error: Invalid input '%c' (%d) for hex_char_to_binary.\n", ch, ch);
        return 0;
    } 
}

uint8_t* hex2bin(char* hex, size_t hex_len) {
    uint8_t* byte_arr = xmalloc((hex_len / 2 + 1) * sizeof(uint8_t));
    size_t in_i = 0, bytes_i = 0;

    if (hex_len % 2 == 1) { // odd length, save "0b0000 + the rest"
        byte_arr[bytes_i++] = hexchar2bin(hex[in_i++]);
    }

    while (in_i < hex_len - 1) {
        byte_arr[bytes_i++] = (hexchar2bin(hex[in_i] ) << 4) | (hexchar2bin(hex[in_i+1]));
        in_i += 2;
    }

    return byte_arr;
}

char* bin2hex(uint8_t* byte_arr, size_t byte_amount) {
    const char hexmap[] = "0123456789abcdef";
    size_t hex_i = 0, byte_i = 0;

    char* hex = xmalloc((byte_amount * 2 + 1) * sizeof(char));

    while (byte_i < byte_amount) {
        hex[hex_i++] = hexmap[(byte_arr[byte_i] & 0b11110000) >> 4];
        hex[hex_i++] = hexmap[byte_arr[byte_i] & 0b00001111];
        byte_i += 1;
    }

    return hex;
}

char* bin_to_base64(uint8_t* byte_arr, size_t byte_amount) {
    const char base64map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t base64_i = 0, bytes_i = 0;

    size_t base64size = byte_amount * 4.0f / 3; // 3 bytes of binary is 4 characters in base64
    base64size += (4 - base64size % 4) % 4; // add size so it's divisible by 4 (memory later used for padding)
    char* base64 = xmalloc((base64size + 1) * sizeof(char));

    while (byte_amount >= 3) {
        uint8_t b1 = byte_arr[bytes_i++], b2 = byte_arr[bytes_i++], b3 = byte_arr[bytes_i++]; // fetch next 3 bytes
        base64[base64_i++] = base64map[ (b1 & 0b11111100) >> 2 ];
        base64[base64_i++] = base64map[ ((b1 & 0b00000011) << 4) | ((b2 & 0b11110000) >> 4) ];
        base64[base64_i++] = base64map[ ((b2 & 0b00001111) << 2) | ((b3 & 0b11000000) >> 6) ];
        base64[base64_i++] = base64map[ (b3 & 0b00111111) ];

        byte_amount -= 3;
    }

    // convert the last 0-2 bytes and add padding
    if (byte_amount == 2) {
        uint8_t b1 = byte_arr[bytes_i++], b2 = byte_arr[bytes_i++]; // fetch last 2 bytes
        base64[base64_i++] = base64map[ (b1 & 0b11111100) >> 2 ];
        base64[base64_i++] = base64map[ ((b1 & 0b00000011) << 4) | ((b2 & 0b11110000) >> 4) ];
        base64[base64_i++] = base64map[ ((b2 & 0b00001111) << 2) ];
        base64[base64_i++] = '=';
    } else if (byte_amount == 1) {
        uint8_t b1 = byte_arr[bytes_i++]; // fetch last byte
        base64[base64_i++] = base64map[ (b1 & 0b11111100) >> 2 ];
        base64[base64_i++] = base64map[ ((b1 & 0b00000011) << 4) ];
        base64[base64_i++] = '=';
        base64[base64_i++] = '=';
    }

    return base64;
}

uint8_t* base64_to_bin(char* input, size_t input_len) {
    const char base64map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // check last characters for padding bytes
    size_t padding_byte_amount = 0;
    if (input[input_len - 1] == '=') { padding_byte_amount++; }
    if (input[input_len - 2] == '=') { padding_byte_amount++; }
    input_len -= padding_byte_amount; // discard padding bytes

    uint8_t* byte_arr = xmalloc((input_len * 3 / 4 + 10) * sizeof(uint8_t));

    /*  Base64 to binary conversion:
        | c1 | | c2 | | c3 | | c4 |
        123456 123456 123456 123456
        | byte1 || byte2 || byte3 |
    */
    size_t base64_i = 0, bytes_i = 0;
    while (base64_i <= input_len - 3) {
        char c1 = input[base64_i++], c2 = input[base64_i++], c3 = input[base64_i++], c4 = input[base64_i++];
        uint8_t b1 = strchr(base64map, c1) - base64map;
        uint8_t b2 = strchr(base64map, c2) - base64map;
        uint8_t b3 = strchr(base64map, c3) - base64map;
        uint8_t b4 = strchr(base64map, c4) - base64map;
        byte_arr[bytes_i++] = (b1 << 2 | b2 >> 4);
        byte_arr[bytes_i++] = ((b2 & 0b001111) << 4 | b3 >> 2);
        byte_arr[bytes_i++] = ((b3 & 0b000011) << 6 | b4);
    }

    // account for missing bytes/padding
    if (padding_byte_amount == 1) {
        char c1 = input[base64_i++], c2 = input[base64_i++], c3 = input[base64_i++];
        uint8_t b1 = strchr(base64map, c1) - base64map;
        uint8_t b2 = strchr(base64map, c2) - base64map;
        uint8_t b3 = strchr(base64map, c3) - base64map;
        byte_arr[bytes_i++] = (b1 << 2 | b2 >> 4);
        byte_arr[bytes_i++] = ((b2 & 0b001111) << 4 | b3 >> 2);

    } else if (padding_byte_amount == 2) {
        char c1 = input[base64_i++], c2 = input[base64_i++];
        uint8_t b1 = strchr(base64map, c1) - base64map;
        uint8_t b2 = strchr(base64map, c2) - base64map;
        byte_arr[bytes_i++] = (b1 << 2 | b2 >> 4);
    }

    byte_arr[bytes_i] = '\0';

    return byte_arr;
}

char* hex_to_base64(char* input, size_t input_len) {
    // convert hex into binary
    uint8_t* byte_arr = hex2bin(input, input_len);

    // convert binary to base64
    char* base64 = bin_to_base64(byte_arr, input_len / 2); 

    free(byte_arr);

    return base64;
}

// returns the XOR of two byte arrays, repeating the second if necessary
uint8_t* multi_xor(uint8_t* byte_arr1, uint8_t* byte_arr2, size_t arr1_byte_amount, size_t arr2_byte_amount) {
    size_t arr1_i = 0, arr2_i = 0;
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
    uint8_t* xor = xmalloc(byte_amount * sizeof(uint8_t));
    for (size_t i = 0; i < byte_amount; i++) {
        xor[i] = byte_arr[i] ^ c;
    }
    return xor;
}

// lower score is better
double score_plaintext(uint8_t* input, size_t input_len) {
    if (input == NULL) { return INFINITY; }
    FILE* fp = fopen("./etc/ascii_frequencies.txt", "r");

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
    for (size_t i = 0; i < input_len; i++ ) {
        count_array[input[i]]++;
    }

    // compute chi-squared
    double final_score = 0.0;
    for (size_t i = 0; i < 256; i++) {
        if (count_array[i] == 0 || ascii_frequencies[i] == 0) { continue; } // skip zeroes and non-printable chars
        double difference = (double)count_array[i] / input_len - ascii_frequencies[i];
        final_score += (difference * difference) / ascii_frequencies[i];
    }

    // penalty for each non-printable char
    for (size_t i = 0; i < 256; i++) {
        if (ascii_frequencies[i] == 0 && count_array[i] != 0) {
            final_score += count_array[i];      // strings with only non-printable chars -> bad scores
            final_score *= count_array[i] * 2;  // some non-printable chars -> much worse score
        }
    }

    // penalty for small lengths (+100 for 1, +25 for 2, +1 for 10, +0.01 for 100...)
    final_score += 100.0f / (input_len * input_len);

    return final_score;
}

uint8_t crack_single_xor(uint8_t* byte_arr, size_t byte_amount, uint8_t** best_guess) {
    // generate placeholder xor
    *best_guess = single_xor(byte_arr, 'a', byte_amount);
    uint8_t best_guess_key = 'a';

    for (uint8_t key = ' '; key <= '~'; key++) { // key is a printable char
        uint8_t* current_xor = single_xor(byte_arr, key, byte_amount);
        double current_score = score_plaintext(current_xor, byte_amount);
        double first_score   = score_plaintext(*best_guess, byte_amount);

        // update the scores
        if (current_score < first_score) {
            best_guess_key = key;
            strncpy((char*)*best_guess, (char*)current_xor, byte_amount);
        }

        free(current_xor);
    }
    return best_guess_key;
}

// pass byte arrays using pointer arithmetic
size_t hamming_distance(uint8_t* str1, uint8_t* str2, size_t byte_amount) {
    size_t ret = 0;
    for (size_t byte_num = 0; byte_num < byte_amount; byte_num++) {
        uint8_t byte1 = str1[byte_num], byte2 = str2[byte_num]; // fetch bytes
        for (int i = 0; i < 8; i++) { // iterate over bits
            if ((byte1 & 0x1) != (byte2 & 0x1)) { ret++; }
            byte1 = byte1 >> 1;
            byte2 = byte2 >> 1;
        }
    }
    return ret;
}

size_t guess_keysize(uint8_t* byte_arr, size_t byte_amount, size_t min_keysize, size_t max_keysize) {
    size_t guessed_keysize = -1;
    float best_keysize_score = INFINITY; // placeholder value

    for (size_t keysize = min_keysize; keysize <= max_keysize && keysize <= byte_amount; keysize++) {
        float curr_score = 0.0;
        size_t i = 0;
        while (i < byte_amount / keysize - 1) { // split byte array into blocks of keysize and compute hamming distance
            curr_score += hamming_distance(byte_arr + keysize * i, byte_arr + keysize * (i + 1), keysize);
            i++;
        }
        curr_score /= i * keysize; // normalize the score
        
        // update the score
        if (curr_score < best_keysize_score) {
            guessed_keysize = keysize;
            best_keysize_score = curr_score;
        }
    }
    return guessed_keysize;
}

// break repeating-key XOR (vigenere cipher)
// returns key
uint8_t* break_vigenere(uint8_t* ciphertext, size_t text_len, size_t min_keysize, size_t max_keysize) {
    size_t keysize = guess_keysize(ciphertext, text_len, min_keysize, max_keysize);

    // split the ciphertext into blocks of keysize length, like:
    // ABCDEFGHIJKL ->
    // ABC
    // DEF
    // GHI
    // JKL
    //
    // then make a block out of the first byte of every block,
    // a block out of the second byte of every block... - like:
    // ADGJ, BEHK, CFIL
    size_t block_amount = text_len / keysize;
    uint8_t blocks[keysize][block_amount + 1]; // keysize amount of strings, each with strlen block_amount
    for (size_t i = 0; i < block_amount; i++) {
        for (size_t j = 0; j < keysize; j++) {
            blocks[j][i] = ciphertext[i * keysize + j];
        }
    }

    for (size_t i = 0; i < keysize; i++) {
        blocks[i][block_amount] = '\0';
    }

    // break each block using single char xor and store the keys in one string -> final key
    uint8_t* key_string = xmalloc((keysize + 1) * sizeof(uint8_t));
    for (size_t i = 0; i < keysize; i++) {
        uint8_t* decrypted_block; // placeholder
        key_string[i] = crack_single_xor(blocks[i], block_amount, &decrypted_block);
        free(decrypted_block);
    }
    key_string[keysize] = '\0';

    return key_string;
}
