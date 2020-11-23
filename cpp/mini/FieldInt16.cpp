#include <cassert>
#include "FieldInt16.hpp"

using std::uint16_t;

FieldInt16::FieldInt16(const uint16_t val) :
		value(val) {
    if (value>=MODULUS)
        value -= MODULUS;
	assert(*this < MODULUS);
}


void FieldInt16::add(const FieldInt16 &other) {
	uint32_t ext = value + other.value;  // Perform addition
	assert((ext >> 17) == 0);
    while (ext>=MODULUS)
        ext -= MODULUS;
    value = ext;
    assert(*this < MODULUS);
}


void FieldInt16::subtract(const FieldInt16 &other) {
	int32_t extended = (int32_t)value - other.value;  // Perform subtraction
    while (extended < 0)
        extended += MODULUS;
    value = extended;
    assert(*this < MODULUS);
}

void FieldInt16::multiply2() {
	uint32_t extended = value << 1;
    while (extended >= MODULUS)
        extended -= MODULUS;
    value = extended;
}


void FieldInt16::square() {
	multiply(*this);
}


void FieldInt16::sqrt() {
    power((MODULUS+1)/4);
}

void FieldInt16::negate() {
    int32_t ext = -value;  // Perform subtraction
    while (ext<0)
        ext += MODULUS;
    value = ext;
    assert(*this < MODULUS);
}

void FieldInt16::multiply(const FieldInt16 &other) {
	uint32_t m = value*other.value;
	value = m % MODULUS;
	assert(*this < MODULUS);
}


uint16_t reciproc(uint16_t value, const uint16_t modulus) {
    // Extended GCD algorithm
    assert(value != modulus && (modulus & 1) == 1 && modulus > 1 && value < modulus);
    uint16_t r0 = value;
    uint16_t r1 = modulus;
    int x0 = 1;
    int x1 = 0;
    while (r1>0) {
        uint16_t r = r0 % r1;
        uint16_t q = (r0 - r) / r1;
        int x = x0 - q * x1;
        x0 = x1;
        x1 = x;
        r0 = r1;
        r1 = r;
    }
    if (x0<0)
        value = x0 + modulus;
    else
        value  = x0;
    return value;
}

void FieldInt16::reciprocal() {
    value = reciproc(value, MODULUS);
}

void FieldInt16::power(uint16_t y) {
    FieldInt16 x = *this;
    FieldInt16 one(1);
    *this = one;
    while (y) {
        if (y & 1) {
            multiply(x);
        }
        x.multiply(x);
        y >>= 1;
    }
}

void FieldInt16::replace(const FieldInt16 &other, uint32_t enable) {
    if (enable)
        value = other.value;
}

FieldInt16 FieldInt16::operator+ (const FieldInt16 &other) const {
    FieldInt16 result(*this);
    result.add(other);
    return result;
}

FieldInt16 FieldInt16::operator- (const FieldInt16 &other) const {
    FieldInt16 result(*this);
    result.subtract(other);
    return result;
}

FieldInt16 FieldInt16::operator* (const FieldInt16 &other) const {
    FieldInt16 result(*this);
    result.multiply(other);
    return result;
}

bool FieldInt16::operator==(const FieldInt16 &other) const {
    return value==other.value;
}

bool FieldInt16::operator!=(const FieldInt16 &other) const {
    return value!=other.value;
}

bool FieldInt16::operator<(const FieldInt16 &other) const {
    return value<other.value;
}

bool FieldInt16::operator<=(const FieldInt16 &other) const {
    return value<=other.value;
}

bool FieldInt16::operator>(const FieldInt16 &other) const {
    return value>other.value;
}

bool FieldInt16::operator>=(const FieldInt16 &other) const {
    return value>=other.value;
}

bool FieldInt16::operator<(const uint16_t other) const {
    return value<other;
}

bool FieldInt16::operator!=(const uint16_t other) const {
    return value!=other;
}


// Static initializers
const uint16_t FieldInt16::MODULUS = 65167;
