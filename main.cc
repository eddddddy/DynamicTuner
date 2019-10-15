#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#include "frac.h"
#include "pitch.h"
#include "interval.h"
#include "tunings.h"
#include "hash.h"
#include "algo.h"
#include "score.h"

int main() {
	
	// EXAMPLE

    /*
    Score test = Score{}.add(EPitch{Pitch::C, 4}, 4, 1)
                        .add(EPitch{Pitch::Cs, 4}, 4, 1)
                        .add(EPitch{Pitch::D, 4}, 4, 1)
                        .add(EPitch{Pitch::Ds, 4}, 4, 1);
    test.calculateFreqs();

    for (auto it = test.nbegin(); it != test.nend(); ++it) {
        std::cout << (*it).pitch << std::endl;
    }*/

    Tuning tuning = Tuning{};
    std::list<EPitch> notes = std::list<EPitch>{
        EPitch{Pitch::C, 4},
        EPitch{Pitch::D, 4},
        EPitch{Pitch::Fs, 4},
        EPitch{Pitch::A, 4},
        EPitch{Pitch::As, 4}
    };

    /*
    Tuning tuning = Tuning{}.addNoteTuning(NoteTuning{EPitch{Pitch::C, 3}, Frac{80, 243}})
                            .addNoteTuning(NoteTuning{EPitch{Pitch::F, 3}, Frac{25600, 59049}})
                            .addNoteTuning(NoteTuning{EPitch{Pitch::Gs, 3}, Frac{10000, 19683}})
                            .addNoteTuning(NoteTuning{EPitch{Pitch::B, 3}, Frac{4000, 6561}})
                            .addNoteTuning(NoteTuning{EPitch{Pitch::D, 4}, Frac{3125, 4374}})
                            .addNoteTuning(NoteTuning{EPitch{Pitch::E, 4}, Frac{3125, 3888}})
                            .addNoteTuning(NoteTuning{EPitch{Pitch::F, 4}, Frac{51200, 59049}})
                            .addNoteTuning(NoteTuning{EPitch{Pitch::Cs, 5}, Frac{80000, 59049}})
                            .addNoteTuning(NoteTuning{EPitch{Pitch::D, 5}, Frac{3125, 2187}})
                            .addNoteTuning(NoteTuning{EPitch{Pitch::E, 5}, Frac{3125, 1944}});
    std::list<EPitch> notes = std::list<EPitch> {
        EPitch{Pitch::G, 3},
        EPitch{Pitch::F, 4},
        EPitch{Pitch::D, 5},
        EPitch{Pitch::C, 3},
        EPitch{Pitch::E, 4},
        EPitch{Pitch::F, 3},
        EPitch{Pitch::Gs, 3},
        EPitch{Pitch::B, 3},
        EPitch{Pitch::D, 4},
        EPitch{Pitch::G, 4},
        EPitch{Pitch::Cs, 5},
        EPitch{Pitch::E, 5}
    };*/

    std::map<Tuning, int> m1 = Algo::getValuesSelectiveRec(tuning, notes);
    std::cout << "done selective" << std::endl;
    std::map<Tuning, int> m2 = Algo::getValuesRec(tuning, notes);

    for (auto p : m1) {
        std::cout << p.first << " " << p.second << std::endl;
    }
    std::cout << std::endl;

    for (auto p : m2) {
        std::cout << p.first << " " << p.second << std::endl;
    }
}
