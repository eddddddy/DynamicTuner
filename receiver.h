#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include <vector>
#include <memory>

class EPitchFreq;
struct ReceiverImpl;

class Receiver {
	// Class that receives frequency notifications from a Controller object and
	//   parses them into some form observable by the user (in this case, sound).
	//   Support subclassing.
	private:
		std::unique_ptr<ReceiverImpl> imp;
	
	public:
		// Construct a receiver. The receiver will parse the notifications it
		//   receives and output sound through the system speakers
		Receiver();
		
		// Destroy the receiver. Marked virtual to support subclassing
		virtual ~Receiver();
		
		// Notify the receiver that the frequencies that are currently being
		//   played has changed. This method will be called automatically
		//   by a Controller object with the optimal frequencies
		virtual void notify(const std::vector<EPitchFreq>& ep);
};


#endif