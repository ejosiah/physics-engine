//
//  random.h
//  opengl
//
//  Created by Josiah Ebhomenye on 05/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_random_h
#define opengl_random_h

#include <random>
#include <functional>
#include <typeinfo>
#include "precision.h"
#include "core.h"

namespace physics {

    static std::random_device seed;

    template <typename T = real>
    std::function<T(void)> rng(T lower, T upper){
        
        std::default_random_engine eng{seed()};
        
        if(typeid(lower) == typeid(int)){
            return std::bind(std::uniform_int_distribution<T>{}, eng);
        }else if(typeid(lower) == typeid(real)){
            return std::bind(std::uniform_real_distribution<T>{}, eng);
        }
        throw "invalid type";
    }

    template <typename T>
    class RandomType{
    private:
        std::function<T(void)> _rng;
        
    public:
        RandomType(T l, T u){
            _rng = rng(l, u);
        }
        
        T operator()(){
            return _rng();
        }
        
    };

    using RandomInt = RandomType<int>;
    using RandomReal = RandomType<real>;

    class Random{
    protected:
        RandomReal rng;

    public:
        Random()
        :rng{0.0, 1.0}{
        }
        
        real _real(real min, real max){
            return rng() * (max - min) + min;
        }
        
        int _int(int x){
            return round((rng() * x));
        }
        
        Vector vector(Vector& min, Vector max){
            return Vector{
                _real(min.x, max.x),
                _real(min.y, max.y),
                _real(min.z, max.z)
            };
        }
        
    };
}
#endif
