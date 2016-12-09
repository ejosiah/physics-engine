//
//  canonTest.h
//  opengl
//
//  Created by Josiah Ebhomenye on 08/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_canonTest_h
#define opengl_canonTest_h

#include "scene.h"
#include <array>
#include <tuple>
#include <utility>
#include <string>
#include <algorithm>

enum Type { PISTOL = 0, ARTILLARY, FIREBALL, LEASER, UNUSED };
const static int ROUNDS= 16;

static int nextId = 0;

class Ammo : public ParticleSceneObject{
private:
    int _id;
    std::string _name;
    
public:
     Ammo():_id(++nextId){
         _name = std::string{"amon"} + std::to_string(_id);
     }
    Type type = UNUSED;
    real startTime;
    
    ~Ammo(){
        std::cout << "calling destructor for" << _name << std::endl;
    }
    
    virtual const char* name() override{
        return _name.c_str();
    }
    virtual void draw() override{
        glPushMatrix();
        glTranslatef(x(), y(), -z());
        // bullet
        glColor3f(0, 0, 0);
        glutSolidSphere(2.5, 10, 10);
        glPopMatrix();
        
        glPushMatrix();
        glColor3f(0.5, 0.5, 0.5);
        glTranslatef(x(), 0, -z());
        glScalef(1, 0.1, 1);
        glutSolidSphere(2.5, 10, 10);
        glPopMatrix();
        
        
        
    }
    
    virtual void update(float elapsedTime) override{
        if(y() < 0 || z() > 1000 or startTime+5 < elapsedTime){
            type = UNUSED;
            std::cout << "removing " << _name << " from scene" << std::endl;
            Scene::remove(this);
        }
        
    }
};

template <size_t size_t>
using Ammunition = std::array<Ammo*, size_t>;

class Barrel{
private:
    Vector p;
    real& spin;
public:
    real length = 30;
    Barrel(real& spin, Vector position = {}): p(position), spin(spin){}
    
    void draw(){
        glPushMatrix();
        glTranslatef(p.x, p.y, p.z);
        glRotatef(spin, 1, 0, 0);
        glTranslatef(0, 15, 0);
        glScalef(1, length/10, 1);
        glutSolidCube(length/3);
        glPopMatrix();
    }
};

class Base{
private:
    Vector p;
public:
    Base(Vector position = {}): p(position){
        
    }
    
    void draw(){
        glPushMatrix();{
            glTranslatef(p.x, p.y, p.z);
            glutSolidSphere(15, 20, 20);
            glPushMatrix();{
                glTranslatef(0, -10, 0);
                glScalef(1, 0.5, 1);
                glutSolidCube(30);
            }glPopMatrix();
        }glPopMatrix();
    }
};

class Canon : public SceneObject{
private:
    Ammunition<ROUNDS> ammuiation;
    Type currentType = PISTOL;
    real spin = -90;
    real dx = 100;
    Vector p{0, 15, 0};
    Barrel barrel{spin, {0, 15, 0}};
    Base base{{0, 15, 0}};
    
    virtual const char* name() override{
        return "Canon";
    }
    

public:
    Canon()
    {
        for(int i = 0; i < ROUNDS; i++){
            ammuiation[i] = new Ammo();
        }
    }
    friend class CanonTest;
    virtual void update(float elapsedTime) override{
    }
    
    void aimUp(){
        if(spin <= -30) spin += dx * Timer::get().lastFrameTime;
        
    }
    
    void aimDown(){
        if(spin >= -90) spin -= dx * Timer::get().lastFrameTime;
    }
    
    void fire(){
        auto itr = std::find_if(ammuiation.begin(), ammuiation.end(), [](Ammo* a){ return a->type == UNUSED; });
        if(itr != ammuiation.end()){
            Ammo& shot = **itr;
            shot.type = currentType;
            load(shot);
            shot.startTime = Timer::get().lastFrameTime;
            
            real theta = angle();
            real y = real_sin(theta * DEG_TO_RAD) * barrel.length;
            real z = real_cos(theta * DEG_TO_RAD) * barrel.length;
            
            shot.position({p.x, p.y + y, z});
            Scene::addObject(&shot);
        }
    }
    
