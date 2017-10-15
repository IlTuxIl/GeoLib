//
// Created by julien on 15/10/17.
//

#include <iostream>
#include "Maillage2D.h"


Maillage2D::Maillage2D(const Maillage2D &m) {
    for(int i : m.segments)
        segments.push_back(i);
    for(Sommet s : m.Vertex)
        Vertex.push_back(s);
}

Maillage2D &Maillage2D::operator=(const Maillage2D &m) {

    for(int i : m.segments)
        segments.push_back(i);
    for(Sommet s : m.Vertex)
        Vertex.push_back(s);

    return *this;
}


std::vector<float> Maillage2D::getArretes() {
    std::vector<float> ret;
    ret.reserve(segments.size() * 3);

    for(int p : segments){
        Sommet s = Vertex[p];

        ret.push_back(s[0]);
        ret.push_back(s[1]);
        ret.push_back(s[2]);
//        std::cout << s;
    }

    return ret;
}
