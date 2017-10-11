//
// Created by julien on 10/10/17.
//

#include "Iterateur.h"

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

faceIterator::faceIterator(std::vector<TriangleTopo>* tab, int startIndex) {
    tabTri = tab;
    index = startIndex;
}

void faceIterator::nextFace() {
    index++;
    if(index > tabTri->size())
        index = static_cast<int>(tabTri->size());
}

TriangleTopo* faceIterator::operator*() {
    if(index < tabTri->size() && index >= 0)
        return &(*tabTri)[index];
    else
        return new TriangleTopo();
}

TriangleTopo* faceIterator::getFace() {
    if(index < tabTri->size() && index >= 0) {
        return &(*tabTri)[index];
    }
    else {
        return new TriangleTopo();
    }
}

faceIterator faceIterator::operator++(int) {
    index = index + 1;
    if(index > tabTri->size())
        index = static_cast<int>(tabTri->size());
    return *this;
}

faceIterator faceIterator::operator--(int) {
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
    changeTriangle = true;
    if (idSommet >= 0 && idSommet < mesh->getNbVertex()) {
        Sommet s = mesh->getVertex(idSommet);
        curTriangle = &mesh->getTriangles(s.getIdTriangle()-1);
        curIndex = curTriangle->getId();
        startIndex = idSommet;
    }
}

void faceCirculator::nextFace() {
    if (curTriangle->getNeighbor(curTriangle->getIndexInTriangle(startIndex) + 1) == 0){
        TriangleTopo *tmpTri = &mesh->getTriangles(mesh->getVertex(startIndex).getIdTriangle() - 1);

        while (tmpTri->getNeighbor(tmpTri->getIndexInTriangle(startIndex) - 1) != 0) {
            tmpTri = &mesh->getTriangles(tmpTri->getNeighbor(tmpTri->getIndexInTriangle(startIndex) - 1) - 1);
            curTriangle = tmpTri;
//                std::cout << curTriangle->getId() << std::endl;
        }
        changeTriangle = false;
        curIndex = curTriangle->getId();
    } else {
        curTriangle = &mesh->getTriangles(curTriangle->getNeighbor(curTriangle->getIndexInTriangle(startIndex) + 1) - 1);
        curIndex = curTriangle->getId();
    }
}

int faceCirculator::getIndex() {
    return curIndex;
}

TriangleTopo* faceCirculator::getFace() {
    if(curIndex - 1 < mesh->getNbFaces() && curIndex - 1 >= 0)
        return &mesh->getTriangles(curIndex - 1);
    else
        return new TriangleTopo();
}

TriangleTopo* faceCirculator::operator*() {
    if(curIndex - 1 < mesh->getNbFaces() && curIndex - 1 >= 0)
        return &mesh->getTriangles(curIndex - 1);
    else
        return new TriangleTopo();
}

faceCirculator faceCirculator::operator++(int) {
    if (curTriangle->getNeighbor(curTriangle->getIndexInTriangle(startIndex) + 1) == 0){
        TriangleTopo *tmpTri = &mesh->getTriangles(mesh->getVertex(startIndex).getIdTriangle() - 1);
        curTriangle = tmpTri;
        while (tmpTri->getNeighbor(tmpTri->getIndexInTriangle(startIndex) - 1) != 0) {
            tmpTri = &mesh->getTriangles(tmpTri->getNeighbor(tmpTri->getIndexInTriangle(startIndex) - 1) - 1);
            curTriangle = tmpTri;
        }
        changeTriangle = false;
        curIndex = curTriangle->getId();
    } else {
        curTriangle = &mesh->getTriangles(curTriangle->getNeighbor(curTriangle->getIndexInTriangle(startIndex) + 1) - 1);
        curIndex = curTriangle->getId();
    }
}

bool faceCirculator::operator!=(const faceCirculator &fc) {
    return curIndex != fc.curIndex;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

faceExtIterator::faceExtIterator(std::vector<TriangleTopo> *tri, std::vector<int>* tab, int startIndex) {
    index = startIndex;
    tabIndex = tab;
    tabTriangle = tri;
}

void faceExtIterator::nextFace() {
    index++;
    if(index > tabIndex->size())
        index = static_cast<int>(tabIndex->size());
}

TriangleTopo *faceExtIterator::getFace() {
    if(index < tabIndex->size() && index >= 0)
        return &(*tabTriangle)[(*tabIndex)[index]-1];
    else
        return new TriangleTopo();
}

TriangleTopo *faceExtIterator::operator*() {
    if(index < tabIndex->size() && index >= 0)
        return &(*tabTriangle)[(*tabIndex)[index]-1];
    else
        return new TriangleTopo();
}

faceIterator faceExtIterator::operator++(int) {
    index++;
    if(index > tabIndex->size())
        index = static_cast<int>(tabIndex->size());
}

faceIterator faceExtIterator::operator--(int) {
    index--;
    if(index < 0)
        index = 0;
}

bool faceExtIterator::operator<(const faceExtIterator &fi) {
    return index < fi.index;
}
