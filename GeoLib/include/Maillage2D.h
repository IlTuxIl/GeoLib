//
// Created by julien on 15/10/17.
//

#ifndef GEOALGO_MAILLAGE2D_H
#define GEOALGO_MAILLAGE2D_H


#include "Mesh.h"

class Maillage2D {
public:
    Maillage2D() = default;
    Maillage2D(const Maillage2D& m);
    Maillage2D(std::vector<int> couple, std::vector<Sommet> points){segments = couple; Vertex = points;}
    std::vector<float> getArretes();

    Maillage2D&operator=(const Maillage2D&);
    std::vector<int> segments;
    std::vector<Sommet> Vertex;
private:
};


#endif //GEOALGO_MAILLAGE2D_H
