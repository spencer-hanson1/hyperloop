// UNSW Hyperloop
// Software Screen Task
// Spencer Hanson
// Machine.cpp

#include "Machine.h"

using namespace std;

// set current state to SAFE
// set clocks to current time
Machine::Machine() {
  currState = State::SAFE;
  timeProgStart = chrono::high_resolution_clock::now();
  timeCurrState = timeProgStart;
  inputError = false;
  receivedNumber = false;
  mostRecentEnteredNumber = 0;
}

// interface for obtaining buffer size
int Machine::bufferSize() {
  return (int) buffer.size();
}

// interface for error checking
bool Machine::errorFound() {
  return inputError;
}

// updates machine based on value, time, and currState
void Machine::update() {
  // wait until buffer is not empty, then lock
  std::unique_lock<std::mutex> mlock(mtx);
  while (buffer.empty()) {
    cv.wait(mlock);
  }

  // pop integer from the queue
  int enteredNumber = buffer.front();
  buffer.pop();

  // calculate time since current state began
  auto timeSinceCurrState = chrono::duration_cast<std::chrono::seconds>
      (chrono::high_resolution_clock::now() - timeCurrState).count();

  // handle invalid input
  if (enteredNumber < 1 || enteredNumber > 100) {
    cout << "Error\n";
  }

  // SAFE
  else if (currState == State::SAFE) {
    // transition to LAUNCH
    if (enteredNumber == 1) {
      currState = State::LAUNCH;
      timeCurrState = chrono::high_resolution_clock::now();
    }

    // number entered in wrong state
    else {
      cout << "Error\n";
    }
  }

  // LAUNCH
  else if (currState == State::LAUNCH) {
    // emergency transition to BRAKE
    if (enteredNumber == 2) {
      cout << "Emergency!\n";
      receivedNumber = false;
      currState = State::BRAKE;
      timeCurrState = chrono::high_resolution_clock::now();
    }

    // remain in LAUNCH
    // print enteredNumber multiplied by timeSinceCurrState
    else if (enteredNumber >= 3 && enteredNumber <= 100) {
      receivedNumber = true;
      mostRecentEnteredNumber = enteredNumber;
    }

    // number entered in wrong state
    else {
      cout << "Error\n";
    }
  }

  // BRAKE
  else if (currState == State::BRAKE) {
    // transition to SAFE
    if (enteredNumber == 99 && timeSinceCurrState > 5) {
      cout << "Brooklyn\n";
      currState = State::SAFE;
    }

    // number entered in wrong state
    else {
      cout << "Error\n";
    }
  }

  // unlock and notify a waiting thread
  mlock.unlock();
  cv.notify_one();
  return;
}

// read integers from stdin
void Machine::read() {
  // read numbers until input error
  int enteredNumber = 0;
  while (!inputError) {
    // lock, push to buffer, unlock
    if (cin >> enteredNumber) {
      mtx.lock();
      buffer.push(enteredNumber);
      mtx.unlock();
    }

    // handle invalid integer input
    else {
      cout << "Error\n";
      receivedNumber = false;
      inputError = true;
      break;
    }
  }

  return;
}

// prints to stdout
void Machine::print() {
  // time point of last print
  auto timeLastPrint = timeProgStart;

  // continue printing until an error is encountered
  while (!inputError) {
    // check time since last print
    // continue waiting if less than one second
    if (chrono::duration_cast<std::chrono::milliseconds>
      (chrono::high_resolution_clock::now() - timeLastPrint).count() < 1000) {
      continue;
    }

    // record timePrint, then lock
    timeLastPrint = chrono::high_resolution_clock::now();
    mtx.lock();

    // transition to BRAKE
    // special case - independent of enteredNumber
    if (currState == State::LAUNCH && chrono::duration_cast<std::chrono::seconds>
      (chrono::high_resolution_clock::now() - timeCurrState).count() > 15) {
      receivedNumber = false;
      currState = State::BRAKE;
      timeCurrState = chrono::high_resolution_clock::now();
    }

    // print current state
    cout << "-----------------------------------------\nCurrent State: ";

    switch (currState) {
  		case (State::SAFE):
  			cout << "SAFE\n";
  			break;

      case (State::LAUNCH):
    		cout << "LAUNCH\n";
    		break;

      case (State::BRAKE):
    		cout << "BRAKE\n";
    		break;
    }

    // print time elapsed since program start and state change
    cout << "Time elapsed since program start: "
         << chrono::duration_cast<std::chrono::seconds>
         (chrono::high_resolution_clock::now() - timeProgStart).count()
         << "\nTime elapsed since current state: "
         << chrono::duration_cast<std::chrono::seconds>
         (chrono::high_resolution_clock::now() - timeCurrState).count() << "\n";

    // if a number between 3 and 100 has been received, print additional info
    if (currState == State::LAUNCH && receivedNumber) {
      cout << "enteredNumber * timeSinceCurrState = " <<
        mostRecentEnteredNumber * chrono::duration_cast<std::chrono::seconds>
        (chrono::high_resolution_clock::now() - timeCurrState).count()  << "\n";
    }
    cout << "-----------------------------------------\n";

    // unlock before looping again
    mtx.unlock();
  }

  return;
}
