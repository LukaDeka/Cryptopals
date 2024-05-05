#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"

// values taken from the challenge description
#define MIN_KEYSIZE 2
#define MAX_KEYSIZE 40

// only applicable for challenge_06.txt, obviously
#define ANSWER_KEY "Terminator X: Bring the noise"

// challenge 6: Break repeating-key XOR
int main() {
    FILE* fp = fopen("challenge_06.txt", "r"); // also try "challenge_06_test.txt"
    
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

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

    // print the text correctly
    char print_buffer[byte_amount];
    snprintf(print_buffer, byte_amount, "%s", plaintext);
    print_str("\nPlaintext:\n", print_buffer);
    print_str("Key: ", (char*)key);

    if (strncmp((char*)key, ANSWER_KEY, strlen(ANSWER_KEY)) == 0) {
        printf(GREEN "Success!\n\n" RESET);
    } else {
        printf(RED "Failure!\n\n" RESET);
    }

    free(ciphertext); free(key); free(plaintext);
    return 0;
}
