//
//  Quarternion.h
//  opengl
//
//  Created by Josiah Ebhomenye on 14/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Quaternion_h
#define opengl_Quaternion_h

#include <iostream>
#include "vector.h"

namespace physics {
    class Quaternion{
    public:
        union{
            struct{
                real r;
                imaginary i;
                imaginary j;
                imaginary k;
            };
            struct{
                real w;
                real x;
                real y;
                real z;
            };
            real data[4];
        };
        
		Quaternion(real r = 0, imaginary i = 0, imaginary j = 0, imaginary k = 0)
        :r(r), i(i), j(j), k(k){}
        
		Quaternion(const Vector& v): Quaternion(0, v.x, v.y, v.z){
            
        }
        
        Quaternion& operator=(const Quaternion& q){
            r = q.r;
            i = q.i;
            j = q.j;
            k = q.k;
            
            return *this;
        }

        
        void normailize(){
            real d = r * r + i * i + j * j + k * k;
            
            if(d == 0) {
                r = 1;
                return;
            }
            
            d = real(1)/real_sqrt(d);
            
            r *= d;
            i *= d;
            j *= d;
            k *= d;
        }
        
        Quaternion operator*(const Quaternion& q){
            return Quaternion{
                r * q.r - i * q.i - j * q.j - k * q.k,
                r * q.i + i * q.r + j * q.k - k * q.j,
                r * q.j - i * q.k + j * q.r + k * q.i,
                r * q.k + i * q.j - j * q.i + k * q.r
            };
        }
        
        
		Quaternion& operator*=(const Quaternion& q){
            real r1 = r * q.r - i * q.i - j * q.j - k * q.k;
            real i1 = r * q.i + i * q.r + j * q.k - k * q.j;
            real j1 = r * q.j - i * q.k + j * q.r + k * q.i;
            real k1 = r * q.k + i * q.j - j * q.i + k * q.r;
            
            r = r1; i = i1; j = j1; k = k1;
            
            return *this;
        }
        
		Quaternion& rotateBy(const Vector& v){
            (*this) *= v;
            return *this;
        }
        
		Quaternion& addScaled(const Vector& v, real s){
			Quaternion q (0, v.x * s, v.y * s, v.z * s);
            
            q *= *this;
            real half = real(0.5);
            r += q.r * half;
            i += q.i * half;
            j += q.j * half;
            k += q.k * half;
            
            return *this;
        }
        
        friend std::ostream& operator<<(std::ostream& out, const Quaternion& q){
            out << "q(" << q.r << ", " <<  q.i << "i, " << q.j << "j, " << q.k << "k)";
            return out;
        }
    };

}

#endif
