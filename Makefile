.PHONY: all
all: generator primeCounter

clean:
	-rm randomGeneratorA primeCounterA randomGenerator primeCounter 2>/dev/null

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primeCounter.c
	gcc -o primeCounter primeCounter.c
	

.PHONY: clean