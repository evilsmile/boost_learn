progress_timer也是一个计时器，它继承自timer，会在析构时自动输出时间，省去了timer调用elapsed()的工作。是一个用于自动计时的小工具。
progress_timer 继承自timer，因此它的接口与timer相同。但是progress_timer的构造函数progress_timer(std::ostream& os)，它允许将析构时的输出定向到指定的IO流，默认是std::cout。如果有特别的需求，可以用其它标准库输出流替换，或者用cout.rdbuf()重定向cout的输出。

