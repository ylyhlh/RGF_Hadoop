#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

// A microsecond precision timer based on the gettimeofday() call
// (which should be low overhead).
//
// Usage:
//
//   Timer t;
//   t.start();
//   ... event we want to clock
//   t.end();
//
//   std::cout << "elapsed time in seconds" << t.elapsed();
//

class Timer {
public:
  Timer();
  ~Timer();

  void start();
  void end();
  void reset();
  double elapsed() const;

private:
  struct timeval start_;
  struct timeval end_;
};

class StopWatch {
public:
  StopWatch() : total_(0) { }
  void start() { timer_.start(); }
  void end() {
    timer_.end();
    total_ += timer_.elapsed();
    timer_.reset();
  }

  void reset() {
    total_ = 0;
    timer_.reset();
  }

  double get_total() { return total_; }

private:
  double total_;
  Timer timer_;
};

#endif // TIMER_H
