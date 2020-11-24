/*
 * Mini elliptic curve
 */

#pragma once

#include <cstdint>
#include "FieldInt16.hpp"

class CurvePoint16 final {
	
	/*---- Fields ----*/
	
	public: FieldInt16 x;
	public: FieldInt16 y;
	public: FieldInt16 z;  // The point is normalized iff (z = 1 OR (x,y,z)=(0,1,0))
	
	
	
	/*---- Constructors ----*/
	
	// Constructs a normalized point (z=1) from the given coordinates. Constant-time with respect to the values.
	public: explicit CurvePoint16(const FieldInt16 &x_, const FieldInt16 &y_);


    // Find y for given x
    public: explicit CurvePoint16(const FieldInt16 &x_);

    public: void negate();

	// Constructs the special "point at infinity" (normalized), which is used by ZERO and in multiply().
	private: CurvePoint16();
	
	/*---- Arithmetic methods ----*/
	
	// Adds the given curve point to this point. The resulting state is
	// usually not normalized. Constant-time with respect to both values.
    public: void add(const CurvePoint16 &other);

	// Doubles this curve point. The resulting state is usually
	// not normalized. Constant-time with respect to this value.
    public: void twice();

	// Multiplies this point by the given unsigned integer. The resulting state
	// is usually not normalized. Constant-time with respect to both values.
	public: void multiply(small_type n);
	
	
	// Normalizes the coordinates of this point. Idempotent operation.
	// Constant-time with respect to this value.
	public: void normalize();
	
	
	// Copies the given point into this point if enable is 1, or does nothing if enable is 0.
	// Constant-time with respect to both values and the enable.
	public: void replace(const CurvePoint16 &other, std::uint32_t enable);
	
	
	// Tests whether this point is on the elliptic curve.
	// This point needs to be normalized before the method is called.
	// Zero is considered to be off the curve. Constant-time with respect to this value.
	public: bool isOnCurve() const;
	
	
	// Tests whether this point is equal to the special zero point.
	// This point need not be normalized. Constant-time with respect to this value.
	// This method is equivalent to, but more convenient than:
	// { CurvePoint temp(*this); temp.normalize(); return temp == ZERO; }
	public: bool isZero() const;
	
	
	// Tests whether this point equals the given point in all 3 coordinates. This comparison is
	// meaningful only if both points are normalized. Constant-time with respect to both values.
	public: bool operator==(const CurvePoint16 &other) const;
	
	// Tests whether this point mismatches the given point in any of the 3 coordinates. This comparison
	// is meaningful only if both points are normalized. Constant-time with respect to both values.
	public: bool operator!=(const CurvePoint16 &other) const;
	
	/*---- Static functions ----*/
	
	// Returns a normalized public curve point for the given private exponent key.
	// Requires 0 < privExp < ORDER. Constant-time with respect to the value.
	public: static CurvePoint16 privateExponentToPublicPoint(const small_type &privExp);
	
	
	/*---- Class constants ----*/
	
	public: static const FieldInt16 FI_ZERO;  // These FieldInt16 constants are declared here because they are only needed in this class,
	public: static const FieldInt16 FI_ONE;   // and because of C++'s lack of guarantee of static initialization order.
	public: static const FieldInt16 A;       // Curve equation parameter
	public: static const FieldInt16 B;       // Curve equation parameter
	public: static const small_type ORDER;    // Order of base point, which is a prime number
	public: static const CurvePoint16 G;     // Base point (normalized)
	public: static const CurvePoint16 ZERO;  // Dummy point at infinity (normalized)
	
};
