//
//  core.h
//  opengl
//
//  Created by Josiah Ebhomenye on 03/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef openglcoreh
#define openglcoreh

#include <sstream>
#include <string>
#include "precision.h"
#include "vector.h"
#include "Quaternion.h"
#include "Matrix3.h"
#include "Matrix3X4.h"
#include "Matrix4.h"


namespace physics {
    
    using Position = Vector;
    using Velocity = Vector;
    using Acceleration = Vector;
    using Force = Vector;
	using Torque = Vector;
	using Point = Vector;
    
    
    Matrix3 IdentityMatrix3{
        {1, 0, 0},
        {0, 1, 0},
        {0, 1, 0}
    };
    
    Matrix4 IdentityMatrix4{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    
    Matrix4 translate(const Vector v){
        return {
            {1, 0, 0, v.x},
            {0, 1, 0, v.y},
            {0, 0, 1, v.z},
            {0, 0, 0, 1}
        };
    }
    
    Matrix4 rotate(const real angle, const Vector& axis){
        real s = real_sin(angle * DEG_TO_RAD);
        real c = real_cos(angle * DEG_TO_RAD);
        real t = 1 - c;
        real x = axis.x;
        real y = axis.y;
        real z = axis.z;
        
        return Matrix4{
            {t * x * x + c,      t * x * y + s * z,  t * x * z - s * y,  0},
            {t * x * y - s * z,  t * y * y + c,      t * y * z + s * x,  0},
            {t * x * z + s * y,  t * y * z - s * x,  t * z * z + c,      0},
            {        0,                  0,                   0,         1}
        };
    }
    
    Matrix4 rotateZ(const real angle){
        return rotate(angle, {0, 0, 1.0});
    }
    
    Matrix4 rotateX(const real angle){
        return rotate(angle, {1.0, 0, 0});
    }
    
    Matrix4 rotateY(const real angle){
        return rotate(angle, {0, 1.0, 0});
    }
    
    
    
    Matrix4 RotationMatrix(const real yaw, const real pitch, const real roll){
        return rotateY(yaw) * rotateX(pitch) * rotateZ(roll);
    }
    

}

physics::Quaternion operator"" _q(const char* literal, size_t n){
    std::stringstream ss{ literal };
    physics::real r, i, j, k;
    ss >> r;
    ss >> i;
    ss >> j;
    ss >> k;
    
    return physics::Quaternion{r, i, j, k};
}

physics::Vector operator"" _v(const char* literal, size_t n){
    std::stringstream ss{ literal };
    physics::real x, y, z;
    ss >> x;
    ss >> y;
    ss >> z;
    
    return {x, y, z};
}

#endif
