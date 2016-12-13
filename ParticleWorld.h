//
//  ParticleWorld.h
//  opengl
//
//  Created by Josiah Ebhomenye on 13/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_ParticleWorld_h
#define opengl_ParticleWorld_h

#include <vector>
#include <array>
#include <algorithm>
#include "forces.h"
#include "contacts.h"

namespace physics{
    using Particles = std::vector<Particle*>;
    using ContactGenerators = std::vector<ContactGenerator*>;
    using Contacts = std::vector<Contact>;
    class ParticleWorld{
        
    protected:
        Particles _partices;
        bool _iterations;
        ForceRegistry _forceRegistry;
        ContactResolver _contactResolver;
        ContactGenerators _contactGenerators;
        Contacts _contacts;
        int _maxContacts;
        bool calculateIterations;
        
    public:
        ParticleWorld(int maxContacts, int iterations=0)
        :_maxContacts(maxContacts)
        , _contactResolver(iterations){
            calculateIterations = (iterations == 0);
        }
        
        ~ParticleWorld(){}
        
        void intergrate(real elapsedTime){
            for(auto p : _partices){
                p->integrate(elapsedTime);
            }
        }
        
        void runPhysics(real elapsedTime){
            _forceRegistry.applyForces(elapsedTime);
            intergrate(elapsedTime);
            
            generateContacts();
            
            if(!_contacts.empty()){
                if(calculateIterations) _contactResolver.iterations(_contacts.size() * 2);
                _contactResolver.resolveContacts(_contacts, elapsedTime);
            }
        }
        
        void startFrame(){
            for(auto p : _partices){
                p->clearAccumulator();
            }
            _contacts.clear();
        }
        
        Particles& particles(){
            return _partices;
        }
        
        void add(Particle& p){
            _partices.push_back(&p);
        }
        
        void remove(Particle& p){
            auto itr = std::find_if(_partices.begin(), _partices.end(), [&](Particle* p1){ return p1 == &p; });
            if(itr != _partices.end()){
                _partices.erase(itr);
                _forceRegistry - (**itr);
            }
        }
        
        ContactGenerators& contactGenerators(){
            return _contactGenerators;
        }
        
        ForceRegistry& forceRegistry(){
            return _forceRegistry;
        }
        
        void generateContacts(){
            int limit = _maxContacts;
            
            for(auto contactGen : _contactGenerators){
                contactGen->add(_contacts, limit);
                limit = _maxContacts - _contacts.size();
                if (limit <= 0) break;
            }
        }
    };
    
    class GroundContactGenrator : public ContactGenerator{
    private:
        const Particles& particles;
        Position ground{0, 1, 0};
        
    public:
        GroundContactGenrator(const Particles& p): particles(p){}
        
        virtual void add(std::vector<Contact>& contacts, int limit) const override{
            int count = 0;
            for(auto particle : particles){
                real penetration = particle->y() - 2;
                if(penetration < 0){
                    Contact c;
                    c.particle[0] = particle;
                    c.particle[1] = nullptr;
                    c.contactNormal = ground;
                    c.resitution = 0.4;
                    c.penetration = -penetration;
                    contacts.push_back(c);
                    count++;
                }
                if(count >= limit) break;
            }
            
        }
    };
    
    class SphericalContactGenerator : public ContactGenerator{
    private:
        const Particles& particles;
        real radius;
        
    public:
        SphericalContactGenerator(const Particles& p, real r):
        particles(p), radius(r){}
        
        virtual void add(std::vector<Contact>& contacts, int limit) const override{
            for(auto pa : particles){
                for(auto pb : particles){
                    if(pa == pb) continue;
                    
                    int pen = peneratration(*pa, *pb);
                    if(pen > 0){
                        Contact c;
                        c.particle[0] = pa;
                        c.particle[1] = pb;
                        c.contactNormal = (pa->position() - pb->position()).normalize();
                        c.resitution = 0.2;
                        c.penetration = pen;
                        contacts.push_back(c);
                    }
                }
            }
        }
        
        int peneratration(const Particle& pa, const Particle& pb) const{
            return (2 * radius) - (pa.position() - pb.position()).length();
        }
    };
}




#endif
