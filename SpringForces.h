//
//  SpringForces.h
//  opengl
//
//  Created by Josiah Ebhomenye on 08/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_SpringForces_h
#define opengl_SpringForces_h

#include <memory>
#include "core.h"
#include "forces.h"
#include "RigidBody.h"

namespace physics {
    
    class SpringForce : public ForceGenerator{
    protected:
        Position connectionPoint;
        Position otherConnectionPoint;
        Particle& other;
        real springConstant;
        real restLength;
        
    public:
        SpringForce(const Position& cp, const Position& ocp, Particle& p, real k, real l)
        :connectionPoint(cp), otherConnectionPoint(ocp),
        other(p), springConstant(k), restLength(l){ }
        
        virtual void apply(Particle& p, real time) override{
            
            Vector pos = getPos(p);
            real d = pos.length();
            real k = springConstant;
            real l = restLength;
            pos.normalize();
            
            Vector force =  pos * (-k * (d - l));
            
            p.addForce(force);

        }
        
        Vector getPos(Particle& p){
            try{
                RigidBody& rb = dynamic_cast<RigidBody&>(p);
                RigidBody& rbo = dynamic_cast<RigidBody&>(other);
                Vector lws = rb.pointInWorldSpace(connectionPoint);
                Vector ows = rbo.pointInWorldSpace(otherConnectionPoint);
                return lws - ows;
            }catch(std::bad_cast& e){
                return p.position() - other.position();
            }
        }
    };
    
    class AnchoredSpringForce : public ForceGenerator{
    protected:
        Vector& anchor;
        real springConstant;
        real restLength;
        
    public:
        AnchoredSpringForce(Vector& anchor, real springConstant, real restLength)
        :anchor(anchor), springConstant(springConstant), restLength(restLength){
            
        }
        
        virtual void apply(Particle& p, real time) override{
            Vector pos = p.position() - anchor;
            real d = pos.length();
            real k = springConstant;
            real l = restLength;
            
            Vector force = pos * (k * (l - d));
            
            p.addForce(force);
        }
        
    };
    
}

#endif
