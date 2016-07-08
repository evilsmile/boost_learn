.phony: clean

test:test.cpp
	g++ -g -o $@ $<  -lboost_system
	@echo  "[running]:" && ./test

clean:
	rm -f test
