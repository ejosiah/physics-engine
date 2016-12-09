//
//  SpringForces.h
//  opengl
//
//  Created by Josiah Ebhomenye on 08/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_SpringForces_h
#define opengl_SpringForces_h

#include "forces.h"

namespace physics {
    
    class SpringForce :public Force{
    protected:
        Particle& other;
        real springConstant;
        real restLength;
        
    public:
        SpringForce(Particle& p, real k, real l)
        :other(p), springConstant(k), restLength(l){ }
        
        virtual void apply(Particle& p, real time) override{
            Vector pos = p.position() - other.position();
            real d = pos.length();
            real k = springConstant;
            real l = restLength;
            
            Vector force =  pos * (-k * (d - l));
            
            p.addForce(force);

        }
    };
    
    class AnchoredSpringForce : public Force{
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
