// UNSW Hyperloop
// Software Screen Task
// Spencer Hanson
// Machine.h

#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

// enumerator list of states
enum State { SAFE, LAUNCH, BRAKE };

// encapsulates state machine functionality
class Machine {
private:
  // state information
  State currState;
  std::chrono::time_point<std::chrono::high_resolution_clock> timeProgStart;
  std::chrono::time_point<std::chrono::high_resolution_clock> timeCurrState;

  // printing vars
  bool inputError;
  bool receivedNumber;
  int mostRecentEnteredNumber;

  // asynchronous container for reading
  std::queue<int> buffer;
  std::mutex mtx;
  std::condition_variable cv;

public:
  // constructor
  Machine();

  // conditions for monitoring input
  int bufferSize();
  bool errorFound();

  // allow for easy threading
  void update();
  void read();
  void print();
};
