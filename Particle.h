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
        
    protected:
		Vector _position;
        Velocity _velocity;
        Acceleration _acceleration;
        Force _accumulatedForce;
        real _inverseMass;
        real _damping;
        
        
        
    public:
        Particle(): _inverseMass(0), _damping(1){}
        
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
        
        Position position() const { return _position; }
        
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
        
        Velocity velocity(){ return _velocity; }
        
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
        
        Acceleration acceleration() const{ return _acceleration; }

		real inverseMass() {
			return _inverseMass;
		}
        
        void mass(real m){
            _inverseMass = 1/m;
        }
        
        virtual void addForce(const Vector& force){
            _accumulatedForce += force;
        }
        
        Vector accumulatedForces() const {
            return _accumulatedForce;
        }
        
        real mass() const { return 1/_inverseMass; }
        
        real damping(const real d){
            return _damping = d;
        }
        
        real damping() const{
            return _damping;
        }
        
        
        virtual void integrate(real time){
            
            using namespace std;
            if(_inverseMass <= 0) return;
            
            _position.addScaled(_velocity, time);
            
            Acceleration acc = _acceleration;
            
            acc.addScaled(_accumulatedForce, _inverseMass);
            
            _velocity.addScaled(acc, time);
            
            _velocity *= real_pow(_damping, time);
            

         
            clearAccumulators();
        }
        
        virtual void clearAccumulators(){
            _accumulatedForce.clear();
        }
        
    };
}


#endif
