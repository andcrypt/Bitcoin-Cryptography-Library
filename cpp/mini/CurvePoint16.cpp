/*
 * Mini elliptic curve
 */

#include <cassert>
#include "CountOps.hpp"
#include "CurvePoint16.hpp"

using std::uint16_t;

CurvePoint16::CurvePoint16(const FieldInt16 &x_, const FieldInt16 &y_) :
	x(x_), y(y_), z(FI_ONE) {}


CurvePoint16::CurvePoint16(const FieldInt16 &x_) :
        x(x_), y(FI_ZERO), z(FI_ONE) {
    y = x;
    y.power(3);
    y.add(CurvePoint16::B);
    y.sqrt();
}

void CurvePoint16::negate(){
    y.negate();
}

CurvePoint16::CurvePoint16() :
	x(FI_ZERO), y(FI_ONE), z(FI_ZERO) {}


void CurvePoint16::add(const CurvePoint16 &other) {
    if (*this == ZERO)
        *this = other;
    else if (other == ZERO)
        return;
    else {
        FieldInt16 t0 = y * other.z;
        FieldInt16 t1 = other.y * z;
        FieldInt16 u0 = x * other.z;
        FieldInt16 u1 = other.x * z;
        if (u0 == u1) {  // Same x coordinates
            if (t0 == t1)  // Same y coordinates
                twice();
            else
                replace(ZERO, 1);
        } else {
            FieldInt16 t = t0 - t1;
            FieldInt16 u = u0 - u1;
            FieldInt16 u2 = u*u;
            FieldInt16 v = z * other.z;
            FieldInt16 w = t*t * v - u2 * (u0 + u1);
            x = u * w;
            FieldInt16 u3 = u2 * u;
            y = t * (u0 * u2 - w) - t0 * u3;
            z = u3 * v;
        }
    }
}

void CurvePoint16::twice() {
    if (*this == ZERO || y == FI_ZERO)
        replace(ZERO,1);
    else {
        FieldInt16 fi2(2);
        FieldInt16 fi3(3);
        FieldInt16 t = fi3 * x*x + A * z*z;
        FieldInt16 u = fi2 * y * z;
        FieldInt16 v = fi2 * u * x * y;
        FieldInt16 w = t*t - fi2 * v;
        x = u * w;
        FieldInt16 uy = u * y;
        y = t * (v - w) - fi2 * uy*uy;
        z = u*u*u;
    }
}

void CurvePoint16::multiply(uint16_t n) {
    CurvePoint16 temp = *this;
    CurvePoint16 result = ZERO;
    while (n != 0) {
			if (n & 1)
				result.add(temp);
			temp.twice();
			n >>= 1;
     }
    *this = result;
}


void CurvePoint16::normalize() {
	/*
	 * Algorithm pseudocode:
	 * if (z != 0) {
	 *   x /= z
	 *   y /= z
	 *   z = 1
	 * } else {
	 *   x = x != 0 ? 1 : 0
	 *   y = y != 0 ? 1 : 0
	 *   z = 0
	 * }
	 */
	countOps(functionOps);
	CurvePoint16 norm = *this;
	norm.z.reciprocal();
	norm.x.multiply(norm.z);
	norm.y.multiply(norm.z);
	norm.z = FI_ONE;
	x.replace(FI_ONE, static_cast<uint32_t>(x != FI_ZERO));
	y.replace(FI_ONE, static_cast<uint32_t>(y != FI_ZERO));
	this->replace(norm, static_cast<uint32_t>(z != FI_ZERO));
	countOps(1 * FieldInt16CopyOps);
	countOps(1 * CurvePoint16CopyOps);
}


void CurvePoint16::replace(const CurvePoint16 &other, uint32_t enable) {
	assert((enable >> 1) == 0);
	countOps(functionOps);
	this->x.replace(other.x, enable);
	this->y.replace(other.y, enable);
	this->z.replace(other.z, enable);
}


bool CurvePoint16::isOnCurve() const {
	countOps(functionOps);
	FieldInt16 left = y;
	left.square();
	FieldInt16 right = x;
	right.square();
	right.add(A);
	right.multiply(x);
	right.add(B);
	countOps(2 * arithmeticOps);
	countOps(2 * FieldInt16CopyOps);
	return (left == right) & !isZero();
}


bool CurvePoint16::isZero() const {
	countOps(functionOps);
	countOps(2 * arithmeticOps);
	return (x == FI_ZERO) & (y != FI_ZERO) & (z == FI_ZERO);
}


bool CurvePoint16::operator==(const CurvePoint16 &other) const {
	countOps(functionOps);
	countOps(2 * arithmeticOps);
	return (x == other.x) & (y == other.y) & (z == other.z);
}

bool CurvePoint16::operator!=(const CurvePoint16 &other) const {
	countOps(functionOps);
	countOps(1 * arithmeticOps);
	return !(*this == other);
}

CurvePoint16 CurvePoint16::privateExponentToPublicPoint(const uint16_t &privExp) {
	assert((0 < privExp) & (privExp < CurvePoint16::ORDER));
	CurvePoint16 result = CurvePoint16::G;
	result.multiply(privExp);
	result.normalize();
	return result;
}


// Static initializers
const FieldInt16 CurvePoint16::FI_ZERO(0);
const FieldInt16 CurvePoint16::FI_ONE (1);
const FieldInt16 CurvePoint16::A    (0);
const FieldInt16 CurvePoint16::B    (7);
const uint16_t  CurvePoint16::ORDER(64879);
const CurvePoint16 CurvePoint16::G(
	FieldInt16(62171),
	FieldInt16(14828));
const CurvePoint16 CurvePoint16::ZERO;  // Default constructor
