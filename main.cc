#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>

#include "frac.h"
#include "pitch.h"
#include "interval.h"
#include "tunings.h"
#include "algo.h"
#include "score.h"

#include "input.h"
#include "controller.h"
#include "receiver.h"

int main() {
	
	// EXAMPLE

    SAMPLE_SONG.calculateFreqs();

    for (auto it = SAMPLE_SONG.nbegin(); it != SAMPLE_SONG.nend(); ++it) {
        std::cout << (*it).pitch << std::endl;
    }
	
	Input input;
	Controller controller;
	Receiver receiver;
	
	input.setController(&controller);
	controller.setReceiver(&receiver);
	controller.setRefresh(400);
	input.run();

}
