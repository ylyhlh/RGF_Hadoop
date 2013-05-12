#include <stdlib.h>
#include "timer.h"

Timer::Timer() {
  reset();
}

Timer::~Timer() {
}

void Timer::reset() {
  start_.tv_sec = 0;
  start_.tv_usec = 0;
  end_.tv_sec = 0;
  end_.tv_usec = 0;
}

void Timer::start() {
  gettimeofday(&start_, NULL);
}

void Timer::end() {
  gettimeofday(&end_, NULL);
}

double Timer::elapsed() const {
  // assumes end_ >= start_

  double sec = 0;
  double usec = 0;
  if (end_.tv_usec < start_.tv_usec) {
    sec = end_.tv_sec - 1 - start_.tv_sec;
    usec = (end_.tv_usec + 1000000 - start_.tv_usec) / 1000000.0;
  } else {
    sec = end_.tv_sec - start_.tv_sec;
    usec = (end_.tv_usec - start_.tv_usec) / 1000000.0;
  }
  return sec+usec;
}
