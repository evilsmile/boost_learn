#include <iostream>
#include <sstream>
#include <istream>
#include <boost/progress.hpp>
#include <boost/static_assert.hpp>

void delay()
{
	int a = 0;
	for (int i = 0; i < 100000000; ++i) {
		a += i;
	}
}

// progress_timer 精度为2，自定义new_progress_timer可以设置精度
template <int N =2>
class new_progress_timer : public boost::timer
{
public:
    new_progress_timer(std::ostream &os = std::cout) 
        : m_os(os)
    {
        BOOST_STATIC_ASSERT(N >= 0 && N <= 10);
    }
    ~new_progress_timer(void) 
    {
        try {
            std::istream::fmtflags old_flags 
                = m_os.setf( std::istream::fixed, std::istream::floatfield);
            std::streamsize old_prec = m_os.precision(N);

            m_os << elapsed() << " s\n" << std::endl;

            m_os.flags(old_flags);
            m_os.precision(old_prec);
        } catch (...) {
            std::cerr << "Exception" << std::endl;
        }
    }

private:
    std::ostream& m_os;
};

// 精度为2的话直接使用progress_timer
template <>
class new_progress_timer<2> : public boost::progress_timer
{
};

int main()
{
	{
		//要在一开始声明
		boost::progress_timer t;
		delay();
		//析构时输出时间
	}
	{
        std::ostringstream oss;
        {
            //重定向输出
            boost::progress_timer t(oss);
            delay();
        }
		std::cout << "time: " << oss.str() << std::endl;
	}

    {
        // 使用rdbuf修改cout缓存
        std::ostringstream oss;
        std::streambuf *x = std::cout.rdbuf(oss.rdbuf());
        {
            boost::progress_timer t;
            delay();
        }
        std::cout.rdbuf(x);
		std::cout << "redirect: " << oss.str() << std::endl;
    }
    {
        //要在一开始声明
        new_progress_timer<8> t;
		delay();
		//析构时输出时间
	}
	return 0;
}
