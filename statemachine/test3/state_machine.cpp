#include <iostream>
#include <ctime>

#include <boost/statechart/transition.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>

namespace sc = boost::statechart;

class EvtStartStop : public sc::event<EvtStartStop>{};
class EvtReset :     public sc::event<EvtReset>{};
class EvtGo :     public sc::event<EvtGo>{};

class MainState;
class StopState;
class RunState;
class TwoState;

class Machine : public sc::state_machine<Machine, MainState>
{
};

class MainState : public sc::simple_state<MainState, Machine, StopState>
{
public:
	typedef sc::transition<EvtReset, MainState> reactReset;
	typedef sc::transition<EvtGo, TwoState> reactGo;
	typedef boost::mpl::list<reactReset, reactGo> reactions;

	MainState(void) {
		std::cout << "Enter MainState" << std::endl;
		mTime = 0;
	}
	~MainState(void) {
		std::cout << "Exit MainState" << std::endl;
	}

	double mTime;
};

class TwoState : public sc::simple_state<TwoState, Machine>
{
public:
	typedef sc::transition<EvtGo, MainState> reactions;
	TwoState(void) {
		std::cout << "Enter TwoState" << std::endl;
	}

	~TwoState(void) {
		std::cout << "Exit TwoState" <<std::endl;
	}
};

class StopState : public sc::simple_state<StopState, MainState>
{
public:
	typedef sc::transition<EvtStartStop, RunState> reactions;
	StopState(void) {
		std::cout << "Enter StopState" << std::endl;
	}
	~StopState(void) {
		std::cout << "Exit StopState" << std::endl;
	}
};

class RunState : public sc::simple_state<RunState, MainState>
{
public:
	typedef sc::transition<EvtStartStop, StopState> reactions;
	RunState(void) {
		std::cout << "Enter RunState" << std::endl;
		mStartTime = 0;
	}
	~RunState(void) {
		std::cout << "Exit RunState" << std::endl;
		context<MainState>().mTime += std::difftime(std::time(0), mStartTime);
	}
	std::time_t mStartTime;
};

int main(int argc, char* argv[])
{
	Machine mc;
	mc.initiate();

	mc.process_event(EvtStartStop());
	mc.process_event(EvtStartStop());
	mc.process_event(EvtReset());
	mc.process_event(EvtGo());
	mc.process_event(EvtGo());

	return 0;
}

