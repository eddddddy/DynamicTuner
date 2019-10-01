#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <memory>

class EPitch;
class Receiver;
struct ControllerImpl;

class Controller {
	// Class that receives add and remove pitch messages from an Input object,
	//   computes new frequencies, and relays those frequencies as messages to
	//   a Receiver object
    private:
        std::unique_ptr<ControllerImpl> imp;

    public:
		// Construct a controller with a default refresh time of 300 milliseconds.
		//   Upon construction, the controller will spawn and detach a thread to
		//   handle internal actions.
        Controller();
		
		// Destroy the controller
		~Controller();

		// Get the refresh time of the controller in milliseconds
        int getRefresh() const;
		
		// Set the refresh time of the controller in milliseconds. The refresh
		//   time represents the time it takes for previous tunings to be dropped
		//   when optimizing tunings for pitches to be played. If a note is played
		//   within the refresh span of other notes, the new note's frequency is
		//   calculated based on the frequencies of the previous notes. Otherwise if
		//   a note is played outside of the refresh span of other notes, the new
		//   note's frequency is calculated only based on itself (and the notes
		//   played within the refresh span of the new note, and so on).
        void setRefresh(int);
		
		// Set the receiver that is to receive messages from the controller.
		//   The receiver's notify(std::vector<EPitchFreq>) method will be called
		//   every time a note is added or removed from the controller, and the
		//   updated vector of calculated pitch frequencies will be passed in as
		//   the parameter.
		void setReceiver(Receiver*);

		// Add the given pitch to the controller. The controller will then
		//   re-calculate the optimal pitch frequencies and notify the receiver
		//   of the changes
        void add(EPitch&);
		
		// Remove the given pitch from the controller. The controller will then
		//   re-calculate the optimal pitch frequencies and notify the receiver
		//   of the changes
        void remove(EPitch&);
};


#endif
