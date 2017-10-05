//
// Created by julien on 05/09/17.
//

#include "../include/Triangle.h"

Triangle::Triangle(int s1, int s2, int s3) {
    v[0] = s1;
    v[1] = s2;
    v[2] = s3;
}

int Triangle::getIdSommet(int id) const {
    if(id == -1)
        return v[2];
    return v[id%3];
}

void Triangle::setIdSommet(int sommet, int index) {
    v[index%3] = sommet;
}

std::ostream &operator<<(std::ostream &os, const Triangle &t) {
    os << t.getIdSommet(0) << " " << t.getIdSommet(1) << " " << t.getIdSommet(2) << std::endl;
    return os;
}