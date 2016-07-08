#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>

struct demo_class {
public:
    int a, b, c;
    demo_class(int x = 1, int y = 2, int z = 3) : a(x), b(y), c(z) {}
};

int main()
{
    boost::pool<> pl(sizeof(int));
    int *p = (int*)pl.malloc();
    if (p == NULL) {
        std::cerr << "pool malloc failed!" << std::endl;
        return -1;
    }
    assert(pl.is_from(p));

    pl.free(p);
    for (int i = 0; i < 100; ++i) {
        pl.ordered_malloc(10);
    }

    boost::object_pool<demo_class> pl_o;
    demo_class *p_d = pl_o.malloc();
    assert(pl_o.is_from(p_d));

    assert(p_d->a != 1 || p_d->b != 2);

    p_d = pl_o.construct(7, 8, 9);
    assert(p_d->a == 7);

    boost::object_pool<std::string> pl_s;
    for (int i = 0; i < 10; ++i) {
        std::string *ps = pl_s.construct("hello");
        std::cout << *ps << std::endl;
    }


    return 0;
}
