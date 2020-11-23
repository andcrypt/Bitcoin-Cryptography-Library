/* 
 * Bitcoin cryptography library
 * Copyright (c) Project Nayuki
 * 
 * https://www.nayuki.io/page/bitcoin-cryptography-library
 * https://github.com/nayuki/Bitcoin-Cryptography-Library
 */
#include <cassert>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include "CountOps.hpp"
#include "Ecdsa16.hpp"
#include "FieldInt16.hpp"

using std::uint16_t;

bool Ecdsa16::sign_simple(const uint16_t privateKey, const uint16_t msgHash, const uint16_t nonce, uint16_t &outR, uint16_t &outS) {
    if (nonce==0 || nonce>=CurvePoint16::ORDER) return false;
    CurvePoint16 p = CurvePoint16::G;
    p = CurvePoint16::privateExponentToPublicPoint(nonce);
    if (p.x.value>=CurvePoint16::ORDER && p.x.value<=FieldInt16::MODULUS) return false;
    uint16_t r = p.x.value % CurvePoint16::ORDER;
    if (r == 0) return false;
    outR = r;
    uint16_t s = (uint32_t)reciproc(nonce, CurvePoint16::ORDER) * ((msgHash + (uint32_t)r * privateKey) % CurvePoint16::ORDER) % CurvePoint16::ORDER;
    if (s == 0) return false;
    s = std::min((int)s, CurvePoint16::ORDER - s);
    outS = s;
    return true;
}


bool Ecdsa16::verify_simple(const CurvePoint16 &publicKey, const uint16_t msgHash, const uint16_t r, const uint16_t s) {
    if (publicKey == CurvePoint16::ZERO || !(publicKey.z.value==1) ||
        !(publicKey.x*publicKey.x*publicKey.x+FieldInt16(7)==publicKey.y*publicKey.y))
        return false;
    CurvePoint16 checkzero = publicKey;
    checkzero.multiply(CurvePoint16::ORDER);
    if (checkzero!=CurvePoint16::ZERO) return false;
    if (!(0 < r && r < CurvePoint16::ORDER)) return false;
    if (!(0 < s && s < CurvePoint16::ORDER)) return false;
    uint16_t  w = reciproc(s, CurvePoint16::ORDER) % CurvePoint16::ORDER;
    uint16_t u1 = ((uint32_t)msgHash * w) % CurvePoint16::ORDER;
    uint16_t u2 = ((uint32_t)r * w) % CurvePoint16::ORDER;
    CurvePoint16 p = CurvePoint16::G;
    CurvePoint16 q = publicKey;
    p.multiply(u1);
    q.multiply(u2);
    p.add(q);
    p.normalize();
    return r == p.x.value % CurvePoint16::ORDER;
}

bool Ecdsa16::recovery(const uint16_t msgHash, const uint16_t r, const uint16_t s, CurvePoint16 &publicKeyA, CurvePoint16 &publicKeyB) {
    const uint16_t &order = CurvePoint16::ORDER;
    const uint16_t &zero = 0;
    if (!(zero < r && r < order && zero < s && s < order))
        return false;
    FieldInt16 rf(r);
    CurvePoint16 R1(rf);
    CurvePoint16 R2 = R1;
    R2.negate();
    uint16_t rinv = reciproc(r, CurvePoint16::ORDER);
    uint16_t negmsg;
    negmsg = CurvePoint16::ORDER - msgHash;
    if (msgHash>=CurvePoint16::ORDER)
        negmsg+=CurvePoint16::ORDER;
    uint16_t u1 = ((uint32_t)negmsg * rinv) % CurvePoint16::ORDER;
    uint16_t u2 = ((uint32_t)s * rinv) % CurvePoint16::ORDER;
    CurvePoint16 u1G = CurvePoint16::G;
    u1G.multiply(u1);
    CurvePoint16 u2R1 = R1;
    u2R1.multiply(u2);
    CurvePoint16 u2R2 = R2;
    u2R2.multiply(u2);
    publicKeyA = u1G;
    publicKeyA.add(u2R1);
    publicKeyA.normalize();
    publicKeyB = u1G;
    publicKeyB.add(u2R2);
    publicKeyB.normalize();
    return true;
}

