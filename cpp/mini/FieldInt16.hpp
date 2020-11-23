/*
 * Mini field
 */

#pragma once

#include <cstdint>

uint16_t reciproc(uint16_t value, const uint16_t modulus);

class FieldInt16 {
	/*---- Fields ----*/

	public: uint16_t value;
	
	/*---- Constructors ----*/
	
	public: explicit FieldInt16(const uint16_t val);
	
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

    public: void power(uint16_t y);

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

    public: bool operator<(const uint16_t other) const;

    public: bool operator!=(const uint16_t other) const;
    /*---- Class constants ----*/

	private: static const uint16_t MODULUS;  // Prime number
};
