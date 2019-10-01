#ifndef _INPUT_H_
#define _INPUT_H_

class Controller;

class Input {
	// Class that listens to input from the user and sends pitch messages to 
	//   a Controller object. Supports subclassing.
	private:
		Controller* controller;
		bool b;
	
	public:
		// Construct an Input object that listens to keyboard input from the user.
		//   The keys and the corresponding pitches they are attached to are as follows:
		//     - A: C-4
		//     - W: Cs-4
		//     - S: D-4
		//     - E: Ds-4
		//     - D: E-4
		//     - F: F-4
		//     - T: Fs-4
		//     - G: G-4
		//     - Y: Gs-4
		//     - H: A-4
		//     - U: As-4
		//     - J: B-4
		//     - K: C-5
		//     - O: Cs-5
		//     - L: D-5
		//     - P: Ds-5
		//     - ;: E-5
		//     - ': F-5
		//     - ]: Fs-5
		//     - \: G-5
		Input();
		
		// Destroy the Input object. Marked virtual to support subclassing.
		virtual ~Input();
		
		// Set the controller that is to receive messages from the input. The
		//   controller's add(EPitch) and remove(EPitch) methods will be called
		//   every time a new note is detected by the input or a note is
		//   removed. The parameter passed in to the methods will be the note that
		//   is added or removed.
		void setController(Controller*);
		
		// Start the Input loop. Internally will call the controller's add and remove
		//   methods.
		virtual void run();
		
		// Stop the Input loop
		virtual void stop();
};


#endif