    void load(Ammo& ammo){
        switch (ammo.type) {
            case PISTOL:
                configureAmmo(ammo, {0, -1.0f, 0}, 35.0f, 2.0f, 0.99f);
                break;
            case ARTILLARY:
                configureAmmo(ammo, {0, -20.0f, 0}, 50.0f, 200.0f, 0.99f);
                break;
            case FIREBALL:
                configureAmmo(ammo, {0, 0.6, 0} , 5.0f, 1.0f, 1.0f);
                break;
            case LEASER:
                configureAmmo(ammo, {0, 0, 0}, 100.0f, 0.1f, 0.99f);
                break;
            default:
                break;
            }
    }

    void configureAmmo(Ammo& ammo, const Vector& acc, const real speed, const real mass, const real damping){
        real theta = angle();
        real y = real_sin(theta * DEG_TO_RAD) * speed;
        real z = real_cos(theta * DEG_TO_RAD) * speed;
        
        ammo.velocity({0, y, z});
        ammo.acceleration(acc);
        ammo.mass(mass);
        ammo.damping(damping);
    }
    
    virtual void draw() override {
        glColor3f(0, 0, 1);
//        glPushMatrix();
//       // glTranslatef(0, 20, -5);
//        glRotatef(spin, 1, 0, 0);
//        
//        
//        glTranslatef(0, p.y + 15, 0);
//        glScalef(1, 3, 1);
//        glTranslatef(-p.x, -p.y, -p.z);
//        glutSolidCube(10);
//        glPopMatrix();
//        
//        glPushMatrix();{
//            glutSolidSphere(15, 20, 20);
//            glPushMatrix();{
//                glTranslatef(p.x + 0, p.y -10, p.z + 0);
//                glScalef(1, 0.5, 1);
//                glutSolidCube(30);
//            }glPopMatrix();
//        }glPopMatrix();
        
        barrel.draw();
        base.draw();
        
        glPushMatrix();
        float theta = angle();
        real y = real_sin(theta * DEG_TO_RAD) * barrel.length;
        real z = real_cos(theta * DEG_TO_RAD) * barrel.length;
        glTranslatef(p.x, p.y + y, -z);
        // bullet
        glColor3f(0, 0, 0);
        glutSolidSphere(2.5, 10, 10);
        glPopMatrix();
    }
    
    const real angle(){
        real a = spin + 90;
        return a < 0 ? 0 : a;
    }
    
    const char* ammoType(){
        switch (currentType) {
            case PISTOL: return "pistol";
            case ARTILLARY: return "artillary";
            case FIREBALL: return "fireball";
            case LEASER: return "leaser";
            default: return "";
        }
    }
};

class CanonTest : public Scene{
private:
    Canon canon;
    
public:
    using Scene::Scene;
    
    
    virtual void init() override {
        Scene::init();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        addObject(&canon);
        FreeCamera* camera = dynamic_cast<FreeCamera*>(this->camera);
        if(camera){
            camera->speed(200);
        }
    }
    
    virtual void display() override{
        Scene::display();
        
        std::string msg = "cannon angle: " + std::to_string((int)(canon.angle()));
        renderText(10, 25, msg.c_str());
        const char* ammoType = canon.ammoType();
        msg = std::string{"current loaded ammo: "} + ammoType;
        renderText(10, 10, msg.c_str());
    }
    
    
    virtual void onKeyPress(unsigned char key, int x, int y) override{
        switch(key){
            case 'i':
                canon.aimUp();
                break;
            case 'k':
                canon.aimDown();
                break;
            case ' ':
                canon.fire();
                break;
            case '1':
                canon.currentType = PISTOL;
                break;
            case '2':
                canon.currentType = ARTILLARY;
                break;
            case '3':
                canon.currentType = FIREBALL;
                break;
            case '4':
                canon.currentType = LEASER;
                break;
            default:
                break;
        }
    }
    
    virtual void onMouseDown(int button, int state, int x, int y) override{
        
    }
    
};

Scene* Scene::instance = new CanonTest("CanonTest");

#endif
