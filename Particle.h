//
//  Particle.h
//  opengl
//
//  Created by Josiah Ebhomenye on 03/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Particle_h
#define opengl_Particle_h

#include <initializer_list>
#include "core.h"
#include <iostream>

namespace physics{
    
    class Particle{
        Vector _position;
    protected:
        Vector _velocity;
        Vector _acceleration;
        Vector accumulatedForce;
        real inverseMass;
        real _damping;
        
        
        
    public:
        Particle(){}
        
        real x() const{
            return _position.x;
        }
        
        real y() const{
            return _position.y;
        }
        
        real z() const{
            return _position.z;
        }
        
        void position(const Vector& p){
            _position = p;
        }
        
        void position(std::initializer_list<real> values){
            if(values.size() < 3) throw "3 arguments required";
            auto itr = values.begin();
            _position.x = *(itr++);
            _position.y = *(itr++);
            _position.z = *itr;
        }
        
        Vector position(){ return _position; }
        
        void velocity(const Vector v){
            _velocity = v;
        }
        
        void velocity(std::initializer_list<real> values){
            if(values.size() < 3) throw "3 arguments required";
            auto itr = values.begin();
            _velocity.x = *(itr++);
            _velocity.y = *(itr++);
            _velocity.z = *itr;
        }
        
        Vector velocity(){ return _velocity; }
        
        void acceleration(const Vector& acc){
            _acceleration = acc;
        }
        
        void acceleration(std::initializer_list<real> values){
            if(values.size() < 3) throw "3 arguments required";
            auto itr = values.begin();
            _acceleration.x = *(itr++);
            _acceleration.y = *(itr++);
            _acceleration.z = *itr;
        }
        
        Vector acceleration(){ return _acceleration; }
        
        void mass(real m){
            inverseMass = 1/m;
        }
        
        void addForce(const Vector& force){
            accumulatedForce += force;
        }
        
        real mass(){ return 1/inverseMass; }
        
        real damping(const real d){
            return _damping = d;
        }
        
        real damping() const{
            return _damping;
        }
        
        
        void integrate(real time){
            using namespace std;
            if(inverseMass <= 0) return;
            
            _position += _velocity * time;
            
           _acceleration  += (accumulatedForce * inverseMass);
            _velocity += _acceleration * time;
            
            _velocity *= real_pow(_damping, time);
            
//            cout << "acceleration: " << _acceleration << endl;
//            cout << "velocity" << _velocity << endl;
//            cout << "position" << _position << endl << endl;
            
            accumulatedForce.clear();
        }
        
    };
}


#endif
