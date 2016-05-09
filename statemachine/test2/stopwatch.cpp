// 下面我们要为一个机械秒表建模一个状态机。这样一个秒表通常会有两个按钮。
// *Start/Stop
// *Reset
// 同时有两种状态：
// *Stoped:表针停留在上次停止时的位置：
// 	 o按下Reset按钮，表针回退到0的位置。秒表保持在Stoped状态不变。
// 	 o按下Start/Stop按钮，秒表转到Running状态。
// *Running:表针在移动，并持续显示过去的时间：
//   o按下Reset按钮，表针回退到0的位置，秒表转到停止状态。
//   o按下Start/Stop按钮，转到Stoped状态。

#include <iostream>
#include <ctime>

#include <boost/statechart/transition.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>

namespace sc = boost::statechart;

struct EvStartStop : sc::event<EvStartStop> {};
struct EvReset : sc::event<EvReset> {};

struct Active;

struct IElapsedTime
{
	virtual double ElapsedTime() const = 0;
};

struct StopWatch : sc::state_machine<StopWatch, Active> {
	double ElapsedTime() const
	{
		return state_cast<const IElapsedTime&>().ElapsedTime();
	}
	StopWatch() { std::cout << "====StopWatch!\n"; }
	~StopWatch() { std::cout << "====Out StopWatch!\n"; }
};

struct Stopped;
struct Running;

// 这里的simple_state类模板可以接受4个参数。
// 第3个参数指定内部的初始状态，如果有一个这样的状态的话。
// 在这里，Active有一个内部状态(Stopped)，所以将这个内部状态传给它的基类。
// 第4个参数指定是否保留和保留什么类型历史。
// Active是最外层的状态，因此要把它所属的状态机类传给它
struct Active : sc::simple_state<Active, StopWatch, Stopped> {
public:
	typedef sc::transition<EvReset, Active> reactions;

	Active() : elapsedTime_(0.0) { std::cout << "Active!\n"; }
	double ElapsedTime() const { return elapsedTime_; }
	double &ElapsedTime() { return elapsedTime_; }
	~Active() { std::cout << "Out Active!\n"; }
private:
	double elapsedTime_;
};

// Stopped和Running都把Active作为它们的上下文，这使它们嵌入到Active状态中。
struct Running : IElapsedTime, sc::simple_state<Running, Active> {
public:
	typedef sc::transition<EvStartStop, Stopped> reactions;

	Running() : startTime_(std::time(0)){ std::cout << "Running!\n"; }
	~Running() 
	{ 
		std::cout << "Out Running!\n"; 
		context<Active>().ElapsedTime() = ElapsedTime();
	}
	virtual double ElapsedTime() const 
	{
		return context<Active>().ElapsedTime() + std::difftime(std::time(0), startTime_);
	}
private:
	std::time_t startTime_;
};

struct Stopped : IElapsedTime, sc::simple_state<Stopped, Active> {
	typedef sc::transition<EvStartStop, Running> reactions;

	virtual double ElapsedTime() const
	{
		return context<Active>().ElapsedTime();
	}
	Stopped() { std::cout << "Stopped!\n"; }
	~Stopped() { std::cout << "Out Stopped!\n"; }
};

// 因为状态的上下文必须是一个完整的类型（不能单单是声明），
// 所以状态机必须在"外层状态"之间先定义。
// 也就是说，我们要先从状态机开始，然后是最外层的状态，然后是其内部的状态，如此反复。
// 我们可以用广度或深度方式，或者以两者混合 的方式来定义。
//
// 一个状态可以定义任意数量的动作。 这就是为什么当多于一个时，
// 不得不将它们放到一个mpl::list<>里
int main(int argc, char* argv[])
{
	StopWatch myWatch;
	myWatch.initiate();
	std::cout <<myWatch.ElapsedTime() << "\n";
	myWatch.process_event(EvStartStop());
	std::cout <<myWatch.ElapsedTime() << "\n";
	myWatch.process_event(EvStartStop());
	std::cout <<myWatch.ElapsedTime() << "\n";
	myWatch.process_event(EvStartStop());
	std::cout <<myWatch.ElapsedTime() << "\n";
	myWatch.process_event(EvReset());
	std::cout <<myWatch.ElapsedTime() << "\n";

	return 0;
}
