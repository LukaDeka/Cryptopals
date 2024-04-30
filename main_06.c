#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"

#define MIN_KEYSIZE 2 // taken from the challenge description
#define MAX_KEYSIZE 40 // taken from the challenge description

#define ANSWER_KEY "Terminator X: Bring the noise"

int main() {
    // uint8_t test1[] = "this is a test";
    // uint8_t test2[] = "wokka wokka!!!";
    // size_t testout = hamming_distance(test1, test2, 14);
    // printf("The test hamming distance is: %zu\n", testout);

    FILE* fp = fopen("challenge_06.txt", "r");
    
    // TODO: turn reading from file to buffer into a function
    // get file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // read the base64 string
    char* base64_buffer = xmalloc(file_size * sizeof(char));
    int ch;
    size_t i = 0;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch != '\n') {
            base64_buffer[i++] = ch;
        }
    }
    base64_buffer[i] = '\0';
    fclose(fp);

    uint8_t* ciphertext = base64_to_binary(base64_buffer, i);
    free(base64_buffer);

    size_t byte_amount = i * 3.0f / 4 - 1;
    uint8_t* key = break_vigenere(ciphertext, byte_amount, MIN_KEYSIZE, MAX_KEYSIZE);
    uint8_t* plaintext = multi_xor(ciphertext, key, byte_amount, strlen((char*)key));

    char print_buffer[byte_amount];
    snprintf(print_buffer, byte_amount, "%s", plaintext);
    puts("");
    print_str("Plaintext:\n", print_buffer);
    print_str("Key: ", (char*)key);

    if (strncmp((char*)key, ANSWER_KEY, strlen(ANSWER_KEY)) == 0) {
        printf(GREEN "Success!\n\n" RESET);
    } else {
        printf(RED "Failure!\n\n" RESET);
    }

    free(ciphertext); free(key); free(plaintext);
    return 0;
}
