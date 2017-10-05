//
// Created by julien on 08/09/17.
//

#ifndef GEOLIB_TRIANGULATION_H
#define GEOLIB_TRIANGULATION_H


#include "Mesh.h"

class faceCirculator{
public:
    faceCirculator(Mesh* _mesh, int idSommet);

    void nextFace();
    int getIndex();
    TriangleTopo* getFace();
    TriangleTopo* operator*();

    faceCirculator operator++(int);
    bool operator!=(const faceCirculator& fc);
private:
    Mesh* mesh;
    bool changeTriangle;
    TriangleTopo* curTriangle;
    int curIndex;
    int startIndex;
};

class vertexCirculator{
public:
    vertexCirculator(Mesh* _mesh, int idSommet);

    void nextVertex();
    int getIndex();
    Sommet* getVertex();
    Sommet* operator*();

    vertexCirculator operator++(int);
    bool operator!=(const vertexCirculator& vc);
private:
    bool changeTriangle;
    Mesh* mesh;
    TriangleTopo* curTriangle;
    int curIndex;
    int startIndex;
};

class vertexIterator{
public:
    vertexIterator(Mesh* _mesh, int startIndex);

    void nextVertex();

    Sommet* getVertex();
    Sommet* operator*();

    vertexIterator operator++(int);
    vertexIterator operator--(int);
    bool operator<(const vertexIterator& fi);

private:
    Mesh* mesh;
    int index = 0;
};

class faceIterator{
public:
    faceIterator(std::vector<TriangleTopo>* tab, int startIndex);

    void nextFace();

    TriangleTopo* getFace();
    TriangleTopo* operator*();

    faceIterator operator++(int);
    faceIterator operator--(int);
    bool operator<(const faceIterator& fi);

private:
    std::vector<TriangleTopo>* tabTri;
    int index = 0;
};

class faceExtIterator{
public:
    faceExtIterator(std::vector<TriangleTopo>* tri, std::vector<int>* tab, int startIndex);

    void nextFace();

    TriangleTopo* getFace();
    TriangleTopo* operator*();

    faceIterator operator++(int);
    faceIterator operator--(int);
    bool operator<(const faceExtIterator& fi);

private:
    std::vector<TriangleTopo>* tabTriangle;
    std::vector<int>* tabIndex;
    int index = 0;
};

class Triangulation : public Mesh {
public:
    bool loadPoints(char* filename);
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
    Sommet* getVertex();
    int* getIndex();

private:
    void addPointDelaunay(int idPoint);
    int appartientMesh(int idPoint);
    std::vector<int> idExterieur;
};

#endif //GEOLIB_TRIANGULATION_H