bool Ecdsa16::sign(const uint16_t privateKey, const uint16_t msgHash, const uint16_t nonce, uint16_t &outR, uint16_t &outS) {
    /*
     * Algorithm pseudocode:
     * if (nonce outside range [1, order-1]) return false
     * p = nonce * G
     * r = p.x % order
     * if (r == 0) return false
     * s = nonce^-1 * (msgHash + r * privateKey) % order
     * if (s == 0) return false
     * s = min(s, order - s)
     */
    countOps(functionOps);

    const uint16_t &order = CurvePoint16::ORDER;
    const uint16_t &zero = 0;
    if (nonce == zero || nonce >= order)
        return false;
    countOps(2 * arithmeticOps);

    const CurvePoint16 p = CurvePoint16::privateExponentToPublicPoint(nonce);
    uint16_t r = p.x.value;
    if (r >= order)
        r-= order;
    if (r == zero)
        return false;
    assert(r < order);
    countOps(1 * arithmeticOps);
    countOps(1 * uint16_tCopyOps);
    countOps(1 * curvepointCopyOps);

    uint16_t s = r;
    const uint16_t z = msgHash;
    multiplyModOrder(s, privateKey);
    uint32_t carry = ((uint32_t)s+z)>>16;
    s+=z;
    if (carry | static_cast<uint32_t>(s >= order))
        s-=order;
    countOps(1 * arithmeticOps);
    countOps(2 * uint16_tCopyOps);

    uint16_t kInv = nonce;
    kInv = reciproc(kInv, order);
    multiplyModOrder(s, kInv);
    if (s == zero)
        return false;
    countOps(1 * arithmeticOps);
    countOps(1 * uint16_tCopyOps);

    uint16_t negS = order;
    negS -= s;
    if (static_cast<uint32_t>(negS < s))
        s = negS;  // To ensure low S values for BIP 62
    outR = r;
    outS = s;
    countOps(3 * uint16_tCopyOps);
    return true;
}

void Ecdsa16::multiplyModOrder(uint16_t &x, const uint16_t y) {
    /*
     * Russian peasant multiplication with modular reduction at each step. Algorithm pseudocode:

     */
    uint16_t z = 0;
    for (int i=15; i>=0; i--) {
        z = (z * 2) % CurvePoint16::ORDER;
        if (y & (1<<i))
            z = (z + x) % CurvePoint16::ORDER;
    }
    x = z;
}

bool Ecdsa16::verify(const CurvePoint16 &publicKey, const uint16_t msgHash, const uint16_t r, const uint16_t s) {
    /*
     * Algorithm pseudocode:
     * if (pubKey == zero || !(pubKey is normalized) ||
     *     !(pubKey on curve) || n * pubKey != zero)
     *   return false
     * if (!(0 < r, s < order))
     *   return false
     * w = s^-1 % order
     * u1 = (msgHash * w) % order
     * u2 = (r * w) % order
     * p = u1 * G + u2 * pubKey
     * return r == p.x % order
     */
    countOps(functionOps);
    countOps(11 * arithmeticOps);

    const uint16_t &order = CurvePoint16::ORDER;
    const uint16_t &zero = 0;
    CurvePoint16 q = publicKey;
    q.multiply(CurvePoint16::ORDER);
    if (!(zero < r && r < order && zero < s && s < order))
        return false;
    countOps(5 * arithmeticOps);
    if (publicKey.isZero() || publicKey.z != CurvePoint16::FI_ONE || !publicKey.isOnCurve() || !q.isZero())
        return false;
    countOps(4 * arithmeticOps);
    countOps(1 * curvepointCopyOps);

    uint16_t w = s;
    w = reciproc(w,order);
    const uint16_t z(msgHash);
    uint16_t u1 = w;
    uint16_t u2 = w;
    multiplyModOrder(u1, z);
    multiplyModOrder(u2, r);
    countOps(4 * uint16_tCopyOps);

    CurvePoint16 p = CurvePoint16::G;
    q = publicKey;
    p.multiply(u1);
    q.multiply(u2);
    p.add(q);
    p.normalize();
    countOps(2 * curvepointCopyOps);

    uint16_t px = p.x.value;
    if (static_cast<uint32_t>(px >= order))
    px-= order;
    countOps(1 * uint16_tCopyOps);
    return r == px;
}

