#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
//#include <boost/unity.hpp>
#include <boost/noncopyable.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/optional.hpp>

#define auto_t BOOST_AUTO

class NonCopy : public boost::noncopyable {

};

std::vector<std::string> func() 
{
    std::vector<std::string> v(10);
    return v;
}

int main()
{
    //1. --------
    NonCopy nc;
   // NonCopy nc2 = nc;  // This leads to error

    //2. --------
    std::vector<int> v(2);
    v[0] = 3;
    v[1] = 23;

    // 自动推断
    auto_t(iter, v.begin());
    for (; iter != v.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
    auto_t(v2, func());
    auto_t(const &d, new double[20]);

    return 0;
}
