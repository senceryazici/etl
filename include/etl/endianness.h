///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_ENDIAN_INCLUDED
#define ETL_ENDIAN_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "enum_type.h"
#include "binary.h"

#if ETL_CPP20_SUPPORTED && ETL_USING_STL
  #include <bit>
#endif

///\defgroup endian endian
/// Constants & utilities for endianess
///\ingroup utilities

#if !((!defined(ETL_ENDIAN_LITTLE) && !defined(ETL_ENDIAN_BIG) && !defined(ETL_ENDIAN_NATIVE)) || ((defined(ETL_ENDIAN_LITTLE) && defined(ETL_ENDIAN_BIG) && defined(ETL_ENDIAN_NATIVE))))
  #error All three of ETL_ENDIAN_LITTLE, ETL_ENDIAN_BIG and ETL_ENDIAN_NATIVE must be defined
#endif

// Have we not already defined all of the macros?
#if !defined(ETL_ENDIAN_LITTLE) || !defined(ETL_ENDIAN_BIG) || !defined(ETL_ENDIAN_NATIVE)
  #undef ETL_ENDIAN_LITTLE
  #undef ETL_ENDIAN_BIG
  #undef ETL_ENDIAN_NATIVE
  // Can we use the C++20 definitions?
  #if ETL_CPP20_SUPPORTED && ETL_USING_STL
    #define ETL_ENDIAN_LITTLE std::endian::little
    #define ETL_ENDIAN_BIG    std::endian::big
    #define ETL_ENDIAN_NATIVE std::endian::native
  // If not can we use the compiler macros?
  #elif defined(__BYTE_ORDER__)
    // Test the two versions of the macro we are likely to see.
    #if defined(__ORDER_LITTLE_ENDIAN__)
      #define ETL_ENDIAN_LITTLE __ORDER_LITTLE_ENDIAN__
      #define ETL_ENDIAN_BIG    __ORDER_BIG_ENDIAN__
      #define ETL_ENDIAN_NATIVE __BYTE_ORDER__
    #elif defined(__LITTLE_ENDIAN__)
      #define ETL_ENDIAN_LITTLE __LITTLE_ENDIAN__
      #define ETL_ENDIAN_BIG    __BIG_ENDIAN__
      #define ETL_ENDIAN_NATIVE __BYTE_ORDER__
    #endif
  #endif
#endif

// If true, then the endianness of the platform can be constexpr.
#define ETL_ENDIANNESS_IS_CONSTEXPR (ETL_CPP11_SUPPORTED && defined(ETL_ENDIAN_NATIVE))

namespace etl
{
  //***************************************************************************
  /// Constants to denote endianness of operations.
  ///\ingroup endian
  //***************************************************************************
  struct endian
  {
#if defined(ETL_ENDIAN_NATIVE)
    enum enum_type
    {
      little  = static_cast<int>(ETL_ENDIAN_LITTLE),
      big     = static_cast<int>(ETL_ENDIAN_BIG),
      native  = static_cast<int>(ETL_ENDIAN_NATIVE),
      unknown = INT_MAX
    };
#else
    enum enum_type
    {
      little,
      big,
      native,
      unknown = native
    };
#endif

    ETL_DECLARE_ENUM_TYPE(endian, int)
    ETL_ENUM_TYPE(little,  "little")
    ETL_ENUM_TYPE(big,     "big")
    ETL_ENUM_TYPE(unknown, "unknown")
    ETL_END_ENUM_TYPE
  };

  //***************************************************************************
  /// Checks the endianness of the platform.
  ///\ingroup endian
  //***************************************************************************
  struct endianness
  {
    etl::endian operator ()() const
    {
      return etl::endian(*this);
    }

#if ETL_ENDIANNESS_IS_CONSTEXPR
    ETL_CONSTEXPR
#endif
    operator etl::endian() const
    {
      return get();
    }

#if ETL_ENDIANNESS_IS_CONSTEXPR
    ETL_CONSTEXPR
#endif
    static etl::endian value()
    {
      return get();
    }

  private:

#if ETL_ENDIANNESS_IS_CONSTEXPR
    ETL_CONSTEXPR static etl::endian get()
    {
      return etl::endian::native;
    }
#else
    static etl::endian get()
    {
      static const uint32_t i = 0xFFFF0000;

      return (*reinterpret_cast<const unsigned char*>(&i) == 0) ? etl::endian::little : etl::endian::big;
    }
#endif
  };

  //***************************************************************************
  inline uint8_t ntoh(const uint8_t network)
  {
    return network;
  }

  //***************************************************************************
  inline uint16_t ntoh(const uint16_t network)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(network);
    }
    else
    {
      return network;
    }
  }

  //***************************************************************************
  inline uint32_t ntoh(const uint32_t network)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(network);
    }
    else
    {
      return network;
    }
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  inline uint64_t ntoh(const uint64_t network)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(network);
    }
    else
    {
      return network;
    }
  }
#endif

  //***************************************************************************
  inline uint8_t hton(const uint8_t host)
  {
    return host;
  }

  //***************************************************************************
  inline uint16_t hton(const uint16_t host)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(host);
    }
    else
    {
      return host;
    }
  }

  //***************************************************************************
  inline uint32_t hton(const uint32_t host)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(host);
    }
    else
    {
      return host;
    }
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  inline uint64_t hton(const uint64_t host)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(host);
    }
    else
    {
      return host;
    }
  }
#endif
}

#endif
