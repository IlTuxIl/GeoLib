//
// Created by julien on 07/09/17.
//

#include "TriangleTopo.h"

TriangleTopo::TriangleTopo(int _s1, int _s2, int _s3, int _id) {
    v[0] = _s1;
    v[1] = _s2;
    v[2] = _s3;

    id = _id;
}

void TriangleTopo::setNeighbor(int tri, int id) {
    t[id%3] = tri;
}

int TriangleTopo::getNeighbor(int id) const {
    if(id == -1)
        return t[2];
    return t[id%3];
}

int TriangleTopo::estExterieur() {
    for(int i = 0; i < 3; i++){
        if(t[i] == 0)
            return i;
    }
    return -1;
}

int TriangleTopo::getIndexInTriangle(int idSommet) {
    for(int i = 0; i < 3; i++){
        if(v[i] == idSommet)
            return i;
    }
    return -1;
}

std::ostream &operator<<(std::ostream &os, const TriangleTopo &t) {
    os << t.getIdSommet(0) << " " << t.getIdSommet(1) << " " << t.getIdSommet(2) << std::endl;
    return os;
}
