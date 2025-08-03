
all: thrice stderr foreach

clean:
	rm -f thrice stderr foreach *.txt *.output *.data

thrice: thrice.cpp
	clang++ -o thrice thrice.cpp -std=c++17 -Wall -Wextra -Wpedantic -Werror

stderr: stderr.cpp
	clang++ -o stderr stderr.cpp -std=c++17 -Wall -Wextra -Wpedantic -Werror

foreach: foreach.cpp
	clang++ -o foreach foreach.cpp -std=c++17 -Wall -Wextra -Wpedantic -Werror
