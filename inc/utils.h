
#ifndef VESC_UTILS_H
#define VESC_UTILS_H

#include <vector>
#include <numeric>
#include <cstddef>

namespace {
    using std::vector;
    using std::iota;
}

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;

namespace utils
{
    template<class T>
    auto castu8(T data){return static_cast<uint8_t>(data);}
    template<class T>
    auto castu16(T data){return static_cast<uint16_t>(data);}
    template<class T>
    auto castu32(T data){return static_cast<uint32_t>(data);}
    template<class T>
    auto castdouble(T data){return static_cast<double>(data);}

};

#endif //VESC_UTILS_H
