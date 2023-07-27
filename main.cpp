#include <iostream>
#include <cstdint>

// AES S-Box
static const uint8_t sbox[256] = {
    /* 0 */ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    /* 1 */ 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    /* 2 */ 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    /* 3 */ 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    /* 4 */ 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    /* 5 */ 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    /* 6 */ 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    /* 7 */ 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    /* 8 */ 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    /* 9 */ 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    /* A */ 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    /* B */ 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    /* C */ 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    /* D */ 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    /* E */ 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    /* F */ 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// AES Rijndael Rcon
static const uint8_t Rcon[11] = {
    /* 0 */ 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

// AES SubBytes transformation
void aes_sub_bytes(uint8_t* state) {
    for (int i = 0; i < 16; ++i) {
        state[i] = sbox[state[i]];
    }
}

// AES ShiftRows transformation
void aes_shift_rows(uint8_t* state) {
    uint8_t temp[16];

    temp[0] = state[0];
    temp[1] = state[5];
    temp[2] = state[10];
    temp[3] = state[15];

    temp[4] = state[4];
    temp[5] = state[9];
    temp[6] = state[14];
    temp[7] = state[3];

    temp[8] = state[8];
    temp[9] = state[13];
    temp[10] = state[2];
    temp[11] = state[7];

    temp[12] = state[12];
    temp[13] = state[1];
    temp[14] = state[6];
    temp[15] = state[11];

    for (int i = 0; i < 16; ++i) {
        state[i] = temp[i];
    }
}

// AES MixColumns transformation
void aes_mix_columns(uint8_t* state) {
    for (int i = 0; i < 4; ++i) {
        uint8_t s0 = state[i];
        uint8_t s1 = state[i + 4];
        uint8_t s2 = state[i + 8];
        uint8_t s3 = state[i + 12];

        state[i] = (uint8_t)(0x02 * s0) ^ (uint8_t)(0x03 * s1) ^ s2 ^ s3;
        state[i + 4] = s0 ^ (uint8_t)(0x02 * s1) ^ (uint8_t)(0x03 * s2) ^ s3;
        state[i + 8] = s0 ^ s1 ^ (uint8_t)(0x02 * s2) ^ (uint8_t)(0x03 * s3);
        state[i + 12] = (uint8_t)(0x03 * s0) ^ s1 ^ s2 ^ (uint8_t)(0x02 * s3);
    }
}

// AES AddRoundKey transformation
void aes_add_round_key(uint8_t* state, const uint8_t* round_key) {
    for (int i = 0; i < 16; ++i) {
        state[i] ^= round_key[i];
    }
}

// AES Key Expansion
void aes_key_expansion(const uint8_t* key, uint8_t* round_keys) {
    for (int i = 0; i < 16; ++i) {
        round_keys[i] = key[i];
    }

    for (int i = 1; i < 11; ++i) {
        uint8_t temp[4];
        temp[0] = round_keys[4 * i - 4];
        temp[1] = round_keys[4 * i - 3];
        temp[2] = round_keys[4 * i - 2];
        temp[3] = round_keys[4 * i - 1];

        if (i % 4 == 0) {
            uint8_t t = temp[0];
            temp[0] = sbox[temp[1]] ^ Rcon[i];
            temp[1] = sbox[temp[2]];
            temp[2] = sbox[temp[3]];
            temp[3] = sbox[t];
        }

        round_keys[4 * i + 0] = round_keys[4 * i - 16] ^ temp[0];
        round_keys[4 * i + 1] = round_keys[4 * i - 15] ^ temp[1];
        round_keys[4 * i + 2] = round_keys[4 * i - 14] ^ temp[2];
        round_keys[4 * i + 3] = round_keys[4 * i - 13] ^ temp[3];
    }
}

// AES Encryption
void aes_encrypt(uint8_t* state, const uint8_t* key) {
    uint8_t round_keys[176];
    aes_key_expansion(key, round_keys);

    aes_add_round_key(state, key);

    for (int round = 1; round < 10; ++round) {
        aes_sub_bytes(state);
        aes_shift_rows(state);
        aes_mix_columns(state);
        aes_add_round_key(state, round_keys + (round * 16));
    }

    aes_sub_bytes(state);
    aes_shift_rows(state);
    aes_add_round_key(state, round_keys + 160);
}

int main() {
    // Example AES-128 key (16 bytes)
    const uint8_t key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };

    // Example plaintext (16 bytes)
    uint8_t plaintext[16] = {
        0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37,
        0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34
    };

    // Print the original plaintext
    std::cout << "Original Plaintext: ";
    for (int i = 0; i < 16; ++i) {
        std::cout << std::hex << static_cast<int>(plaintext[i]) << " ";
    }
    std::cout << std::endl;

    // Encrypt the plaintext using AES-128
    aes_encrypt(plaintext, key);

    // Print the encrypted ciphertext
    std::cout << "Encrypted Ciphertext: ";
    for (int i = 0; i < 16; ++i) {
        std::cout << std::hex << static_cast<int>(plaintext[i]) << " ";
    }
    std::cout << std::endl;

    return 0;
}