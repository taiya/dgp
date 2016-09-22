#pragma once
#include <OpenGP/types.h>
using namespace OpenGP;

struct ImplicitSphere{
    Vec3 center = Vec3(0,0,0);
    Scalar r = .5; //< radius
    
    Scalar eval(const Vec3& x){
        return ((x - center).squaredNorm() - r*r);
    }    
};
