//
// Created by julien on 08/09/17.
//

#include <iostream>
#include "Triangulation.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

vertexIterator::vertexIterator(Mesh *_mesh, int startIndex) {
    mesh = _mesh;
    index = startIndex;
}

void vertexIterator::nextVertex() {
    index++;
    if(index >= mesh->getNbVertex())
        index = mesh->getNbVertex();
}

Sommet* vertexIterator::getVertex() {
    if(index >= 0 && index < mesh->getNbVertex())
        return &(mesh->getVertex(index));
    return new Sommet();
}

Sommet* vertexIterator::operator*() {
    if(index >= 0 && index < mesh->getNbVertex())
        return &(mesh->getVertex(index));
    return new Sommet();}

vertexIterator vertexIterator::operator++(int) {
    index++;
    if(index >= mesh->getNbVertex())
        index = mesh->getNbVertex();
    return *this;
}

vertexIterator vertexIterator::operator--(int) {
    index--;
    if(index < 0)
        index = 0;
    return *this;
}

bool vertexIterator::operator<(const vertexIterator &fi) {
    return index<fi.index;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

faceIterator::faceIterator(Mesh *_mesh, int startIndex) {
    mesh = _mesh;
    index = startIndex;
}

void faceIterator::nextFace() {
    index++;
    if(index > mesh->getNbFaces())
        index = mesh->getNbFaces();
}

TriangleTopo* faceIterator::operator*() {
    if(index < mesh->getNbFaces() && index >= 0)
        return &mesh->getTriangles(index);
    else
        return new TriangleTopo();
}

TriangleTopo* faceIterator::getFace() {
    if(index < mesh->getNbFaces() && index >= 0) {
        return &mesh->getTriangles(index);
    }
    else {
        return new TriangleTopo();
    }
}

faceIterator faceIterator::operator++(int) {
    index = index + 1;
    if(index > mesh->getNbFaces())
        index = mesh->getNbFaces();
    return *this;
}

faceIterator faceIterator::operator--(int i) {
    index = index - 1;
    if(index < 0)
        index = 0;
    return *this;
}

bool faceIterator::operator<(const faceIterator &fi) {
    return index < fi.index;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

faceIterator Triangulation::faceBegin() {
    return faceIterator(this, 0);
}

faceIterator Triangulation::faceEnd() {
    return faceIterator(this, getNbFaces());
}

vertexIterator Triangulation::vertexBegin() {
    return vertexIterator(this, 0);
}

vertexIterator Triangulation::vertexEnd() {
    return vertexIterator(this, getNbVertex());
}

faceCirculator Triangulation::faceAround(int p) {
    return faceCirculator(this, p);
}

vertexCirculator Triangulation::vertexAround(int p) {
    return vertexCirculator(this, p);
}

bool estTrigo(const vector3& a, const vector3& b){
    return a.cross(b).z > 0;
}

bool Triangulation::loadPoints(char *filename) {
    std::ifstream ifs;
    ifs.open(filename);
    if(ifs.bad()){
        std::cout<<"Impossible d'ouvrir le fichier "<<filename<<" en lecture \n";
        return false;
    }
    ifs >> nbVertex;
    vertex.reserve(nbVertex);
    triangles.reserve(nbVertex);
    faces.reserve(nbVertex);

    for(int i = 0; i < nbVertex; i ++){
        Sommet s;
        ifs >> s.x >> s.y >> s.z;
        vertex.push_back(s);
    }

    ifs.close();

    //1er Triangle
    if(estTrigo((vertex[0] - vertex[1]), (vertex[0] - vertex[2]))){
        faces.push_back(vector3(0,1,2));
        TriangleTopo t(0,1,2,1);
        triangles.push_back(t);
    }
    else{
        faces.push_back(vector3(0,2,1));
        TriangleTopo t(0,2,1,2);
        triangles.push_back(t);
    }
    //tous les autres points
    for(int i = 3; i < nbVertex; i++){
        if(appartient(1, i)){
            splitTriangle(1, i);
            std::cout << "check " << faces[0] << faces[1] << faces[2] << std::endl;
            std::cout << "check " << triangles[0] << triangles[1] << triangles[2] << std::endl;
        }
        else{
            std::cout << "non" << std::endl;
        }
    }

}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


vertexCirculator::vertexCirculator(Mesh *_mesh, int _index) {
    mesh = _mesh;
    changeTriangle = true;
    if (_index >= 0 && _index < mesh->getNbVertex()) {
        Sommet s = mesh->getVertex(_index);
        curTriangle = &mesh->getTriangles(s.getIdTriangle()-1);
        curIndex = curTriangle->getIdSommet(curTriangle->getIndexInTriangle(_index) + 1);
        startIndex = _index;
    }
}

void vertexCirculator::nextVertex() {
    if(changeTriangle){
        curTriangle = &mesh->getTriangles(curTriangle->getNeighbor(curTriangle->getIndexInTriangle(startIndex) - 1) - 1);

        if(curTriangle->getId() == 0){
            changeTriangle = false;
            Sommet s = mesh->getVertex(startIndex);
            TriangleTopo* tmpTri = &mesh->getTriangles(s.getIdTriangle()-1);

            while(tmpTri->getId() != 0){
                curTriangle = tmpTri;
                tmpTri = &mesh->getTriangles(tmpTri->getNeighbor(tmpTri->getIndexInTriangle(startIndex) + 1) - 1);
            }
            curIndex = curTriangle->getIdSommet(curTriangle->getIndexInTriangle(startIndex) - 1);
        }
        else {
            curIndex = curTriangle->getIdSommet(curTriangle->getIndexInTriangle(startIndex) + 1);
        }
    }
    else{
        changeTriangle = true;
        curIndex = curTriangle->getIdSommet(curTriangle->getIndexInTriangle(startIndex) + 1);
    }
}

Sommet *vertexCirculator::operator*() {
    if(curIndex < mesh->getNbFaces() && curIndex >= 0)
        return &mesh->getVertex(curIndex);
    else
        return new Sommet();
}

int vertexCirculator::getIndex() {
    return curIndex;
}

Sommet *vertexCirculator::getVertex() {
    if(curIndex < mesh->getNbVertex() && curIndex >= 0)
        return &mesh->getVertex(curIndex);
    else
        return new Sommet();
}

vertexCirculator vertexCirculator::operator++(int) {
    if(changeTriangle){
        curTriangle = &mesh->getTriangles(curTriangle->getNeighbor(curTriangle->getIndexInTriangle(startIndex) - 1) - 1);

        if(curTriangle->getId() == 0){
            changeTriangle = false;
            Sommet s = mesh->getVertex(startIndex);
            TriangleTopo* tmpTri = &mesh->getTriangles(s.getIdTriangle()-1);

            while(tmpTri->getId() != 0){
                curTriangle = tmpTri;
                tmpTri = &mesh->getTriangles(tmpTri->getNeighbor(tmpTri->getIndexInTriangle(startIndex) + 1) - 1);
            }
            curIndex = curTriangle->getIdSommet(curTriangle->getIndexInTriangle(startIndex) - 1);
        }
        else {
            curIndex = curTriangle->getIdSommet(curTriangle->getIndexInTriangle(startIndex) + 1);
        }
    }
    else{
        changeTriangle = true;
        curIndex = curTriangle->getIdSommet(curTriangle->getIndexInTriangle(startIndex) + 1);
    }
}

bool vertexCirculator::operator!=(const vertexCirculator &vc) {
    return curIndex != vc.curIndex;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

faceCirculator::faceCirculator(Mesh *_mesh, int idSommet) {
    mesh = _mesh;
    if (idSommet >= 0 && idSommet < mesh->getNbVertex()) {
        Sommet s = mesh->getVertex(idSommet);
        curTriangle = &mesh->getTriangles(s.getIdTriangle()-1);
        curIndex = curTriangle->getId();
        startIndex = idSommet;
    }
}

void faceCirculator::nextFace() {
    curTriangle = &mesh->getTriangles(curTriangle->getNeighbor(curTriangle->getIndexInTriangle(startIndex) - 1) - 1);

    if(curTriangle->getId() == 0){
        Sommet s = mesh->getVertex(startIndex);
        TriangleTopo* tmpTri = &mesh->getTriangles(s.getIdTriangle()-1);

        while(tmpTri->getId() != 0){
            curTriangle = tmpTri;
            tmpTri = &mesh->getTriangles(tmpTri->getNeighbor(tmpTri->getIndexInTriangle(startIndex) + 1) - 1);
        }
    }
    curIndex = curTriangle->getId();
}

int faceCirculator::getIndex() {
    return curIndex;
}

TriangleTopo* faceCirculator::getFace() {
    if(curIndex < mesh->getNbFaces() && curIndex >= 0)
        return &mesh->getTriangles(curIndex);
    else
        return new TriangleTopo();
}

TriangleTopo* faceCirculator::operator*() {
    if(curIndex < mesh->getNbFaces() && curIndex >= 0)
        return &mesh->getTriangles(curIndex);
    else
        return new TriangleTopo();
}

faceCirculator faceCirculator::operator++(int) {
    curTriangle = &mesh->getTriangles(curTriangle->getNeighbor(curTriangle->getIndexInTriangle(startIndex) - 1) - 1);

    if(curTriangle->getId() == 0){
        Sommet s = mesh->getVertex(startIndex);
        TriangleTopo* tmpTri = &mesh->getTriangles(s.getIdTriangle()-1);

        while(tmpTri->getId() != 0){
            curTriangle = tmpTri;
            tmpTri = &mesh->getTriangles(tmpTri->getNeighbor(tmpTri->getIndexInTriangle(startIndex) + 1) - 1);
        }
    }
    curIndex = curTriangle->getId();
}

bool faceCirculator::operator!=(const faceCirculator &fc) {
    return curIndex != fc.curIndex;
}
