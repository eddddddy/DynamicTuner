#include <windows.h>
#include <iostream>
#include <vector>

#include "pitch.h"
#include "controller.h"
#include "input.h"

#define C4Key  65
#define Cs4Key 87
#define D4Key  83
#define Ds4Key 69
#define E4Key  68
#define F4Key  70
#define Fs4Key 84
#define G4Key  71
#define Gs4Key 89
#define A4Key  72
#define As4Key 85
#define B4Key  74
#define C5Key  75
#define Cs5Key 79
#define D5Key  76
#define Ds5Key 80
#define E5Key  186
#define F5Key  222
#define Fs5Key 221
#define G5Key  220

struct Key {
	int code;
	EPitch pitch;
	bool pressed;
};

Input::Input(): b{true} {}

Input::~Input() {}

void Input::setController(Controller* c) {
	controller = c;
}

void Input::run() {
	
	std::vector<Key> keys {
		{C4Key,  EPitch{Pitch::C, 4},  false},
		{Cs4Key, EPitch{Pitch::Cs, 4}, false},
		{D4Key,  EPitch{Pitch::D, 4},  false},
		{Ds4Key, EPitch{Pitch::Ds, 4}, false},
		{E4Key,  EPitch{Pitch::E, 4},  false},
		{F4Key,  EPitch{Pitch::F, 4},  false},
		{Fs4Key, EPitch{Pitch::Fs, 4}, false},
		{G4Key,  EPitch{Pitch::G, 4},  false},
		{Gs4Key, EPitch{Pitch::Gs, 4}, false},
		{A4Key,  EPitch{Pitch::A, 4},  false},
		{As4Key, EPitch{Pitch::As, 4}, false},
		{B4Key,  EPitch{Pitch::B, 4},  false},
		{C5Key,  EPitch{Pitch::C, 5},  false},
		{Cs5Key, EPitch{Pitch::Cs, 5}, false},
		{D5Key,  EPitch{Pitch::D, 5},  false},
		{Ds5Key, EPitch{Pitch::Ds, 5}, false},
		{E5Key,  EPitch{Pitch::E, 5},  false},
		{F5Key,  EPitch{Pitch::F, 5},  false},
		{Fs5Key, EPitch{Pitch::Fs, 5}, false},
		{G5Key,  EPitch{Pitch::G, 5},  false}
	};
	
	while (b) {
		for (Key& key : keys) {
			if (GetAsyncKeyState(key.code) & 0x8000) {
				if (!key.pressed) {
					key.pressed = true;
					controller->add(key.pitch);
				}
			} else {
				if (key.pressed) {
					key.pressed = false;
					controller->remove(key.pitch);
				}
			}
		}
	}
	
}

void Input::stop() {
	b = false;
}

