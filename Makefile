CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ofast -MMD

EXEC = tuner
OBJECTS = main.o algo.o frac.o pitch.o interval.o tunings.o score.o sample.o controller.o input.o receiver.o
FIXED_OBS = algo.o frac.o pitch.o interval.o tunings.o score.o
REAL_OBS = algo.o frac.o pitch.o interval.o tunings.o controller.o input.o receiver.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -pthread -o ${EXEC} -L . -lopenal32

fixed: ${FIXED_OBS}
	ar rfs libfixeddatatune.a ${FIXED_OBS}

real: ${REAL_OBS}
	ar rfs librealtimetune.a ${REAL_OBS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
