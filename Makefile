prog: src/main.o src/initialization.o 
	g++ -g -w -o prog src/initialization.o src/main.o

main.o: src/main.cc
	g++ -g -w -Wall -Wextra -Wwrite-strings -c src/main.cc

initialization.o: src/initialization.h src/initialization.cc
	g++ -g -w -Wall -Wextra -Wwrite-strings -c src/initialization.cc
	
clean:
	rm -f prog
	rm -f src/*.o
