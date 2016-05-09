#include <iostream>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>

namespace sc = boost::statechart;

// 先声明初始状态。在定义状态机时使用它，但又在状态机后定义它
struct Greeting;

// 派生类必须将自己作为基类模板的第一个参数。
// 第二个参数Greeting是Machine状态机初始化后进入的第一个状态
struct Machine : sc::state_machine<Machine, Greeting> {};


// 对于每一个状态，我们需要为其指明它属于哪一个状态机，或者说位于状态图的哪个位置。
// 对于这个简单的状态机来说，上下文状态就是状态机。
// 所以Machine作为 simple_state的第二个模板参数。
struct Greeting : sc::simple_state<Greeting, Machine> 
{
	// 一旦状态机进入一个状态的时候，它就要创建一个相应状态类的对象。
	// 只要状态机保持 在这个状态下，这个对象就会一直存在。
	// 最后当状态机离开这个动作时，对象被销毁。
	// 所以一个状态的进入动作就是这个状态类的构造器，而它的退出动作则是它的析构类。
	Greeting() { std::cout << "Hello World!\n"; }
	~Greeting(){ std::cout <<"Bye Bye World!\n"; }
};

int main(int argc, char* argv[])
{
	Machine myMachine;

	//构造完状态机后它并未开始运行，我们要通过调用它的initiate()来启动它。
	// 同时，它也将触发它的初始状态的构造。
	myMachine.initiate();

	//当离开main时，myMachine将被销毁，这将导致它销毁它内部所有活动的状态类。

	return 0;
}

