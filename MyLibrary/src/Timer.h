#ifndef TIMER_H_
#define TIMER_H_

#include <iostream>
#include <chrono>
#include <ratio>

// scope-based timer
class Timer {
public:
    Timer() {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        std::chrono::duration<double, std::milli> dur = std::chrono::high_resolution_clock::now() - m_Start;
        std::cout << dur.count() << " ms\n";
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

#endif // !TIMER_H_