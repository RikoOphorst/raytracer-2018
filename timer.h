#pragma once

#include <chrono>

class Timer
{
public:
  Timer();

  inline void Reset();
  inline int ElapsedInNanoseconds();
  inline int ElapsedInMicroseconds();
  inline int ElapsedInMilliseconds();
  inline int ElapsedInSeconds();

private:
  std::chrono::high_resolution_clock::time_point start_;
};

//------------------------------------------------------------------------------------------------------
inline void Timer::Reset()
{
  start_ = std::chrono::high_resolution_clock::now();
}

//------------------------------------------------------------------------------------------------------
inline int Timer::ElapsedInNanoseconds()
{
  std::chrono::high_resolution_clock::duration dt = std::chrono::high_resolution_clock::now() - start_;

  return static_cast<int>(std::chrono::duration_cast<std::chrono::nanoseconds>(dt).count());
}

//------------------------------------------------------------------------------------------------------
inline int Timer::ElapsedInMicroseconds()
{
  std::chrono::high_resolution_clock::duration dt = std::chrono::high_resolution_clock::now() - start_;

  return static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(dt).count());
}

//------------------------------------------------------------------------------------------------------
inline int Timer::ElapsedInMilliseconds()
{
  std::chrono::high_resolution_clock::duration dt = std::chrono::high_resolution_clock::now() - start_;

  return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(dt).count());
}

//------------------------------------------------------------------------------------------------------
inline int Timer::ElapsedInSeconds()
{
  std::chrono::high_resolution_clock::duration dt = std::chrono::high_resolution_clock::now() - start_;

  return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(dt).count());
}
