//
// Created by julien on 08/09/17.
//

#ifndef GEOLIB_TRIANGULATION_H
#define GEOLIB_TRIANGULATION_H


#include "Mesh.h"
#include "Iterateur.h"

class Triangulation : public Mesh {
public:
    bool loadPoints(char* filename);
    bool loadPointsDelaunay(char* filename);

    faceIterator faceBegin();
    faceIterator faceEnd();
    faceExtIterator faceExtBegin();
    faceExtIterator faceExtEnd();
    vertexIterator vertexBegin();
    vertexIterator vertexEnd();
    faceCirculator faceAround(int p);
    vertexCirculator vertexAround(int p);
    bool flipTriangle(int tri1, int tri2);
    void makeDelaunay();
    std::vector<float> getVertex();
    std::vector<unsigned int> getIndex();
    std::vector<float> getVoronoi();


    void addPoint(float x, float y);
    std::vector<int> idExterieur;

private:
    void checkExterieur(int idTri);
    void addPointDelaunay(int idPoint);
    int appartientMesh(int idPoint);
};

#endif //GEOLIB_TRIANGULATION_H
