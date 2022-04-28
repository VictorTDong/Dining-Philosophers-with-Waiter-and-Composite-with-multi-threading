TARGET = composite philosopher

all: ${TARGET}

composite: composite.cpp
	g++ composite.cpp -o composite -pthread -O2 -I.

philosopher: philosopher.cpp
	g++ philosopher.cpp -o philosopher -pthread -O2 -I.

.PHONY: clean
clean:
	rm -f *.o ${TARGET}
