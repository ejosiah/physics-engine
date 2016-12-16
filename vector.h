//
//  vector.h
//  opengl
//
//  Created by Josiah Ebhomenye on 14/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_vector_h
#define opengl_vector_h

#include <initializer_list>
#include <iostream>
#include <glm/glm.hpp>

#include "precision.h"

namespace physics {
    class Vector{
    public:
        union{
            struct { real x, y, z, w; };
            struct { real r, g, b, a; };
            real data[4];
        };
        
    public:
        Vector(real x = 0, real y = 0, real z = 0, real w = 0):
        x(x), y(y), z(z), w(w){}
        
        Vector(std::initializer_list<real> v){
            auto itr = v.begin();
            x = *(itr++);
            y = *(itr++);
            z = *itr;
            w = 0;
        }
        
        Vector(const glm::vec3& v):Vector(v.x, v.y, v.z){}
        
        Vector(const glm::vec4& v):Vector(v.x, v.y, v.z, v.w){}
        
        real squreLength() const {
            return x * x + y * y + z * z;
        }
        
        real length() const{
            return real_sqrt(x * x + y * y + z * z);
        }
        
        real  operator()(){
            return length();
        }
        
        Vector& operator+=(const Vector& v){
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }
        
        Vector operator+(const Vector& v) const{
            return Vector(x + v.x, y + v.y, z + v.z);
        }
        
        Vector& operator-=(const Vector& v){
            x -= v.x;
            y -= v.y;
            z -= v.z;
            
            return *this;
        }
        
        
        Vector operator-(const Vector& v) const{
            return Vector(x - v.x, y - v.y, z - v.z);
        }

		Vector compProduct(const Vector& v) const {
			return Vector(x * v.x, y * v.y, z * v.z);
		}
        
        Vector& operator*=(real s){
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }
        
        Vector operator*(const real s) const{
            return Vector(x * s, y * s, z * s);
        }
        
        Vector& addScaled(const Vector& v, real s){
            x += v.x * s;
            y += v.y * s;
            z += v.z * s;
            return *this;
        }
        
        bool operator==(const Vector& v){
            if (this == &v) {
                return true;
            }
            
            return real_equals(x, v.x) && real_equals(y, v.y) && real_equals(z, v.z);
        }
        
        bool operator<=(const Vector& v){
            if(this == &v){
                return true;
            }
            
            return x <= v.x && y <= v.y && z <= v.z;
        }
        
        Vector& normalize(){
            real mag = length();
            
            *this *= 1/mag;
            
            return *this;
        }
        
        real operator[](const char key) const{
            switch(key){
                case 'x': return x;
                case 'y': return y;
                case 'z': return z;
                default: throw "Illegal argument";
            }
        }

		real& operator[](const char key){
			switch (key) {
			case 'x': return x;
			case 'y': return y;
			case 'z': return z;
			default: throw "Illegal argument";
			}
		}

		real operator[](const unsigned i) const {
			switch (i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw "Illegal argument";
			}
		}

		real& operator[](const unsigned i) {
			switch (i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw "Illegal argument";
			}
		}

        
        real dot(const Vector& v) const{
            return x * v.x + y * v.y + z * v.z;
        }
        
        Vector cross(const Vector& v) const{
            return Vector{
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            };
        }
        
        Vector& crossProductUpdate(const Vector& v){
            *this = cross(v);
            
            return *this;
        }
        
        real operator%(const Vector& v){
            return dot(v);
        }
        
        Vector operator*(const Vector& v) const{
            return cross(v);
        }
        
        Vector& operator*=(const Vector& v){
            return crossProductUpdate(v);
        }
        
        Vector& invert(){
            x *= -1;
            y *= -1;
            z *= -1;
            return *this;
        }
        
        Vector& operator-(){
            invert();
            return *this;
        }
        
        operator real*(){
            return data;
        }
        
        Vector& clear(){
            x = 0;
            y = 0;
            z = 0;
            w = 0;
            return *this;
        }
        
        static real angleBetween(Vector a, Vector b){
            a.normalize();
            b.normalize();
            real dot = a.dot(b);
            return real_acos(dot)/DEG_TO_RAD;
            
        }
        
        friend std::ostream& operator<<(std::ostream& out, const Vector& v){
            out << "v(" << v.x << ", " << v.y << ", " << v.z;
            if(!(fabs(v.w - 0) < epsilon)) out << "," << v.w;
            out << ")";
            return out;
        }
        
        operator glm::vec4(){
            return glm::vec4{x, y, z, w};
        }
        
        operator glm::vec3(){
            return glm::vec3(x, y, z);
        }
        
    };
}

#endif
