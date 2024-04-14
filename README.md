
# Cryptopals challenges (Set 1)
The [Cryptopals challenges](https://www.cryptopals.com) are a series of exercises designed to demonstrate how to break cryptography in the real-world.

My language of choice is C, since I wanted to implement low-level storage mechanisms and type conversions myself. All programs run with no memory leaks (Valgrind).

## Completed Challenges of Set 1:

1.  **Convert Hex to Base64**: Convert a given hex string to base64 encoding.
    
2.  **Fixed XOR**: Take two equal-length buffers and produce their XOR combination.
    
3.  **Single-byte XOR cipher**: Decrypt a hex-encoded string that was encrypted with a single-byte XOR cipher. Find the key and the decrypted message.
    
4. **Detect single-character XOR**: Identify the line among a file containing 327 60-byte hex-encoded strings that has been encrypted using a single-character XOR cipher. Decrypt it to find the message.
    
5.  **Implement repeating-key XOR**: Encrypt a plaintext message using repeating-key XOR encryption with a given key.

## Personal Modifications:

1.  **Enhanced XOR-Cracking Algorithm**: Modified the algorithm for challenges 3 and 4 to output the three most probable decryption guesses, along with their respective scores and keys.
 
2.  **Improved Scoring System**: Implemented a chi-squared algorithm that penalizes non-printable characters and shorter plaintext lengths, enhancing the accuracy of decryption results in challenges 3 and 4.

