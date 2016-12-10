//
//  forces.h
//  opengl
//
//  Created by Josiah Ebhomenye on 04/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_forces_h
#define opengl_forces_h

#include "precision.h"
#include <list>
#include <algorithm>
#include <functional>
#include "Particle.h"


namespace physics {
    
    class ForceGenerator{
    public:
        virtual void apply(Particle& p, real time) = 0;
    };
    
    class Gravity : public ForceGenerator{
    private:
        const Vector a;
        
    public:
        Gravity(Vector a): a(a){}
        
        virtual void apply(Particle& p, real time) override{
            Vector gravity = a * p.mass();
            p.addForce(gravity);
        }
    };

    class ForceRequestForVelocity : public ForceGenerator{
    private:
        Velocity expectedV;
        
    public:
        ForceRequestForVelocity(Vector v): expectedV(v){}
        
        virtual void apply(Particle& p, real time) override{
            real m = p.mass();
            real t = time;
            Vector v = expectedV;
            
            Vector f = v * (m/t);
            p.addForce(f);
        }
        
    };
    
    
    struct Registration{
        Particle& p;
        ForceGenerator& f;
        
        bool operator==(const Registration& other){
            return &p == &(other.p) && &f == &other.f;
        }
    };
    
    using Registry = std::list<Registration>;
    
    class ForceRegistry{
    private:
        Registry registry;
        
    public:
        ForceRegistry& operator+(Registration reg){
            bool contains = std::any_of(registry.begin(), registry.end(), [&](Registration& registred){
                return registred == reg;
            });
            if(!contains){
                registry.push_back(reg);
            }
            return *this;
        }
        
        ForceRegistry& operator-(Registration reg){
            auto itr = std::find_if(registry.begin(), registry.end(), [&](Registration& registered){
                return registered == reg;
            });
            if(itr != registry.end()){
                registry.erase(itr);
            }
            return *this;
            
        }
        
        
        void applyForces(float time){
            std::for_each(registry.begin(), registry.end(),[&](Registration& reg){
                reg.f.apply(reg.p, time);
            });
        }
    };
    

}

#endif
