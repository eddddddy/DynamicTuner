CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ofast -MMD
EXEC = tuner
OBJECTS = main.o algo.o frac.o pitch.o interval.o tunings.o score.o sample.o controller.o input.o receiver.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -pthread -o ${EXEC} -L . -lopenal32

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
