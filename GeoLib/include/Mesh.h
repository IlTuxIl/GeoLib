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

//    vector3 getFaces(int) const;

    int getNbVertex() const;

    int getNbFaces() const;

    TriangleTopo& getTriangles(int index);

    void draw();

    bool appartient(int idTri, int idPoint);

    double appartientCercle(int idTri, int s) const;

    ~Mesh() = default;
protected:
    void splitTriangle(int idTri, int idSommet);
    void linkTriangle(int idNewTriangle, int idOldTriangle, const couple& c);
    couple getArreteAdjacent(const TriangleTopo& t1, const TriangleTopo& t2);
    couple getPointsAdjacent(const TriangleTopo& t1, const TriangleTopo& t2);
    std::vector<Sommet> vertex;

    int nbVertex;
    int nbFaces;

    std::vector<TriangleTopo> triangles;
};



#endif //GEOLIB_MESH_H
