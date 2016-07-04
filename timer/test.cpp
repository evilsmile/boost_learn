#include <iostream>
#include <boost/timer.hpp>
#include <unistd.h>
#include <ctime>

void delay()
{
	int a = 0;
	for (int i = 0; i < 100000000; ++i) {
		a += i;
	}
}
int main()
{
	boost::timer t;
	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	//�ɶ��������ʱ��
	std::cout << "Max timespan: " << t.elapsed_max()/3600 << "h" << std::endl;	
	//�ɶ�������Сʱ��
	std::cout << "Min timespan: " << t.elapsed_min() << "s" << std::endl;	
	//timerʵ����ʹ����CLOCKS_PER_SEC�꣬�����ڲ���ϵͳ�ͱ�����
	std::cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << std::endl;
	//start clock
	std::cout << "start clock: " << std::clock() << std::endl;
	delay();
	//end clock
	std::cout << "end clock: " << std::clock() << std::endl;
	//��������ŵ�ʱ��
	//elapsed() = (end_clock - start_clock) / CLOCKS_PER_SEC
	std::cout << "time elapsed: " << t.elapsed() << "s" << std::endl;	

	return 0;
}
