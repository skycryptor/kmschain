//
// Created by Tigran on 6/23/18.
//

#include "BigNumber.h"
#include "iostream"
#include <arpa/inet.h>

namespace CryptoMagic {

  BIGNUM * BigNumber::BNZero = nullptr;

  BigNumber::BigNumber(BIGNUM *bn, Context *ctx) {
    this->bignum = bn;
    this->ctx = ctx;

    // Making order out of EC
    ec_order = BN_new();
    bnCtx = BN_CTX_new();
    int res = EC_GROUP_get_order(ctx->get_ec_group(), ec_order, bnCtx);

    // if we got an error during EC order generation
    // then just setting ec_order to null for checking error later on
    if (res != 1) {
      BN_free(ec_order);
      ec_order = nullptr;
    }

    // Making zero static variable
    if (BigNumber::BNZero == nullptr) {
      BigNumber::BNZero = BN_new();
      unsigned int zeroInt = 0;
      zeroInt = htonl(zeroInt);
      BN_bin2bn((const unsigned char*)&zeroInt, 4, BigNumber::BNZero);
    }
  }

  BigNumber::~BigNumber() {
    if (bignum != nullptr) {
      BN_free(bignum);
    }

    if (ec_order != nullptr) {
      BN_free(ec_order);
    }

    if (bnCtx != nullptr) {
      BN_CTX_free(bnCtx);
    }
  }

  BigNumber BigNumber::generate_random(Context *ctx) {
    BigNumber bn(nullptr, ctx);
    bn.bignum = BN_new();
    int res = BN_rand_range(bn.bignum, bn.ec_order);
    if (res != 1) {
      // TODO: make error handling!!
    }

    // if we got big number not inside EC group range let's try again
    if (!bn.isFromECGroup()) {
      return BigNumber::generate_random(ctx);
    }

    return bn;
  }

  BigNumber BigNumber::from_bytes(unsigned char *buffer, Context *ctx) {
    BigNumber bn(nullptr, ctx);
    bn.bignum = BN_new();
    BN_bin2bn((const unsigned char*)&buffer, 4, bn.bignum);
    return bn;
  }

  BigNumber BigNumber::from_integer(int num, Context *ctx) {
    unsigned int beConverted = htonl((unsigned int)num);
    return BigNumber::from_bytes((unsigned char *)&beConverted, ctx);
  }

  bool BigNumber::isFromECGroup() {
    return BN_cmp(bignum, BigNumber::BNZero) ==1 && BN_cmp(bignum, ec_order) == -1;
  }

  string BigNumber::toHex() {
    char *hexStr = BN_bn2hex(bignum);
    string hex = string(hexStr);
    delete hexStr;
    return hex;
  }
  string BigNumber::toBytes() {
    auto binData = new char[BN_num_bytes(bignum)];
    BN_bn2bin(bignum, (unsigned char*) binData);
    auto buffer = string(binData);
    delete[] binData;
    return buffer;
  }

  bool BigNumber::operator==(const BigNumber &rhs) {
    return BN_cmp(bignum, rhs.bignum) == 0;
  }

  BigNumber BigNumber::operator*(const BigNumber &rhs) {
    BigNumber bn(nullptr, ctx);
    bn.bignum = BN_new();
    int res = BN_mod_mul(bn.bignum, bignum, rhs.bignum, ec_order, bnCtx);
    if (res != 1) {
      // TODO: make error handling!!
    }
    return bn;
  }

  BigNumber BigNumber::operator~() {
    BigNumber bn(nullptr, ctx);
    bn.bignum = BN_new();
    BN_mod_inverse(bn.bignum, bignum, ec_order, bnCtx);
    return bn;
  }

  BigNumber BigNumber::operator/(BigNumber &rhs) {
    // inverting and multiplying to get div
    return (*this) * (~rhs);
  }

  BigNumber BigNumber::operator+(const BigNumber &rhs) {
    BigNumber bn(nullptr, ctx);
    bn.bignum = BN_new();
    int res = BN_mod_add(bn.bignum, bignum, rhs.bignum, ec_order, bnCtx);
    if (res != 1) {
      // TODO: make error handling!!
    }
    return bn;
  }

  BigNumber BigNumber::operator-(const BigNumber &rhs) {
    BigNumber bn(nullptr, ctx);
    bn.bignum = BN_new();
    int res = BN_mod_sub(bn.bignum, bignum, rhs.bignum, ec_order, bnCtx);
    if (res != 1) {
      // TODO: make error handling!!
    }
    return bn;
  }

  BigNumber BigNumber::operator%(const BigNumber &rhs) {
    BigNumber bn(nullptr, ctx);
    bn.bignum = BN_new();
    int res = BN_nnmod(bn.bignum, bignum, rhs.bignum, bnCtx);
    if (res != 1) {
      // TODO: make error handling!!
    }
    return BigNumber(nullptr, nullptr);
  }
}