#include <iostream> 
#include <chrono>

class Timer {
    bool running = false;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point lapStartTime;
    std::chrono::high_resolution_clock::time_point endTime;
private:
    double getDuration() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    }

public:
    Timer(bool start = true) {
        if(start) {
            this->start();
        } 
    }


    void start() {
        if(!running) {
            startTime = std::chrono::high_resolution_clock::now(); 
            lapStartTime = startTime;
            endTime = std::chrono::high_resolution_clock::time_point::min();
            running = true;
        } else {
            throw std::runtime_error("Timer already started");
        }
    }

    double lap() {  
        if(!running) {
            throw std::runtime_error("Timer not running!");
        }

        double duration = 0;
        auto currTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - lapStartTime).count();
        lapStartTime = currTime;
        return duration;
    }

    std::pair<double, double> stop() {
        if(!running) {
            throw std::runtime_error("Timer not running!");
        }
        endTime = std::chrono::high_resolution_clock::now();
        running = false;
        auto lapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - lapStartTime).count();
        auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        return std::make_pair(lapDuration, totalDuration);
    }
};
