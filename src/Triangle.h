//
// Created by julien on 05/09/17.
//

#ifndef GEOLIB_TRIANGLE_H
#define GEOLIB_TRIANGLE_H

#include "Vector.h"

class Triangle {
public:
    Triangle();
    Triangle(const vector3& vec1, const vector3& vec2, const vector3& vec3);
    Triangle(const Triangle& tri);
    Triangle operator=(const Triangle& tri);
    vector3 v1, v2, v3;
};


#endif //GEOLIB_TRIANGLE_H
