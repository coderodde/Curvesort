#include "curvesort.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <list>
#include <vector>

using std::boolalpha;
using std::cout;
using std::endl;
using std::equal;
using std::list;
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

static const int64_t amplitude = 5000;
static const size_t phase_length = 100 * 1000;
static const size_t length = 20 * 1000 * 1000;

template<typename Container>
void sin_populate_container(Container& cont,
                            const size_t length,
                            const int64_t amplitude,
                            const size_t phase_length)
{
    for (size_t i = 0; i != length; ++i)
    {
        cont.push_back(
                    (int64_t) amplitude * sin(2.0 * M_PI * i / phase_length)
                    );
    }
}


int main() {
    CurrentTime ct;
    
    //// std::vector demo ////
    cout << "std::vector demo:" << endl;
    
    vector<int64_t> vec;
    sin_populate_container(vec, length, amplitude, phase_length);
    vector<int64_t> vec2(vec);
    
    uint64_t start = ct.milliseconds();
    net::coderodde::sorting::sort(vec.begin(), vec.end());
    uint64_t end = ct.milliseconds();
    
    cout << "Curvesort in " << (end - start) << " milliseconds." << endl;
    
    start = ct.milliseconds();
    std::sort(vec2.begin(), vec2.end());
    end = ct.milliseconds();
    
    cout << "std::sort in " << (end - start) << " millisecond." << endl;
    
    cout << "Algorithms agree: "
         << boolalpha
         << equal(vec.begin(), vec.end(), vec2.begin()) << endl;
    
    //// std::list demo ////
    cout << "std::list demo:" << endl;
    
    list<int64_t> lst;
    sin_populate_container(lst, length, amplitude, phase_length);
    list<int64_t> lst2(lst);
    
    start = ct.milliseconds();
    net::coderodde::sorting::sort(lst.begin(), lst.end());
    end = ct.milliseconds();
    
    cout << "Curvesort in " << (end - start) << " milliseconds." << endl;
    
    start = ct.milliseconds();
    lst2.sort();
    end = ct.milliseconds();
    
    cout << "std::list.sort in " << (end - start) << " millisecond." << endl;
    
    cout << "Algorithms agree: "
    << boolalpha
    << equal(lst.begin(), lst.end(), lst2.begin()) << endl;
}
