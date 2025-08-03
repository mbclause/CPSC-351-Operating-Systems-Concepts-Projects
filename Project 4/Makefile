CXXFLAGS = -g -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread -fsanitize=thread
SRCS = fizzbuzz.cpp \
       producer-consumer.cpp \
       multiple_producers.cpp \
       multiple_consumers.cpp \
       ordering_output.cpp
TARGETS = $(SRCS:.cpp=)

.PHONY: clean

all: $(TARGETS)

$(TARGETS): % : %.o pthread-wrappers.o
	$(CXX) $(CXXFLAGS) -o $@ $^

pthread-wrappers.o: pthread-wrappers.cpp pthread-wrappers.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(TARGETS)
