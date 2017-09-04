//
// Created by Julien on 04/09/2017.
//

#include <cmath>
#include "Vector.h"

vector2::vector2(float _x = 0, float _y = 0) {
    x = _x;
    y = _y;
}

vector2::vector2(const vector2& vec) {
    x = vec.x;
    y = vec.y;
}

vector2 vector2::operator+(const vector2& vec) {
    return vector2(x + vec.x, y + vec.y);
}

vector2 vector2::operator-(const vector2& vec) {
    return vector2(x - vec.x, y - vec.y);
}

vector2 vector2::operator*(float k) {
    return vector2(x * k, y * k);
}

vector2 vector2::operator/(float k) {
    return vector2(x / k, y / k);
}

vector2 vector2::operator=(const vector2& vec) {
    return vector2(vec.x, vec.y);
}

std::ostream &vector2::operator<<(std::ostream& o) {
    o << x << " " << y << std::endl;
    return o;
}

float vector2::dot(const vector2& vec) {
    return x*vec.x + y*vec.y;
}

float vector2::length() {
    return sqrt(length2());
}

float vector2::length2() {
    return x*x + y*y;
}

vector2 vector2::normalize() {
    float k = 1 / length();
    return vector2(x * k, y * k);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector3::vector3(float x_ = 0, float y_ = 0, float z_ = 0) {
    x = x_;
    y = y_;
    z = z_;
}

vector3::vector3(const vector2& vec) {
    x = vec.x;
    y = vec.y;
    z = 0;
}

vector3::vector3(const vector3& vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

vector3 vector3::operator+(const vector3& vec) {
    return vector3(x + vec.x, y + vec.y, z + vec.z);
}

vector3 vector3::operator-(const vector3& vec) {
    return vector3(x - vec.x, y - vec.y, z - vec.z);
}

vector3 vector3::operator*(float k) {
    return vector3(x * k, y * k, z * k);
}

vector3 vector3::operator/(float k) {
    return vector3(x / k, y / k, z / k);
}

vector3 vector3::operator=(const vector3& vec) {
    return vector3(vec.x, vec.y, vec.z);
}

std::ostream &vector3::operator<<(std::ostream& o) {
    o << x << " " << y << " " << z << std::endl;
    return o;
}

float vector3::dot(const vector3& vec) {
    return x*vec.x + y*vec.y + z*vec.z;
}

vector3 vector3::cross(const vector3& vec) {
    return vector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}

float vector3::length() {
    return sqrt(length2());
}

float vector3::length2() {
    return  x*x + y*y + z*z;
}

vector3 vector3::normalize() {
    float k = 1 / length();
    return vector3(x * k, y * k, z * k);
}
