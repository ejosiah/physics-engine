//
//  Timer.h
//  opengl
//
//  Created by Josiah Ebhomenye on 04/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Timer_h
#define opengl_Timer_h

#include <chrono>

using Clock = std::chrono::steady_clock;

class Timer{
private:
    static Timer* timer;
    Clock::time_point currentTime;
    
public:
    float lastFrameTime;
    
private:
    Timer(): currentTime{Clock::now()}{};
    
    
public:
    static void start(){
        timer = new Timer;
    }
    
    static void stop(){
        delete timer;
    }
    
    static Timer& get(){
        return *timer;
    }
    
    void update(){
        using namespace std::chrono;
        auto diff = Clock::now() - currentTime;
        lastFrameTime = duration_cast<milliseconds>(diff).count()/1000.0;
        currentTime += diff;
        
    }
};

Timer* Timer::timer;

#endif
