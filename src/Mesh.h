//
// Created by Julien on 04/09/2017.
//

#ifndef GEOLIB_MESH_H
#define GEOLIB_MESH_H

#include <vector>
#include "Vector.h"
#include "TriangleTopo.h"

struct couple{
    int p1,p2;
    bool operator< (const couple& c2) const
    {return p1 < c2.p1 || p2 < c2.p2;}
};

class Mesh {
public:
    bool loadOFF(char*);

    bool saveOFF(char*);

    Sommet& getVertex(int);

    vector3 getFaces(int) const;

    int getNbVertex() const;

    int getNbFaces() const;

    TriangleTopo& getTriangles(int index);

    void draw();

    void drawAll();

    bool appartient(int idTri, int idPoint);


    ~Mesh();
protected:
    void splitTriangle(int idTri, int idSommet);
    void linkTriangle(int idNewTriangle, int idOldTriangle, const couple& c);

    std::vector<Sommet> vertex;
    std::vector<vector3> faces;

    int nbVertex;
    int nbFaces;

    std::vector<TriangleTopo> triangles;
    TriangleTopo infinite;
};



#endif //GEOLIB_MESH_H
