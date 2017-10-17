//
// Created by julien on 17/10/17.
//

#include "../include/Vector.h"
#include <cmath>

namespace GeoLib{

    vector3::vector3(double x_, double y_, double z_) {
        setX(x_);
        setY(y_);
        setZ(z_);
    }

    vector3::vector3(const vector3& vec) {
        setX(vec.x());
        setY(vec.y());
        setZ(vec.z());
    }

    vector3::vector3(const Sommet &s) {
        setX(s.x());
        setY(s.y());
        setZ(s.z());
    }

    vector3 vector3::operator+(const vector3& vec) {
        return vector3(x() + vec.x(), y() + vec.y(), z() + vec.z());
    }

    vector3 vector3::operator-(const vector3& vec) {
        return vector3(x() - vec.x(), y() - vec.y(), z() - vec.z());
    }

    vector3 vector3::operator*(float k) {
        return vector3(x() * k, y() * k, z() * k);
    }

    vector3 vector3::operator/(float k) {
        return vector3(x() / k, y() / k, z() / k);
    }

    vector3& vector3::operator=(const vector3& vec) {
        setX(vec.x());
        setY(vec.y());
        setZ(vec.z());
        return *this;
    }

    bool vector3::operator==(const vector3& vec) {
        return x() == vec.x() && y() == vec.y() && z() == vec.z();
    }

    double vector3::dot(const vector3& vec) const {
        return x()*vec.x() + y()*vec.y() + z()*vec.z();
    }

    vector3 vector3::cross(const vector3& vec) const {
        return vector3(y() * vec.z() - z() * vec.y(), z() * vec.x() - x() * vec.z(), x() * vec.y() - y() * vec.x());
    }

    double vector3::length() {
        return sqrt(length2());
    }

    double vector3::length2() {
        return  x()*x() + y()*y() + z()*z();
    }

    vector3 vector3::normalize() {
        float k = 1 / length();
        return vector3(x() * k, y() * k, z() * k);
    }

    double& vector3::operator[](int i) {
        return Triplet::operator[](i);
    }

    std::ostream& operator<<(std::ostream& os, const vector3& vec) {
        os << vec.x() << ' ' << vec.y() << ' ' << vec.z() << std::endl;
        return os;
    }
}