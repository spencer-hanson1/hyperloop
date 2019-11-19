# hyperloop
Representation of state machine used to drive the pod. Written in C++.

Functionality:
* Contains three states - SAFE, LAUNCH and, BRAKE
* Asynchronously reads a command input (int) from stdin
* Continuously outputs to stdout at 1 second intervals according to the current state and time elapsed
* State-specific transitions
* Handles invalid input by printing "Error"
