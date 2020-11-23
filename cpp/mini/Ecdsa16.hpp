/* 
 * Bitcoin cryptography library
 * Copyright (c) Project Nayuki
 * 
 * https://www.nayuki.io/page/bitcoin-cryptography-library
 * https://github.com/nayuki/Bitcoin-Cryptography-Library
 */

#pragma once

#include "CurvePoint16.hpp"

/* 
 * Performs Ecdsa16 signature generation and verification. Provides just three static functions.
 */
class Ecdsa16 final {
	
	// Computes the signature (deterministically) when given the private key, message hash, and random nonce.
	// Returns true if signing was successful (overwhelming probability), or false if a new nonce must be chosen
	// (vanishing probability). Both privateKey and nonce must be in the range [1, CurvePoint::ORDER).
	// outR and outS will be in the same range too; their values are assigned iff signing is successful.
	// Note: The nonce must be unique, unpredictable, and secret. Otherwise the signature may leak the private key.
	// All successful executions are constant-time with respect to the input values; in order words
	// one successful execution is indistinguishable from another one based on side channel information.
	public: static bool sign_simple(const uint16_t privateKey, const uint16_t msgHash, const uint16_t nonce, uint16_t &outR, uint16_t &outS);
    public: static bool sign(const uint16_t privateKey, const uint16_t msgHash, const uint16_t nonce, uint16_t &outR, uint16_t &outS);
	
	
	// Computes a deterministic nonce based on the HMAC-SHA-256 of the message hash with the private key,
	// and then performs Ecdsa16 signing. Returns true iff signing is successful (with overwhelming probability).
	// This has the same constant-time behavior as sign().
	public: static bool signWithHmacNonce(const uint16_t privateKey, const uint16_t msgHash, uint16_t &outR, uint16_t &outS);
	
	
	// Checks whether the given signature, message, and public key are valid together. The public key point
	// must be normalized. This function does not need to be constant-time because all inputs are public.
	public: static bool verify_simple(const CurvePoint16 &publicKey, const uint16_t msgHash, const uint16_t r, const uint16_t s);
    public: static bool verify(const CurvePoint16 &publicKey, const uint16_t msgHash, const uint16_t r, const uint16_t s);
	
	
	// Computes x = (x * y) % CurvePoint::ORDER. Requires x < CurvePoint::ORDER, but y is unrestricted.
	private: static void multiplyModOrder(uint16_t &x, const uint16_t y);

    public: static bool recovery(const uint16_t msgHash, const uint16_t r, const uint16_t s, CurvePoint16 &publicKeyA, CurvePoint16 &publicKeyB);
	
	Ecdsa16() = delete;  // Not instantiable
};
