//
//  precision.h
//  physics
//
//  Created by Josiah Ebhomenye on 30/11/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef physics_precision_h
#define physics_precision_h

#include <cmath>
#include <cfloat>
#include <functional>
#include <limits>

namespace physics {
    using real = float;
    using imaginary = float;
    const static real epsilon = 0.001f;
    const static real epsilon2 = epsilon * epsilon;
    
    const static real TWOPI = 6.283185307179586476925287;
    const static real PI = 3.141592653589793238462643;
    const static real DEG_TO_RAD = 0.0174532925;
    
    std::function<real(real)> real_sqrt =  sqrtf;
    std::function<real(real, real)> real_pow = powf;
    std::function<real(real)> real_cos = cosf;
    std::function<real(real)> real_sin = sinf;
    std::function<real(real)> real_tan = tanf;
    std::function<real(real)> real_acos = acosf;
    std::function<real(real)> real_asin = asinf;
    std::function<real(real, real)> real_atan2 = atan2f;
    
    std::numeric_limits<real> real_limits;
    
    real REAL_MAX = real_limits.max();
    real REAL_MIN = real_limits.min();
    
    bool real_equals(real a, real b){
        return a == b || a - b < epsilon;
    }
    
    
    
}

#endif

