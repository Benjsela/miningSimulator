CFLAGS = -Wall -g
CC = g++





games: games.o strats.o definitions.o engine.o
	$(CC) $(CFLAGS) -o games games.o strats.o definitions.o engine.o 

games.o: games.cpp strats.hpp
	$(CC) $(CFLAGS) -c games.cpp 

engine.o: engine.cpp definitions.hpp strats.hpp engine.hpp 
	$(CC) $(CFLAGS) -c engine.cpp

strats.o: strats.cpp strats.hpp
	$(CC) $(CFLAGS) -c strats.cpp 


definitions.o: definitions.cpp definitions.hpp
	$(CC) $(CFLAGS) -c definitions.cpp 


clean:
	rm -f *.o



