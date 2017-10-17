//
// Created by julien on 17/10/17.
//

#include "../include/Sommet.h"

namespace GeoLib{

    Sommet::Sommet(const Triplet &vec) {
        setX(vec.x());
        setY(vec.y());
        setZ(vec.z());
        idTriangle = -1;
    }

    Sommet::Sommet(double x, double y, double z){
        val[0] = x;
        val[1] = y;
        val[2] = z;
        idTriangle = -1;
    }

    Sommet& Sommet::operator=(const Triplet &vec) {
        setX(vec.x());
        setY(vec.y());
        setZ(vec.z());
        idTriangle = -1;
        return *this;
    }

    Sommet& Sommet::operator=(const Sommet &vec) {
        setX(vec.x());
        setY(vec.y());
        setZ(vec.z());
        idTriangle = vec.getIdTriangle();
        return *this;
    }

    Sommet::Sommet(const Sommet &_sommet) {
        val[0] = _sommet.val[0];
        val[1] = _sommet.val[1];
        val[2] = _sommet.val[2];
        idTriangle = _sommet.idTriangle;
    }

    Sommet Sommet::operator-(const Sommet &s) {
        return Sommet(x() - s.x(), y() - s.y(), z() - s.z());
    }

    std::ostream& operator<<(std::ostream& os, const Sommet& vec) {
        os << vec.x() << ' ' << vec.y() << ' ' << vec.z() << std::endl;
        return os;
    }
}