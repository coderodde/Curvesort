#include "curvesort.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using std::boolalpha;
using std::cout;
using std::endl;
using std::vector;

class CurrentTime {
    std::chrono::high_resolution_clock m_clock;
    
public:
    
    uint64_t milliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>
            (m_clock.now().time_since_epoch()).count();
    }
};

static const int64_t amplitude = 1000;
static const size_t phase_length = 100 * 1000;
static const size_t length = 50 * 1000 * 1000;

vector<int64_t> get_smooth_integer_vector(const size_t length,
                                          const int64_t amplitude,
                                          const size_t phase_length)
{
    vector<int64_t> ret;
    ret.reserve(length);
    
    for (size_t i = 0; i != length; ++i)
    {
        ret.push_back((int64_t) amplitude * sin(2.0 * M_PI * i / phase_length));
    }
    
    return ret;
}


int main() {
    CurrentTime ct;
    
    vector<int64_t> vec = get_smooth_integer_vector(length,
                                                    amplitude,
                                                    phase_length);
    vector<int64_t> vec2(vec);
    
    uint64_t start = ct.milliseconds();
    net::coderodde::sorting::sort(vec.begin(), vec.end());
    uint64_t end = ct.milliseconds();
    
    cout << "Curvesort in " << (end - start) << " milliseconds." << endl;
    
    start = ct.milliseconds();
    std::sort(vec2.begin(), vec2.end());
    end = ct.milliseconds();
    
    cout << "std::sort in " << (end - start) << " millisecond." << endl;
    /*
    cout << "Algorithms agree: " << boolalpha << equal(vec.begin(),
                                                       vec.end(),
                                                       vec2.begin(),
                                                       vec2.end()) << endl;*/
}
