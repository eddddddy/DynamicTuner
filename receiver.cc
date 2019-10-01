#include <cmath>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>

#include "al.h"
#include "alc.h"

#include "pitch.h"
#include "receiver.h"


const int BUFFER_SIZE = 256;
const int NUM_BUFFERS = 16;
const ALenum FORMAT = AL_FORMAT_MONO16;

struct ReceiverImpl {
	
	ALCdevice* device;
	ALCcontext* context;
	ALuint buffers[NUM_BUFFERS];
	ALuint source;
	
	std::vector<EPitchFreq> freqs;
	std::mutex freqMutex;
	std::thread bufferWorker;
	
	void testError() {
		ALCenum error = alGetError();
		if (error != AL_NO_ERROR) {
			std::cout << "error occurred" << std::endl;
		}
	}
	
	void fillBuffers(int num, ALuint* buffer, float volume = 0.18) {
		
		ALshort data[BUFFER_SIZE];
		std::unique_lock<std::mutex> freqLock(freqMutex);
		
		static long toneOffset = 0;
		for (int b = 0; b < num; b++) {
			for (long i = toneOffset; i < toneOffset + BUFFER_SIZE; i++) {
				double at = 0;
				for (unsigned int j = 0; j < freqs.size(); j++) {
					at += sin(2 * 3.14159265358979 * freqs[j].freq * i / 44100);
				}
				data[i - toneOffset] = at * volume * 32767;
			}
			toneOffset += BUFFER_SIZE;
			alBufferData(buffer[b], FORMAT, data, sizeof(data), 44100);
		}
	}
	
	void cycleBuffers() {
		ALint freeBuffers = 0;
		ALuint freeBuffer[NUM_BUFFERS]{};
		alSourcePlay(source);
		while (true) {
			alGetSourcei(source, AL_BUFFERS_PROCESSED, &freeBuffers);
			
			if (freeBuffers > 0) {
				alSourceUnqueueBuffers(source, freeBuffers, freeBuffer);
				fillBuffers(freeBuffers, freeBuffer);
				alSourceQueueBuffers(source, freeBuffers, freeBuffer);
			}
		}
	}
	
	ReceiverImpl() {
		freqs = std::vector<EPitchFreq>{};
	
		device = alcOpenDevice(NULL);
		context = alcCreateContext(device, NULL);
		alcMakeContextCurrent(context);
	
		alGenBuffers(NUM_BUFFERS, buffers);
		alGenSources(1, &source);
	
		fillBuffers(NUM_BUFFERS, buffers);
		alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
		
		bufferWorker = std::thread(&ReceiverImpl::cycleBuffers, this);
		bufferWorker.detach();
	}
	
};

Receiver::Receiver(): imp{std::make_unique<ReceiverImpl>()} {
	
}

Receiver::~Receiver() {
	alSourceStop(imp->source);
	
	alDeleteSources(1, &(imp->source));
	alDeleteBuffers(8, imp->buffers);
	
	alcMakeContextCurrent(NULL);
	alcDestroyContext(imp->context);
	alcCloseDevice(imp->device);
}

void Receiver::notify(const std::vector<EPitchFreq>& ep) {
	std::unique_lock<std::mutex> freqLock(imp->freqMutex);
	imp->freqs = ep;
}


