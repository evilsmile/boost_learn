#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
    string s = "123";
    int a = lexical_cast<int>(s);
    double d = lexical_cast<double>(s);

    cout << "a+1=" << a + 1 << endl;
    cout << "d+1=" << d + 1.3f << endl;

    try {
        int c = lexical_cast<int>("wrong number");
    } catch (bad_lexical_cast & e) {
        cerr << e.what() << endl;
    }

	return 0;
}

