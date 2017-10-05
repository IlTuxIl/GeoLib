//
// Created by Julien on 04/09/2017.
//

#ifndef GEOLIB_VECTOR_H
#define GEOLIB_VECTOR_H

#include <fstream>

class vector2 {

public:

    float x,y;

    vector2(float x = 0, float y = 0);
    vector2(const vector2&);

    vector2 operator+(const vector2&);
    vector2 operator-(const vector2&);
    vector2 operator*(float);
    vector2 operator/(float);
    vector2& operator=(const vector2&);
    bool operator==(const vector2&);
    friend std::ostream& operator<< (std::ostream&, const vector2&);

    float dot(const vector2&);
    double length();
    double length2();
    vector2 normalize();
};

class vector3 {

public:

    double x,y,z;

    vector3(double x = 0, double y = 0, double z = 0);
    vector3(const vector2&);
    vector3(const vector3&);

    vector3 operator+(const vector3&);
    vector3 operator-(const vector3&);
    vector3 operator*(float);
    vector3 operator/(float);
    vector3& operator=(const vector3&);
    bool operator==(const vector3&);
    double& operator[](int);
    friend std::ostream& operator<< (std::ostream&, const vector3&);

    double dot(const vector3&) const;
    vector3 cross(const vector3&) const;
    double length();
    double length2();
    vector3 normalize();
};

#endif //GEOLIB_VECTOR_H