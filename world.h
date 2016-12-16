//
//  world.h
//  opengl
//
//  Created by Josiah Ebhomenye on 16/12/2016.
//  Copyright © 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef world_h
#define world_h

#include <vector>
#include "RigidBody.h"

namespace physics{
    using RigidBodies = std::vector<RigidBody*>;
    
    class World{
        
    protected:
        RigidBodies _rigidBodies;
        ForceRegistry _forceRegistry;

    public:
        
        ~World(){}
        
        void intergrate(real elapsedTime){
            for(auto body : _rigidBodies){
                body->integrate(elapsedTime);
            }
        }
        
        void runPhysics(real elapsedTime){
            _forceRegistry.applyForces(elapsedTime);
            intergrate(elapsedTime);

        }
        
        void startFrame(){
            for(auto body : _rigidBodies){
                body->clearAccumulators();
                body->calculateDerivedData();
            }
           
        }
        
        RigidBodies& rigidBodies(){
            return _rigidBodies;
        }
        

        ForceRegistry& forceRegistry(){
            return _forceRegistry;
        }

    };
    
}
#endif /* world_h */
