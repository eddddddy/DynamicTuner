CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -MMD -g

EXEC = tuner
OBJECTS = main.o algo.o frac.o pitch.o interval.o tunings.o hash.o score.o sample.o
FIXED_OBS = algo.o frac.o pitch.o interval.o tunings.o hash.o score.o sample.o
REAL_OBS = algo.o frac.o pitch.o interval.o tunings.o hash.o controller.o input.o receiver.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

fixed: ${FIXED_OBS}
	ar rfs libfixeddatatune.a ${FIXED_OBS}

real: ${REAL_OBS}
	ar rfs librealtimetune.a ${REAL_OBS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
