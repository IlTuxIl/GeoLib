//
// Created by Julien on 04/09/2017.
//

#include <cmath>
#include "../include/Vector.h"

vector2::vector2(float _x, float _y) {
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

vector2& vector2::operator=(const vector2& vec) {
    x = vec.x;
    y = vec.y;
    return *this;
}

bool vector2::operator==(const vector2& vec) {
    return x == vec.x && y == vec.y;
}

float vector2::dot(const vector2& vec) {
    return x*vec.x + y*vec.y;
}

double vector2::length() {
    return sqrt(length2());
}

double vector2::length2() {
    return x*x + y*y;
}

vector2 vector2::normalize() {
    float k = 1 / length();
    return vector2(x * k, y * k);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector3::vector3(double x_, double y_, double z_) {
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

vector3& vector3::operator=(const vector3& vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
}

bool vector3::operator==(const vector3& vec) {
    return x == vec.x && y == vec.y && z == vec.z;
}

double vector3::dot(const vector3& vec) const {
    return x*vec.x + y*vec.y + z*vec.z;
}

vector3 vector3::cross(const vector3& vec) const {
    return vector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}

double vector3::length() {
    return sqrt(length2());
}

double vector3::length2() {
    return  x*x + y*y + z*z;
}

vector3 vector3::normalize() {
    float k = 1 / length();
    return vector3(x * k, y * k, z * k);
}

double& vector3::operator[](int i) {
    switch(i){
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return x;
    }
}

std::ostream& operator<<(std::ostream& os, const vector2& vec){
    os << vec.x << ' ' << vec.y << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const vector3& vec) {
    os << vec.x << ' ' << vec.y << ' ' << vec.z << std::endl;
    return os;
}