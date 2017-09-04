//
// Created by Julien on 04/09/2017.
//

#ifndef GEOLIB_MESH_H
#define GEOLIB_MESH_H

#include <vector>
#include "Vector.h"

class Mesh {
public:
    bool loadOFF(char*);

    vector3 getVertex(int) const;

    int* getFaces(int) const;

    int getNbVertex() const;

    int getNbFaces() const;

private:
    vector3* vertex;

    int** faces;

    int nbVertex;

    int nbFaces;
};


#endif //GEOLIB_MESH_H
