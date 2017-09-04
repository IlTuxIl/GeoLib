//
// Created by Julien on 04/09/2017.
//

#ifndef GEOLIB_VECTOR_H
#define GEOLIB_VECTOR_H

#include <fstream>

class vector2 {

public:

    float x,y;

    vector2(float, float );
    vector2(const vector2&);

    vector2 operator+(const vector2&);
    vector2 operator-(const vector2&);
    vector2 operator*(float);
    vector2 operator/(float);
    vector2 operator=(const vector2&);
    std::ostream& operator<< (std::ostream&);

    float dot(const vector2&);
    float length();
    float length2();
    vector2 normalize();
};

class vector3 {

public:

    float x,y,z;

    vector3(float, float, float);
    vector3(const vector2&);
    vector3(const vector3&);

    vector3 operator+(const vector3&);
    vector3 operator-(const vector3&);
    vector3 operator*(float);
    vector3 operator/(float);
    vector3 operator=(const vector3&);
    std::ostream& operator<< (std::ostream&);

    float dot(const vector3&);
    vector3 cross(const vector3&);
    float length();
    float length2();
    vector3 normalize();
};

#endif //GEOLIB_VECTOR_H
