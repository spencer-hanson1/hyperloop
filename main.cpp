// UNSW Hyperloop
// Software Screen Task
// Spencer Hanson
// main.cpp

#include "Machine.h"

using namespace std;

int main() {
  // dynamically allocates memory on Heap
  Machine * podPtr = new Machine();

  // threads for reading and printing
  thread stdinThread (&Machine::read, podPtr);
  thread stdoutThread (&Machine::print, podPtr);

  // spawns and detaches threads
  while (!podPtr->errorFound()) {
    // checks if there is an integer in the buffer
    if (podPtr->bufferSize() > 0) {
      thread (&Machine::update, podPtr).detach();
    }
  }

  // join stdio threads
  stdinThread.join();
  stdoutThread.join();

  // deallocates memory, ensures no bad access
  delete podPtr;
  podPtr = nullptr;
  return 0;
}
