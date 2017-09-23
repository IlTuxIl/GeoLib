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
    faceIterator(Mesh* _mesh, int startIndex);

    void nextFace();

    TriangleTopo* getFace();
    TriangleTopo* operator*();

    faceIterator operator++(int);
    faceIterator operator--(int);
    bool operator<(const faceIterator& fi);

private:
    Mesh* mesh;
    int index = 0;
};

class Triangulation : public Mesh {
public:
    bool loadPoints(char* filename);
    faceIterator faceBegin();
    faceIterator faceEnd();
    vertexIterator vertexBegin();
    vertexIterator vertexEnd();
    faceCirculator faceAround(int p);
    vertexCirculator vertexAround(int p);


private:
    void genMesh();

};



#endif //GEOLIB_TRIANGULATION_H
