#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/make_shared.hpp>

// 稍高级的shared_ptr用法 
class shared {
private:
    boost::shared_ptr<int> _p;
public:
    shared(boost::shared_ptr<int> p) : _p(p) {}
    void print()
    {
        std::cout << "count:" << _p.use_count() 
            << "\tv = " << *_p << std::endl;
    }
};

void print_func(boost::shared_ptr<int> p)
{
    std::cout << "count:" << p.use_count() 
        << "\tv = " << *p << std::endl;
}

class sample {
private:
    class impl;
    boost::shared_ptr<impl> p;
public:
    sample();
    void print();
};

class sample::impl
{
public:
    void print()
    {
        std::cout << "impl print" << std::endl;
    }
};

sample::sample() : p(new impl) {}
void sample::print()
{
    p->print();
}

class abstract {
public:
    virtual void f() = 0;
    virtual void g() = 0;
protected:
    virtual ~abstract() {}
};

class impl_a : public abstract {
public:
    virtual void f() 
    {
        std::cout <<  "f from impl_a" << std::endl;
    }
    virtual void g()
    {
        std::cout << "g from impl_a" << std::endl;
    }
};

boost::shared_ptr<abstract> create() 
{
    return boost::shared_ptr<abstract>(new impl_a);
}

struct posix_file {
    posix_file(const char *filename) 
    {
        std::cout << "open file: " << filename << std::endl;
    }
    ~posix_file() {
        std::cout << "close file" << std::endl;
    }
};

int main()
{
    // -------------- shared_ptr ---------------
    // === basic usage ===
    boost::shared_ptr<int> sp1(new int(10));
    assert(sp1.unique());
    boost::shared_ptr<int> sp2 = sp1;
    assert(sp1 == sp2 && sp1.use_count() == 2);

    *sp2 = 100;
    assert(*sp1 == 100);
    sp1.reset();
    assert(!sp1);

    // === better usage ===
    boost::shared_ptr<int> p_s(new int(100));
    shared s1(p_s), s2(p_s);

    s1.print();
    s2.print();
    *p_s = 20;
    print_func(p_s);

    s1.print();

    // === factory ===
    boost::shared_ptr<std::string> sp = boost::make_shared<std::string>("make_shared");
    boost::shared_ptr<std::vector<int> > spv = boost::make_shared<std::vector<int> >(10, 2);
    std::cout << "size: " << spv->size() << std::endl;

    // === 用于标准容器 ===
    typedef std::vector<boost::shared_ptr<int> > vs;
    vs v(10);
    
    int i = 0;
    for (vs::iterator pos = v.begin(); pos != v.end(); ++pos) {
        (*pos) = boost::make_shared<int>(++i);
        std::cout << *(*pos) << ", ";
    }
    std::cout << std::endl;
    boost::shared_ptr<int> p_s_i = v[9];
    *p_s_i = 180;
    std::cout << *v[9] << std::endl;

    // ==== 桥接模式 ====
    sample s;
    s.print();

    // ==== 工厂模式
    boost::shared_ptr<abstract> p_f = create();
    p_f->f();
    p_f->g();

    // 定制删除器
    boost::shared_ptr<FILE> fp_d(fopen("test.txt", "r"), fclose);


    // -------------- shared_array ----------------
    boost::shared_array<int> sa_a(new int[100]);
    boost::shared_array<int> sa2_a = sa_a;
    sa_a[0] = 10;
    std::cout << "sa2_a[0] is " << sa2_a[0] << std::endl;

    // -------------- scoped_ptr ----------------
    // 声明
    boost::scoped_ptr<int> p(new int);
    if (p) {
        *p = 100;
        std::cout << *p << std::endl;
    }
    // 重置
    p.reset();
    assert(p == 0);

    // bool判断
    if (!p) {
        std::cout << "scoped_ptr == null" << std::endl;
    }

    // 自动调用解析
    boost::scoped_ptr<posix_file> fp(new posix_file("./test.txt"));


    // ---------------- scoped_array -----------------
    int *arr = new int[100];
    boost::scoped_array<int> sa(arr);

    memset(&sa[0], 0, sizeof(int) * 100);
    sa[20] = 20;
    sa[30] = 30;
    sa[10] = sa[20] + sa[30];
    std::cout << "sa[10] = " << sa[10] << std::endl;

    // ---------------- weak_ptr -----------------
    boost::shared_ptr<int> sp_w(new int(10));
    boost::weak_ptr<int> wp(sp_w);
    if (!wp.expired()) {
        boost::shared_ptr<int> sp  = wp.lock();
        *sp = 100;
        std::cout << "wp usage after lock: " << wp.use_count() << std::endl;
    }
    std::cout << "wp usage without lock: " << wp.use_count() << std::endl;
    sp_w.reset();
    std::cout << "wp status after sp reset: usage:" << wp.use_count() 
        << " expired: " << wp.expired() 
        << " lock: " << wp.lock() 
        << std::endl;

    return 0;
}
