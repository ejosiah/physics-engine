//
//  Camera.h
//  opengl
//
//  Created by Josiah Ebhomenye on 04/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Camera_h
#define opengl_Camera_h


#ifdef __APPLE__
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

#include <initializer_list>
#include <iostream>
#include "core.h"
#include "Timer.h"
using namespace physics;

struct Projection{
    real fov;
    real aspectRatio;
    real _near;
    real _far;
};

class Camera{
protected:
    float _yaw, _pitch, _roll;
    float _fov, _aspectRatio;
    float _zNear = 0.1f;
    float _zFar = 1000.f;
    Vector _look;
    Vector _up{0, 1, 0};
    Vector _right;
    Vector _position;
    Vector _target;
    
public:
    Camera() = default;
    ~Camera() = default;
    Vector farPts[4];
    Vector nearPts[4];
    
    
public:
    void set(const Projection& p){
        _fov = p.fov;
        _aspectRatio = p.aspectRatio;
        _zNear = p._near;
        _zFar = p._far;
        
    }
    
    void updateProjection(){
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(_fov, _aspectRatio, _zNear, _zFar);
        glMatrixMode(GL_MODELVIEW);
    }
    
    virtual void update() = 0;
    
    
    virtual void rotate(const float yaw, const float pitch, const float roll){
        _yaw = yaw;
        _pitch = pitch;
        _roll = roll;
    }
    
    void clearRotation(){
        _yaw = 0;
        _pitch = 0;
        _roll = 0;
    }
    
    static bool log;
    
    virtual void move(){
        using namespace std;
//                    cout << "position: " << _position << endl;
//                    cout << "target" << _target << endl;
                 //   cout << "position" << _position << endl << endl;
        gluLookAt(_position.x, _position.y, _position.z
                  , _target.x, _target.y, _target.z
                  , _up.x, _up.y, _up.z);
    }
    
    virtual void lookAt(const Vector& position, const Vector& target, const Vector& up){
        _position = position;
        _target = target;
        _up = up;
    }
    
    void position(Vector pos){
        _position = pos;
    }
    
    void position(std::initializer_list<float> pos){
        auto itr = pos.begin();
        _position.x = *(itr++);
        _position.y = *(itr++);
        _position.z = *itr;
    }
    
    Vector position() const;
    
    void fov(float value){
        _fov = value;
    }
    
    float fov() const{
        return _fov;
    }
    
    float aspectRatio() const{
        return _aspectRatio;
    }
    
    void aspectRatio(real ratio){
        _aspectRatio = ratio;
    }
    
    real zNear() const{
        return _zNear;
    }
    
    real zFar() const{
        return _zFar;
    }
    
    real yaw() const{
        return _yaw;
    }
    
    real pitch() const{
        return _pitch;
    }
    
    virtual void keyPress(unsigned char key, int x, int y){}
    
    virtual void mouse(int button , int x, int y){}
    
    virtual void mouseMove(int x, int y){}
};

bool Camera::log = true;

class FreeCamera : public Camera{
private:
    float _speed;
    Vector _translation;
    
public:
    virtual void setup(const Projection& projection, const Vector& position, real speed = 100, const Vector& look = Vector{}){
        set(projection);
        _position = position;
        _look = look;
        _look.invert();
        _look.normalize();
        _speed = speed;
        
        real yaw = 0.0f;
        real pitch = 0.0f;
        if(fabs(_look.length() - 0) > epsilon){
            yaw =  (real_atan2(_look.z, _look.x) + M_PI)/DEG_TO_RAD;
            pitch = (real_asin(_look.y))/DEG_TO_RAD;
        }
        
        rotate(yaw, pitch,0);
        update();
    }
    
    
    virtual void update() override{
        using namespace std;
        
        decayTranslation();
        
        Matrix4 rotate = RotationMatrix(_yaw, _pitch, _roll);
        _position += _translation;
        _translation.clear();
        
        _look = rotate * Vector{0, 0, -1, 0};
        _target = _position + _look;
        _up = rotate * Vector{0, 1, 0, 0};
        _right = _look * _up;
        
        log = true;
        
//        cout << "Position: " << _position << endl;
//        cout << "look: " << _look << endl;
//        cout << "up: " << _up << endl;
//        cout << "target: " << _target << endl;
        
    }
        
    
    virtual void walk(const float dt){
        _translation += _look * _speed * dt;
    }
    
    virtual void strafe(const float dt){
        _translation += _right * _speed * dt;
    }
    
    virtual void lift(const float dt){
        _translation += _up * _speed * dt;
    }
    
    void translate(const Vector& t){
        _translation = t;
    }
    
    Vector translation() const{
        return _translation;
    }
    
    void speed(const float s){
        _speed = s;
    }
    
    const float speed() const{
        return _speed;
    }
    
    void decayTranslation(){
        Vector t1 = _translation;
        Vector t2 = _translation;
        if(t1.dotProduct(t2) > epsilon2){
            _translation *= 0.95f;
        }
    }
    
    virtual void keyPress(unsigned char key, int x, int y) override{
        float dt = Timer::get().lastFrameTime;
        switch(key){
            case 'w':
                walk(dt);
                break;
            case 's':
                walk(-dt);
                break;
            case 'a':
                strafe(-dt);
                break;
            case 'd':
                strafe(dt);
                break;
            case 'q':
                lift(dt);
                break;
            case 'z':
                lift(-dt);
            default:
                break;
        }
        update(); // TODO calling update on no mostion resets the camera
    }
};

class MouseHandler{
private:
    static constexpr real FILTER_WEIGHT=0.75f;
    static const int HISTORY_BUFFER_SIZE = 10;
    
    int state = 1;
    real fov;
    
    Vector preivous;
    Vector position;
    Vector rotation;
    Vector history[HISTORY_BUFFER_SIZE];
    Camera* camera;
    
    
public:
    bool useFitering;
    MouseHandler(){}
    MouseHandler(Camera* c, bool filter = false): camera(c), useFitering(filter){
        for(int i =  0; i < HISTORY_BUFFER_SIZE; i++){
            history[i] = Vector(c->yaw(), c->pitch());
        }
    }
    
protected:
    void filter(Vector dv){
        for(int i = HISTORY_BUFFER_SIZE - 1; i > 0; i--){
            history[i] = history[i - 1];
        }
        
        history[0] = dv;
        
        Vector avg;
        real avgTotal = 0.0f;
        real currentWeight = 1.0f;
        
        for(int i = 0; i < HISTORY_BUFFER_SIZE; i++){
            Vector tmp = history[i];
            
            avg += tmp * currentWeight;
            avgTotal += 1.0f * currentWeight;
        }
        
        position = avg * (real(1.0)/avgTotal);
    }
    
public:
    void onMouseDown(int button , int state, const Vector& position){
        if(state == GLUT_DOWN){
            preivous = position;
        }
        
        if(button == GLUT_MIDDLE_BUTTON){
            this->state = 0;
        }else{
            this->state = 1;
        }
    }
    
    void onMouseMove(real x, real y){
        if(state == 0){
            fov += (y - preivous.y)/5.0f;
            camera->set(Projection{fov, camera->aspectRatio(), camera->zNear(), camera->zFar()});
        }else{
            rotation += Vector{preivous.x - x, y - preivous.y} * 0.5f;
            if(useFitering){
                filter(Vector{rotation});
            }else{
                position = rotation;
            }
            camera->rotate(position.x, position.y, 0);
        }
        preivous = Vector{x, y};
        camera->update();
    }
};

#endif
