#ifndef __PROXYLIB_POINT_H__
#define __PROXYLIB_POINT_H__

#include <memory>
#include <vector>
#include "ErrorWrapper.h"
#include <mbedtls/ecp.h>
#include "defines.h"

namespace SkyCryptor {

class BigNumber;

/**
 * \brief Elliptic curve Point class implementation based on OpenSSL EC_POINT interface
 */
class Point: public ErrorWrapper {
friend class BigNumber;

public:

  /**
   * \brief Making Point object out of given raw point, rakes ownership of the passed pointer. 
   * NOTE: raw could be NULL, and then defined later on
   * @param point
   * @param ctx
   */
  Point(EC_POINT* point);

  Point();

  /**
   * \brief Copying existing point
   * @param p
   */
  Point(const Point& p);

  virtual ~Point();

  /**
   * \brief Getting Generator Point based on Elliptic curve.
   * @param ctx
   * @return
   */
  static Point get_generator();

  /**
   * \brief Converting serialized bytes to Point object
   * NOTE: Serialization is done using Point -> Hex conversion.
   * @param bytes
   * @return
   */
  static Point from_bytes(const std::vector<char>& bytes);
  static Point from_bytes(const char *bytes, int len);

  /**
   * \brief Generating random point for context based Elliptic curve
   * @param ctx
   * @return
   */
  static Point generate_random();

  /**
   * \brief Hashing our Point object as a BigNumber
   * @param points std::vector of points to be hashed
   * @param ...
   * @return
   */
  // TODO(martun): move this out of this class, move to some hasher class.
  static std::vector<char> hash(const std::vector<Point>& points);

  /**
   * \brief Getting bytes from our Point object
   * @return
   */
  std::vector<char> to_bytes() const;

  /**
   * \brief Equality operator for Point == Point
   * @param other
   * @return
   */
  bool operator==(const Point& other) const;

  /**
   * \brief MUL Operator for Point * BigNumber = Point
   * @param other
   * @return
   */
  Point operator*(const BigNumber& other) const;

  /**
   * \brief ADD Operator for Point + Point = Point
   * @param other
   * @return
   */
  Point operator+(const Point& other) const;

private:
  // Raw pointer for OpenSSL object
  EC_POINT *ec_point_;

};

} // namespace SkyCryptor

#endif // __PROXYLIB_POINT_H__
