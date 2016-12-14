//
//  Quarternion.h
//  opengl
//
//  Created by Josiah Ebhomenye on 14/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Quarternion_h
#define opengl_Quarternion_h

#include <iostream>
#include "vector.h"

namespace physics {
    class Quarternion{
    public:
        union{
            struct{
                real r;
                imaginary i;
                imaginary j;
                imaginary k;
            };
            real data[4];
        };
        
        Quarternion(real r = 0, imaginary i = 0, imaginary j = 0, imaginary k = 0)
        :r(r), i(i), j(j), k(k){}
        
        Quarternion(const Vector& v): Quarternion(0, v.x, v.y, v.z){
            
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
        
        Quarternion& operator*=(const Quarternion& q){
            r = r * q.r - i * q.i - j * q.j - k * q.k;
            i = r * q.i + i * q.r + j * q.k - k * q.j;
            j = r * q.j - i * q.k + j * q.r + k * q.i;
            k = r * q.k + i * q.j - j * q.i + k * q.r;
            
            return *this;
        }
        
        Quarternion& rotateBy(const Vector& v){
            (*this) *= v;
            return *this;
        }
        
        Quarternion& addScaled(const Vector& v, real s){
            Quarternion q (0, v.x * s, v.y * s, v.z * s);
            
            q *= *this;
            real half = real(0.5);
            r += q.r * half;
            i += q.i * half;
            j += q.j * half;
            k += q.k * half;
            
            return *this;
        }
        
        friend std::ostream& operator<<(std::ostream& out, const Quarternion& q){
            out << "q(" << q.r << ", " <<  q.i << "i, " << q.j << "j, " << q.k << "k)";
            return out;
        }
    };

}

#endif
