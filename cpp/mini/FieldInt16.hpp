/*
 * Mini field
 */

#pragma once

#include <cstdint>
#include "toydefs.h"

small_type reciproc(small_type value, const small_type modulus);

class FieldInt16 {
	/*---- Fields ----*/

	public: small_type value;
	
	/*---- Constructors ----*/
	
	public: explicit FieldInt16(const small_type val);
	
	/*---- Arithmetic methods ----*/
	
	// Adds the given number into this number, modulo the prime. Constant-time with respect to both values.
	public: void add(const FieldInt16 &other);
	
	
	// Subtracts the given number from this number, modulo the prime. Constant-time with respect to both values.
	public: void subtract(const FieldInt16 &other);
	
	
	// Doubles this number, modulo the prime. Constant-time with respect to this value.
	public: void multiply2();
	
	
	// Squares this number, modulo the prime. Constant-time with respect to this value.
	public: void square();

    // Square root
    public: void sqrt();

    public: void negate();

	// Multiplies the given number into this number, modulo the prime. Constant-time with respect to both values.
	public: void multiply(const FieldInt16 &other);
	
	// Computes the multiplicative inverse of this number with respect to the modulus.
	// If this number is zero, the reciprocal is zero. Constant-time with respect to this value.
	public: void reciprocal();

    public: void power(small_type y);

    public: void replace(const FieldInt16 &other, uint32_t enable);

    public: FieldInt16 operator+ (const FieldInt16 &other) const;

    public: FieldInt16 operator- (const FieldInt16 &other) const;

    public: FieldInt16 operator* (const FieldInt16 &other) const;

        /*---- Equality and inequality operators ----*/

    public: bool operator==(const FieldInt16 &other) const;

    public: bool operator!=(const FieldInt16 &other) const;

    public: bool operator<(const FieldInt16 &other) const;

    public: bool operator<=(const FieldInt16 &other) const;

    public: bool operator>(const FieldInt16 &other) const;

    public: bool operator>=(const FieldInt16 &other) const;

    public: bool operator<(const small_type other) const;

    public: bool operator!=(const small_type other) const;
    /*---- Class constants ----*/

	public: static const small_type MODULUS;  // Prime number
};
