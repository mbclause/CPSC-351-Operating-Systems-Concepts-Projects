all: box rebuild

clean:
	rm -f box rebuild

box: box.cpp
	clang++ -g -o box box.cpp -std=c++17 -Wall -Wextra -Wpedantic -Werror

rebuild: rebuild.cpp
	clang++ -g -o rebuild rebuild.cpp -std=c++17 -Wall -Wextra -Wpedantic -Werror
