//
// Created by julien on 08/09/17.
//

#include <iostream>
#include <map>
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

faceIterator::faceIterator(std::vector<TriangleTopo>* tab, int startIndex) {
    tabTri = tab;
    index = startIndex;
}

void faceIterator::nextFace() {
    index++;
    if(index > tabTri->size())
        index = tabTri->size();
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
        index = tabTri->size();
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
    return faceIterator(&triangles, 0);
}

faceIterator Triangulation::faceEnd() {
    return faceIterator(&triangles, getNbFaces());
}

faceExtIterator Triangulation::faceExtBegin() {
    return faceExtIterator(&triangles, &idExterieur, 0);
}

faceExtIterator Triangulation::faceExtEnd() {
    return faceExtIterator(&triangles, &idExterieur, idExterieur.size());
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

    for(int i = 0; i < nbVertex; i ++){
        Sommet s;
        ifs >> s.x >> s.y >> s.z;
        vertex.push_back(s);
    }

    ifs.close();

    //1er Triangle
    if(estTrigo((vertex[0] - vertex[1]), (vertex[0] - vertex[2]))){
        TriangleTopo t(0,1,2,1);
        t.setNeighbor(0, 0);
        t.setNeighbor(0, 1);
        t.setNeighbor(0, 2);
        triangles.push_back(t);
        idExterieur.push_back(t.getId());
    }
    else{
        TriangleTopo t(0,2,1,1);
        t.setNeighbor(0, 0);
        t.setNeighbor(0, 1);
        t.setNeighbor(0, 2);
        triangles.push_back(t);
        idExterieur.push_back(t.getId());
    }
    nbFaces = 1;
    //tous les autres points
    for(int i = 3; i < nbVertex; i++){

//        for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
//            std::cout << (*it) << std::endl;
//        }
//        std::cout << std::endl << std::endl;
        int estDans = appartientMesh(i);

        if(estDans != -1){
            splitTriangle(estDans, i);

            if(triangles[triangles.size()-1].estExterieur() != -1)
                idExterieur.push_back(triangles[triangles.size()-1].getId());

            if(triangles[triangles.size()-2].estExterieur() != -1) {
                idExterieur.push_back(triangles[triangles.size() - 2].getId());
            }
        }
        else{
            std::map<couple,int> map;
            for(faceExtIterator tri = faceExtBegin(); tri < faceExtEnd(); tri++){
//            for(int j = 0; j < idExterieur.size(); j++){
//                TriangleTopo& tri = triangles[idExterieur[j] - 1];
//                std::cout << (*tri)->getId() << std::endl;
                int test = (*tri)->estExterieur();
                if(test != -1){
                    for(int k = 0; k < 3; k++) {
                        if((*tri)->getNeighbor(k) == 0) {
                            int id1, id2;
                            switch (k) {
                                case 0:
                                    id1 = (*tri)->getIdSommet(1);
                                    id2 = (*tri)->getIdSommet(2);
                                    break;
                                case 1:
                                    id1 = (*tri)->getIdSommet(2);
                                    id2 = (*tri)->getIdSommet(0);
                                    break;
                                case 2:
                                    id1 = (*tri)->getIdSommet(0);
                                    id2 = (*tri)->getIdSommet(1);
                                    break;
                                default :
                                    break;
                            }
                            if (estTrigo((vertex[i] - vertex[id2]), (vertex[i] - vertex[id1]))) {
                                TriangleTopo t(i, id2, id1, triangles.size()+1);
                                int min1 = std::min(i, id1);
                                int max1 = std::max(i, id1);

                                int min2 = std::min(i, id2);
                                int max2 = std::max(i, id2);

                                t.setNeighbor(0, 1);
                                t.setNeighbor(0, 2);
                                t.setNeighbor((*tri)->getId(), 0);
                                triangles[(*tri)->getId() - 1].setNeighbor(triangles.size() + 1, k);

                                if(map[{min1, max1}] == 0) {
                                    map[{min1, max1}] = triangles.size() + 1;
                                }
                                else{
                                    t.setNeighbor(map[{min1, max1}], 1);
                                    int idTri = map[{min1, max1}];
                                    linkTriangle(triangles.size() + 1, idTri, {min1, max1});
                                    if(triangles[idTri].estExterieur() == -1){
                                        for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
                                            if((*it) == idTri) {
//                                                std::cout << "erase1" << (*it) << std::endl;
                                                idExterieur.erase(it);
                                            }
                                        }
                                    }
                                }

                                if(map[{min2, max2}] == 0) {
                                    map[{min2, max2}] = triangles.size() + 1;
                                }
                                else{
                                    t.setNeighbor(map[{min2, max2}], 2);
                                    int idTri = map[{min2, max2}];
                                    linkTriangle(triangles.size() + 1, idTri, {min2, max2});
                                    if(triangles[idTri].estExterieur() == -1){
                                        for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
                                            if((*it) == idTri) {
//                                                std::cout << "erase2" << (*it) << std::endl;
                                                idExterieur.erase(it);
                                            }
                                        }
                                    }
                                }
                                nbFaces++;
                                triangles.push_back(t);
                                idExterieur.push_back(t.getId());
                                if((*tri)->estExterieur() == -1){
                                    for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
                                        if((*it) == (*tri)->getId()) {
                                            idExterieur.erase(it);
//                                            std::cout << "erase3" << (*it) << std::endl;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else{
                    for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
                        if((*it) == (*tri)->getId()) {
                            idExterieur.erase(it);
//                            std::cout << "erase4" << (*it) << std::endl;
                        }
                    }
                }
            }
        }
    }

//    for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
//        std::cout << (*it) << std::endl;
//    }
}

int Triangulation::appartientMesh(int idPoint) {

    for(faceIterator it = faceBegin(); it < faceEnd(); it++){
        if(appartient((*it)->getId(), idPoint))
            return (*it)->getId();
    }
    return -1;
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
        index = tabIndex->size();
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
        index = tabIndex->size();
}

faceIterator faceExtIterator::operator--(int) {
    index--;
    if(index < 0)
        index = 0;
}

bool faceExtIterator::operator<(const faceExtIterator &fi) {
    return index < fi.index;
}
