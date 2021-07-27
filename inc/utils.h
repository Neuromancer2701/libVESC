
#ifndef VESC_UTILS_H
#define VESC_UTILS_H

#include <vector>
#include <numeric>
namespace {
    using std::vector;
    using std::iota;
}


namespace utils
{

    vector<int> range(int stop, int start = 0 )
    {
        vector<int> x(stop);
        iota(begin(x), end(x), start);
        return x;
    }

};

#endif //VESC_UTILS_H
