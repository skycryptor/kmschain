//
// Created by Tigran on 6/23/18.
//

#ifndef CRYPTOMAIC_BIGNUMBER_H
#define CRYPTOMAIC_BIGNUMBER_H

#include "memory"
#include "openssl/bn.h"
#include "openssl/ec.h"
#include "Context.h"
#include "ErrorWrapper.h"
#include "BigNumberRaw.h"
#include "Point.h"

using std::shared_ptr;
using std::make_shared;

namespace CryptoMagic {
  class Point;

  /**
   * \brief Generic implementation for BigNumber
   */
  class BigNumber : public ErrorWrapper {
   private:

    /// Cryptographic context for big number operations
    /// NOTE: this class not taking any ownership for this pointer
    Context *context = nullptr;

    /// Shared pointer for raw big number implementation
    /// This is based on specific Crypto backend choosed compile time
    shared_ptr<BigNumberRaw> bn_raw = make_shared<BigNumberRaw>();

    /// keeping zero bignum initiated and allocated for later usage
    /// this will be created on first BigNumber constructor work at any time
    /// then just we will be checking if it's created or not
    static BIGNUM *BNZero;

    /**
     * \brief Checking if number is in current EC group
     * @return
     */
    bool isFromECGroup() const;

   public:
    /**
     * \brief Making BigNumber object from given raw big number and context
     * @param bn
     * @param ctx
     */
    BigNumber(BIGNUM *bn, Context *ctx);
    explicit BigNumber(Context *ctx) : BigNumber(nullptr, ctx) {}

    /**
     * \brief Copying BigNumber object from existing one
     * @param bn
     */
    BigNumber(const BigNumber& bn);
    virtual ~BigNumber() = default;

    /**
     * \brief Generate random BigNumber from given crypto context
     * @param ctx
     * @return
     */
    static BigNumber generate_random(Context *ctx);

    /**
     * \brief Get BigNumber from integer
     * @param num
     * @param ctx
     * @return
     */
    static BigNumber from_integer(unsigned long num, Context *ctx);

    /**
     * \brief Get BigNumber from big endian ordered bytes
     * @param buffer
     * @param len
     * @param ctx
     * @return
     */
    static BigNumber from_bytes(unsigned char *buffer, int len, Context *ctx);

    /**
     * \brief Getting BigNumber as a HEX string/byte array
     * @return
     */
    string toHex() const;

    /**
     * \brief Convert BigNumber to Point
     * @return
     */
    Point toPoint() const;

    /**
     * \brief Getting BIGNUM bytes from existing OpenSSL BIGNUM
     * @param[out] result_out Serialized string to fill up.
     * @return nothing.
     */
    void toBytes(string& result_out);

    /**
     * \brief Getting reference to OpenSSL BIGNUM
     * @return
     */
    BIGNUM *getRawBigNum() const;
    /**
     * \brief Getting reference to OpenSSL BN_CTX to make context based operations with it
     * @return
     */
    BN_CTX *getRawBnCtx() const;

    /**
     * \brief Checking if BigNumbers are equal
     * @param other
     * @return
     */
    bool operator==(const BigNumber& other) const;

    /**
     * \brief MUL operator for BigNumber * BigNumber = BigNumber
     * @param other
     * @return
     */
    BigNumber operator*(const BigNumber& other);

    /**
     * \brief MUL operator for BigNumber * Point = Point
     * @param other
     * @return
     */
    Point operator*(const Point& other);

    /**
     * \brief Inverting current BigNumber: ~BigNumber = BigNumber
     * @return
     */
    BigNumber operator~() const;

    /**
     * \brief DIV operator for BigNumbers: BigNumber / BigNumber = BigNumber
     * @param other
     * @return
     */
    BigNumber operator/(const BigNumber& other);

    /**
     * \brief ADD operator implementation: BigNumber + BigNumber = BigNumber
     * @param other
     * @return
     */
    BigNumber operator+(const BigNumber& other);

    /**
     * \brief SUB operator implementation: BigNumber - BigNumber = BigNumber
     * @param other
     * @return
     */
    BigNumber operator-(const BigNumber& other);

    /**
     * \brief MOD operator implementation: BigNumber % BigNumber = BigNumber
     * @param other
     * @return
     */
    BigNumber operator%(const BigNumber& other);
  };
}

#endif //CRYPTOMAIC_BIGNUMBER_H
