//
//  scene.h
//  opengl
//
//  Created by Josiah Ebhomenye on 03/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_scene_h
#define opengl_scene_h


#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif


#include <map>
#include <unordered_map>
#include <vector>
#include <string>

#include "core.h"
#include "Particle.h"
#include "forces.h"
#include "Timer.h"
#include "Camera.h"
#include "contacts.h"
#include "ParticleWorld.h"
#include "RigidBody.h"
#include "SpringForces.h"
#include "world.h"


void glMutiMat(Matrix4& m){
    m.transpose();
    glMultMatrixf(m);
    m.transpose();
}


using namespace physics;
int MAX_CONTACTS = 500;
ParticleWorld world{ MAX_CONTACTS };
GroundContactGenrator groundContact{world.particles()};
SphericalContactGenerator sphericalContact{world.particles(), 2};

class SceneObject{
public:
    virtual const char* name() = 0;
    virtual void update(float duration) = 0;
    virtual void draw() = 0;
    
};

class ParticleSceneObject : public SceneObject, public Particle{};

using SceneObjects = std::unordered_map<const char*, SceneObject*>;
using Marked = std::vector<SceneObject*>;
using Contacts = std::vector<Contact>;
using BigInt = long long int;

class Scene{
protected:
    static const int DEFAULT_WIDTH = 500;
    static const int DEFAULT_HEIGHT = 500;
    static SceneObjects objects;
	static Marked markedObjects;
    const char* _title;
    Camera* camera;
    MouseHandler mouse;
    int _width;
    int _height;
    real fov=60;
    real cl = 50;
    int fps;
    BigInt frames;

    
protected:
    Vector speed{1.0, 1.0, 1.0};
    float angle;
    
public:
    static Scene* instance;
    
public:
    Scene(const char* title, int w = DEFAULT_WIDTH, int h = DEFAULT_HEIGHT)
    :_title(title), _width(w), _height(h){
        FreeCamera* camera{ new FreeCamera() };
        camera->setup(Projection{fov, (float)w/h, 1, 1000}, Vector{80, 20, 60}, 100.0);
        mouse = MouseHandler(camera);
        this->camera = camera;
    }
    
    ~Scene(){
        delete camera;
    }
    
    const char* title() const { return _title; }
    int width() const { return _width; }
    int height() const { return _height; }
    
    virtual void display() {
        for(std::pair<const char*, SceneObject*> entry : objects){
            entry.second->draw();
        }
    }
    
    virtual void init(){
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        
        float light_pos[] = {0.0, 1.0, 0.0, 1.0};
        
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glColorMaterial(GL_FRONT, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_RESCALE_NORMAL);
        glEnable(GL_NORMALIZE);
    }
    
    void drawCrosshairs(){
        glLineWidth(2.5);
        glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(cl, 0, 0);
        
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, cl, 0);
        
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, cl);
        glEnd();
    }
    
    virtual void render(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        camera->move();
        drawCrosshairs();
        display();
        frames++;
        debug();

    }
    
    void updateFrameData(int elapsedTime){
        fps = frames/elapsedTime;
    }
    
    virtual void resize(int w, int h){
        _width = w;
        _height = h;
        glViewport(0, 0, _width, _height);
        camera->aspectRatio((real)_width/_height);
        camera->updateProjection();
        
    }
    
    void keyPress(unsigned char key, int x, int y){
        camera->keyPress(key, x, y);
        if(key == 'f') mouse.useFitering = !mouse.useFitering;
        onKeyPress(key, x, y);
    }
    
    void specialKey(int key, int x, int y){

    }
    
    void mouseDown(int button, int state, int x, int y){
        mouse.onMouseDown(button, state, Vector{(real)x, (real)y});
        onMouseDown(button, state, x, y);
    }
    
    virtual void onMouseDown(int button, int state, int x, int y){
        
    }
    
    void mouseMove(int x, int y){
        mouse.onMouseMove((real)x, (real)y);
    }
    
    virtual void onKeyPress(unsigned char key, int x, int y){}
    
    virtual void update(){
        real t = Timer::get().lastFrameTime;
        
        world.startFrame();
        world.runPhysics(t);
        
        for(std::pair<const char*, SceneObject*> entry : objects){
            SceneObject* obj = entry.second;
            obj->update(t);
        }
		clear();
    }

	void clear() {
		for (auto obj : markedObjects) {
			objects.erase(obj->name());
            Particle* particle = dynamic_cast<Particle*>(obj);
            if(particle){
                world.remove(*particle);
            }
		}
		markedObjects.clear();
	}
    
    void renderText(float x, float y, const char *text, const real* color = nullptr, void* font=nullptr){
        if(!color){
            glColor3f(0, 0, 0);
        }else{
            glColor3fv(color);
        }
        glDisable(GL_DEPTH_TEST);
        
        // Temporarily set up the view in orthographic projection.
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, (double)_width, 0.0, (double)_height, -1.0, 1.0);
        
        // Move to modelview mode.
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        // Ensure we have a font
        if (font == nullptr) {
            font = GLUT_BITMAP_HELVETICA_12;
        }
        
        // Loop through characters displaying them.
        size_t len = strlen(text);
        glRasterPos2f(x, y);
        for (const char *letter = text; letter < text+len; letter++) {
            
            // If we meet a newline, then move down by the line-height
            // TODO: Make the line-height a function of the font
            if (*letter == '\n') {
                y -= 12.0f;
                glRasterPos2f(x, y);
            }
            glutBitmapCharacter(font, *letter);
        }
        
        // Pop the matrices to return to how we were before.
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        
        glEnable(GL_DEPTH_TEST);
    }
    
    virtual void debug(){
        size_t n = objects.size();
        std::string text = "object count: " + std::to_string(n);
        renderText(10, _height - 10, text.c_str());
        text = "fps: " + std::to_string(fps);
        renderText(10, _height - 25, text.c_str());
    }
    
    static void addObject(SceneObject* object){
        Particle* particle = dynamic_cast<Particle*>(object);
        if(particle){
            world.add(*particle);
        }
        objects.insert(std::pair<const char*, SceneObject*>(object->name(), object));
    }
    
    template<class T>
    static T* getObject(const char* name){
        return (T*)objects[name];
    }
    
    static void remove(SceneObject* object){
		markedObjects.push_back(object);
    }
    
};

SceneObjects Scene::objects;
Marked Scene::markedObjects;

#endif
