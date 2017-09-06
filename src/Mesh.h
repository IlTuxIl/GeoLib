//
// Created by Julien on 04/09/2017.
//

#ifndef GEOLIB_MESH_H
#define GEOLIB_MESH_H

#include <vector>
#include "Vector.h"
#include "Triangle.h"

class Mesh {
public:
    bool loadOFF(char*);

    bool saveOFF(char*);

    vector3 getVertex(int) const;

    vector3* getVertex();

    Triangle* getTriangle(int);

    int* getFaces(int) const;

    int getNbVertex() const;

    int getNbFaces() const;

    void draw();

    void drawAll();

private:
    vector3* vertex;

    int** faces;

    int nbVertex;

    int nbFaces;
};


#endif //GEOLIB_MESH_H
