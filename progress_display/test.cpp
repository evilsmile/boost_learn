#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <boost/progress.hpp>

int main()
{
    std::vector<std::string> v(100);
    std::ofstream fs("./test.txt");

    boost::progress_display pd(v.size());

    std::vector<std::string>::iterator pos;

    for (pos = v.begin(); pos != v.end(); ++pos) {
        fs << *pos << std::endl;
        ++pd;
    }

    return 0;
}
