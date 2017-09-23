//
// Created by julien on 07/09/17.
//

#include "Sommet.h"

Sommet::Sommet(const vector3 &vec) : vector3(vec) {

}

Sommet Sommet::operator=(const vector3 &vec) {
    return Sommet(vec);
}

Sommet Sommet::operator=(const Sommet &vec) {
    Sommet ret(vec);
    return ret;
}

Sommet::Sommet(const Sommet &_sommet) {
    x = _sommet.x;
    y = _sommet.y;
    z = _sommet.z;
    idTriangle = _sommet.idTriangle;
}

std::ostream& operator<<(std::ostream& os, const Sommet& vec) {
    os << vec.x << ' ' << vec.y << ' ' << vec.z << std::endl;
    return os;
}