CXXFLAGS = -g -std=c++17 -Wall -Wextra -Wpedantic -Werror -fsanitize=address

.PHONY: clean

testalloc: testalloc.o allocator.o
	$(CXX) $(CXXFLAGS) -o $@  $^

testalloc.o: testalloc.cpp allocator.h
	$(CXX) $(CXXFLAGS) -c $<

test: testalloc
	ulimit -c unlimited; ASAN_OPTIONS=detect_leaks=1 ./testalloc

clean:
	rm -f *.o testalloc

allocator: allocator.cpp
	   clang++ -o allocator allocator.cpp -std=c++17 -Wall -Wextra -Wpedantic -Werror

