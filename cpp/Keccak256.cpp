/* 
 * Bitcoin cryptography library
 * Copyright (c) Project Nayuki
 * 
 * https://www.nayuki.io/page/bitcoin-cryptography-library
 * https://github.com/nayuki/Bitcoin-Cryptography-Library
 */

#include <cassert>
#include "Keccak256.hpp"

using std::uint8_t;
using std::uint64_t;
using std::size_t;


void Keccak256::getHash(const uint8_t msg[], size_t len, uint8_t hashResult[HASH_LEN]) {
	assert((msg != nullptr || len == 0) && hashResult != nullptr);
	uint64_t state[25] = {};
	
	// XOR each message byte into the state, and absorb full blocks
	int blockOff = 0;
	for (size_t i = 0; i < len; i++) {
		state[blockOff >> 3] ^= static_cast<uint64_t>(msg[i]) << ((blockOff & 7) << 3);
		blockOff++;
		if (blockOff == BLOCK_SIZE) {
			absorb(state);
			blockOff = 0;
		}
	}
	
	// Final block and padding
	state[blockOff >> 3] ^= UINT64_C(0x01) << ((blockOff & 7) << 3);
	blockOff = BLOCK_SIZE - 1;
	state[blockOff >> 3] ^= UINT64_C(0x80) << ((blockOff & 7) << 3);
	absorb(state);
	
	// Uint64 array to bytes in little endian
	for (int i = 0; i < HASH_LEN; i++)
		hashResult[i] = static_cast<uint8_t>(state[i >> 3] >> ((i & 7) << 3));
}


void Keccak256::absorb(uint64_t state[25]) {
	uint64_t *a = state;
	uint8_t r = 1;
	for (int i = 0; i < NUM_ROUNDS; i++) {
		// Theta step
		uint64_t c[5] = {};
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 25; k += 5)
				c[j] ^= a[j + k];
		}
		for (int j = 0; j < 5; j++) {
			uint64_t d = c[(j + 4) % 5] ^ rotl64(c[(j + 1) % 5], 1);
			for (int k = 0; k < 25; k += 5)
				a[j + k] ^= d;
		}
		
		// Rho and pi steps
		uint64_t b[25];
		for (int j = 0; j < 25; j++)
			b[j] = rotl64(a[PERMUTATION[j]], ROTATION[j]);
		
		// Chi step
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 25; k += 5)
				a[j + k] = b[j + k] ^ (~b[(j + 1) % 5 + k] & b[(j + 2) % 5 + k]);
		}
		
		// Iota step
		for (int j = 0; j < 7; j++) {
			a[0] ^= static_cast<uint64_t>(r & 1) << ((1 << j) - 1);
			r = (r << 1) ^ ((r >> 7) * 0x171);
		}
	}
}


uint64_t Keccak256::rotl64(uint64_t x, int i) {
	return ((0U + x) << i) | (x >> ((64 - i) & 63));
}


// Static initializers
const unsigned char Keccak256::PERMUTATION[25] = {
	 0,  6, 12, 18, 24,
	 3,  9, 10, 16, 22,
	 1,  7, 13, 19, 20,
	 4,  5, 11, 17, 23,
	 2,  8, 14, 15, 21,
};
const unsigned char Keccak256::ROTATION[25] = {
	 0, 44, 43, 21, 14,
	28, 20,  3, 45, 61,
	 1,  6, 25,  8, 18,
	27, 36, 10, 15, 56,
	62, 55, 39, 41,  2,
};
