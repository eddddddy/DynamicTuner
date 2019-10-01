#include <iostream>
#include <chrono>
#include <list>
#include <algorithm>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <utility>

#include "pitch.h"
#include "tunings.h"
#include "algo.h"

#include "controller.h"
#include "receiver.h"


using namespace std::chrono;

struct ControllerImpl {
	
	Receiver* receiver;

    Tuning curr;
    Tuning echo;
    std::list<EPitch> currNotes;
	std::vector<EPitchFreq> freqs;
	
	double relFreq;
	
    std::mutex currMutex;
    std::mutex echoMutex;
    std::mutex noteMutex;

    std::list<std::pair<EPitch, milliseconds>> echoQueue;
    std::mutex echoQueueMutex;

    milliseconds refresh;
    std::atomic<milliseconds> nextRefresh;

    std::thread removeWorker;
	
	void computeFreqs() {
		
		std::unique_lock<std::mutex> currLock(currMutex);
		if (curr.isEmpty()) {
			freqs.clear();
			return;
		}
		
		freqs = curr.getEPitchFreqs(relFreq);
	}
	
	void notifyReceiverAdd() {
		
		currMutex.lock();
		if (currNotes.size() == 1) {
			// set the relative frequency to the actual frequency of the note played
			NoteTuning relNoteTuning = *curr.begin();
			relFreq = getNormalFreq(relNoteTuning.pitch) / relNoteTuning.tuning;
		} else {
			// set the relative frequency to one pitch that is not new
			EPitchFreq relPitch = freqs[0];
			for (const NoteTuning& nt : curr) {
				if (nt.pitch == relPitch.pitch) {
					relFreq = relPitch.freq / nt.tuning;
					break;
				}
			}
		}
		currMutex.unlock();
		
		computeFreqs();
		receiver->notify(freqs);
	}
	
	void notifyReceiverRemove() {
		computeFreqs();
		receiver->notify(freqs);
	}

    ///////////////////////////////////////////////////////////////
    // Helpers for Controller::add

    void add(const EPitch& ep) {
		
		currMutex.lock();
		std::unique_lock<std::mutex> echoLock(echoMutex);
		std::unique_lock<std::mutex> noteLock(noteMutex);
		
		currNotes.emplace_back(ep);
		curr = Algo::getBestValues(echo, currNotes)[0];
		
		currMutex.unlock();
		notifyReceiverAdd();
	}

    /////////////////////////////////////////////////////////////////
    // Helpers for Controller::remove

    void refreshWorker() {
        while (true) {
            auto now = high_resolution_clock::now();
            if (now.time_since_epoch() > nextRefresh.load()) {
                updateEcho();
            }
        }
    }

    void removeNote(const EPitch& ep) {
        currMutex.lock();
		noteMutex.lock();
		auto it = std::find(currNotes.begin(), currNotes.end(), ep);
		bool removed = it != currNotes.end();
		NoteTuning removedNote{EPitch{}, Frac{0, 1}};
		if (removed) {
			currNotes.erase(it);
			removedNote = curr.removePitch(ep);
		}
        currMutex.unlock();
		noteMutex.unlock();
        if (removed) {
            auto removeAt = duration_cast<milliseconds>((high_resolution_clock::now() + refresh).time_since_epoch());
            std::unique_lock<std::mutex> echoQueueLock(echoQueueMutex);
			std::unique_lock<std::mutex> echoLock(echoMutex);
            echoQueue.emplace_back(std::pair<EPitch, milliseconds>{ep, removeAt});
			echo.addNoteTuning(removedNote);
            if (echoQueue.size() == 1) {
                nextRefresh.store(removeAt);
            }
			
			notifyReceiverRemove();
        }
    } 

    void updateEcho() {
        std::unique_lock<std::mutex> echoLock(echoMutex);
        std::unique_lock<std::mutex> echoQueueLock(echoQueueMutex);
        std::pair<EPitch, milliseconds> removed = echoQueue.front();

        echoQueue.pop_front();
        echo.removePitch(removed.first);

        if (echoQueue.empty()) {
            nextRefresh.store(duration_cast<milliseconds>(removed.second + milliseconds(600000)));
        } else {
            nextRefresh.store(echoQueue.front().second);
        }
    }
	
	//////////////////////////////////////////////////////////////////

    ControllerImpl(milliseconds ref): refresh{ref} {
        milliseconds ms = duration_cast<milliseconds>((high_resolution_clock::now() + milliseconds(600000)).time_since_epoch());
        nextRefresh = ms;

        removeWorker = std::thread(&ControllerImpl::refreshWorker, this);
        removeWorker.detach();
    }

};


Controller::Controller(): imp{std::make_unique<ControllerImpl>(milliseconds(300))} {}

Controller::~Controller() = default;

int Controller::getRefresh() const {
    return imp->refresh.count();
}

void Controller::setRefresh(int n) {
    imp->refresh = milliseconds(n);
}

void Controller::setReceiver(Receiver* receiver) {
	imp->receiver = receiver;
}

void Controller::add(EPitch& ep) {
    imp->add(ep);
}

void Controller::remove(EPitch& ep) {
    imp->removeNote(ep);
}
