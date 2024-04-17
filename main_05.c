#include <string.h>
#include "functions.h"

#define INPUT   "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal"
#define KEY     "ICE"
#define ANSWER  "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f"

// Challenge 5: Implement repeating-key XOR
int main() {

    size_t input_size = strlen(INPUT);
    size_t key_size = strlen(KEY);

    uint8_t* xor = multi_xor((uint8_t*)INPUT, (uint8_t*)KEY, input_size, key_size);

    char* encrypted_hex = binary_to_hex(xor, input_size);

    // print results
    puts("");
    print_str("Plaintext:\n", INPUT);
    print_str("Key:             ", KEY);
    print_str("Encrypted:       ", encrypted_hex);
    print_str("Expected:        ", ANSWER);

    if (strncmp(encrypted_hex, ANSWER, strlen(ANSWER)) == 0) {
        printf(GREEN "Success!\n\n" RESET);
    } else {
        printf(RED "Failure!\n\n" RESET);
    }

    free(xor); free(encrypted_hex);

    return 0;
}