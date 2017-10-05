//
// Created by julien on 08/09/17.
//

#include <iostream>
#include <map>
#include <queue>
#include "../include/Triangulation.h"

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
    return faceExtIterator(&triangles, &idExterieur, static_cast<int>(idExterieur.size()));
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
    vertex.reserve(static_cast<unsigned long>(nbVertex));
    triangles.reserve(static_cast<unsigned long>(nbVertex));

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
        vertex[0].setIdTriangle(1);
        vertex[1].setIdTriangle(1);
        vertex[2].setIdTriangle(1);
        triangles.push_back(t);
        idExterieur.push_back(t.getId());
    }
    else{
        TriangleTopo t(0,2,1,1);
        t.setNeighbor(0, 0);
        t.setNeighbor(0, 1);
        t.setNeighbor(0, 2);
        vertex[0].setIdTriangle(1);
        vertex[1].setIdTriangle(1);
        vertex[2].setIdTriangle(1);
        triangles.push_back(t);
        idExterieur.push_back(t.getId());
    }
    nbFaces = 1;
    //tous les autres points
    for(int i = 3; i < nbVertex; i++){
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
                                    id1 = -1;
                                    id2 = -1;
                                    break;
                            }
                            if (estTrigo((vertex[i] - vertex[id2]), (vertex[i] - vertex[id1]))) {
                                TriangleTopo t(i, id2, id1, static_cast<int>(triangles.size() + 1));
                                int min1 = std::min(i, id1);
                                int max1 = std::max(i, id1);

                                int min2 = std::min(i, id2);
                                int max2 = std::max(i, id2);

                                vertex[id2].setIdTriangle(triangles.size() + 1);
                                vertex[id1].setIdTriangle(triangles.size() + 1);
                                vertex[i].setIdTriangle(triangles.size() + 1);

                                t.setNeighbor(0, 1);
                                t.setNeighbor(0, 2);
                                t.setNeighbor((*tri)->getId(), 0);
                                triangles[(*tri)->getId() - 1].setNeighbor(static_cast<int>(triangles.size() + 1), k);

                                if(map[{min1, max1}] == 0) {
                                    map[{min1, max1}] = static_cast<int>(triangles.size() + 1);
                                }
                                else{
                                    t.setNeighbor(map[{min1, max1}], 1);
                                    int idTri = map[{min1, max1}];
                                    linkTriangle(static_cast<int>(triangles.size() + 1), idTri, {min1, max1});
                                    if(triangles[idTri-1].estExterieur() == -1){
                                        for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
                                            if((*it) == idTri) {
                                                idExterieur.erase(it);
                                            }
                                        }
                                    }
                                }

                                if(map[{min2, max2}] == 0) {
                                    map[{min2, max2}] = static_cast<int>(triangles.size() + 1);
                                }
                                else{
                                    t.setNeighbor(map[{min2, max2}], 2);
                                    int idTri = map[{min2, max2}];
                                    linkTriangle(static_cast<int>(triangles.size() + 1), idTri, {min2, max2});
                                    if(triangles[idTri-1].estExterieur() == -1){
                                        for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
                                            if((*it) == idTri) {
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
                        }
                    }
                }
            }
        }
    }
}

int Triangulation::appartientMesh(int idPoint) {

    Sommet but = vertex[idPoint];
    TriangleTopo cur = triangles[rand() % triangles.size()];
    int cpt = 0;
    while(!appartient(cur.getId(), idPoint)) {
        cpt++;
        for (int i = 0; i < 3; i++) {
            Sommet s1 = vertex[cur.getIdSommet(i + 1)];
            Sommet s2 = vertex[cur.getIdSommet(i + 2)];

            if (estTrigo((but - s2), (but - s1))) {
                if (cur.getNeighbor(i) != 0){
                    cur = triangles[cur.getNeighbor(i) - 1];
                    break;
                }
            }
            if(i == 2) {
                //std::cout << cpt << " " << triangles.size() << std::endl;
                return -1;
            }
        }
    }
    //std::cout << cpt << " " << triangles.size() << std::endl;
    return cur.getId();
//    for(faceIterator it = faceBegin(); it < faceEnd(); it++){
//        if(appartient((*it)->getId(), idPoint))
//            return (*it)->getId();
//    }
}

void Triangulation::makeDelaunay() {
    std::queue<int> file;
    //Initialisation de la pile
    for(faceIterator fi = faceBegin(); fi < faceEnd(); fi++) {
        file.push((*fi)->getId());
    }

    //algo
    while(file.size() > 0){
        int top = file.front();
        for(int p = 0; p < vertex.size(); p++){
            bool ok = false;
            if(appartientCercle(top, p) < 0.0f){
//                int idTri = vertex[p].getIdTriangle();
                faceCirculator fc = faceAround(p);
                bool fin = false;
                do {
                    if (getArreteAdjacent(triangles[top - 1], *(*fc)).p1 != -1) {
                        ok = true;
                        fin = true;
                        flipTriangle(top, (*fc)->getId());
                        //PAS BON
                        for(int voisin = 0; voisin < 3; voisin++){
                            if(triangles[top - 1].getNeighbor(voisin) > 0 && triangles[top - 1].getNeighbor(voisin) != (*fc)->getId())
                                file.push(triangles[top - 1].getNeighbor(voisin));
                            if((*fc)->getNeighbor(voisin) > 0 && (*fc)->getNeighbor(voisin) != top)
                                file.push((*fc)->getNeighbor(voisin));
                        }
                    }
                    if(fin)
                        break;
                    fc++;
                }while(fc != faceAround(p) || fin);
            }
            if(ok)
                break;
        }
        file.pop();
    }
}

void Triangulation::addPointDelaunay(int idPoint) {
    Sommet s = vertex[idPoint];
    std::queue<int> file;
    int test = appartientMesh(idPoint);
    if(test != 1){
        splitTriangle(test, idPoint);
        if(triangles[triangles.size()-1].estExterieur() != -1)
            idExterieur.push_back(triangles[triangles.size()-1].getId());

        if(triangles[triangles.size()-2].estExterieur() != -1)
            idExterieur.push_back(triangles[triangles.size() - 2].getId());

        file.push(test);
        file.push(triangles.size() - 2);
        file.push(triangles.size() - 1);
    }
    else{
        //cas exterieur
        std::map<couple,int> map;
        for(faceExtIterator tri = faceExtBegin(); tri < faceExtEnd(); tri++){
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
                                id1 = -1;
                                id2 = -1;
                                break;
                        }
                        if (estTrigo((vertex[idPoint] - vertex[id2]), (vertex[idPoint] - vertex[id1]))) {
                            TriangleTopo t(idPoint, id2, id1, static_cast<int>(triangles.size() + 1));
                            int min1 = std::min(idPoint, id1);
                            int max1 = std::max(idPoint, id1);

                            int min2 = std::min(idPoint, id2);
                            int max2 = std::max(idPoint, id2);

                            vertex[id2].setIdTriangle(triangles.size() + 1);
                            vertex[id1].setIdTriangle(triangles.size() + 1);
                            vertex[idPoint].setIdTriangle(triangles.size() + 1);

                            t.setNeighbor(0, 1);
                            t.setNeighbor(0, 2);
                            t.setNeighbor((*tri)->getId(), 0);
                            triangles[(*tri)->getId() - 1].setNeighbor(static_cast<int>(triangles.size() + 1), k);

                            if(map[{min1, max1}] == 0) {
                                map[{min1, max1}] = static_cast<int>(triangles.size() + 1);
                            }
                            else{
                                t.setNeighbor(map[{min1, max1}], 1);
                                int idTri = map[{min1, max1}];
                                linkTriangle(static_cast<int>(triangles.size() + 1), idTri, {min1, max1});
                                if(triangles[idTri-1].estExterieur() == -1){
                                    for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
                                        if((*it) == idTri) {
                                            idExterieur.erase(it);
                                        }
                                    }
                                }
                            }

                            if(map[{min2, max2}] == 0) {
                                map[{min2, max2}] = static_cast<int>(triangles.size() + 1);
                            }
                            else{
                                t.setNeighbor(map[{min2, max2}], 2);
                                int idTri = map[{min2, max2}];
                                linkTriangle(static_cast<int>(triangles.size() + 1), idTri, {min2, max2});
                                if(triangles[idTri-1].estExterieur() == -1){
                                    for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
                                        if((*it) == idTri) {
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
                    }
                }
            }
        }

    }

    while(file.size() > 0){
        int top = file.front();
        for(int p = 0; p < vertex.size(); p++){
            bool ok = false;
            if(appartientCercle(top, p) < 0.0f){
//                int idTri = vertex[p].getIdTriangle();
                faceCirculator fc = faceAround(p);
                bool fin = false;
                do {
                    if (getArreteAdjacent(triangles[top - 1], *(*fc)).p1 != -1) {
                        ok = true;
                        fin = true;
                        flipTriangle(top, (*fc)->getId());
                        //PAS BON
                        for(int voisin = 0; voisin < 3; voisin++){
                            if(triangles[top - 1].getNeighbor(voisin) > 0 && triangles[top - 1].getNeighbor(voisin) != (*fc)->getId())
                                file.push(triangles[top - 1].getNeighbor(voisin));
                            if((*fc)->getNeighbor(voisin) > 0 && (*fc)->getNeighbor(voisin) != top)
                                file.push((*fc)->getNeighbor(voisin));
                        }
                    }
                    if(fin)
                        break;
                    fc++;
                }while(fc != faceAround(p) || fin);
            }
            if(ok)
                break;
        }
        file.pop();
    }
}

//Tri1 et Tri2 adjacent
bool Triangulation::flipTriangle(int idTri1, int idTri2) {
    TriangleTopo& tri1 = triangles[idTri1-1];
    TriangleTopo& tri2 = triangles[idTri2-1];

    couple arrete1 = getArreteAdjacent(tri1, tri2);
    couple arrete2 = getArreteAdjacent(tri2, tri1);

    if(arrete1.p1 == -1 || arrete2.p1 == -1)
        return false;

    int indexT1 = 3 - arrete1.p1 - arrete1.p2;
    int indexT2 = 3 - arrete2.p1 - arrete2.p2;

    int idVoisin1 = tri1.getNeighbor(arrete1.p2);
    int idVoisin2 = tri2.getNeighbor(arrete2.p2);

    tri1.setIdSommet(tri2.getIdSommet(indexT2), arrete1.p1);
    tri2.setIdSommet(tri1.getIdSommet(indexT1), arrete2.p1);

    tri1.setNeighbor(tri2.getId(), arrete1.p2);
    tri1.setNeighbor(idVoisin2, indexT1);

    tri2.setNeighbor(tri1.getId(), arrete2.p2);
    tri2.setNeighbor(idVoisin1, indexT2);

    if(idVoisin1 > 0){
        TriangleTopo& voisin = triangles[idVoisin1 - 1];
        for(int i = 0; i < 3; i++) {
            if (voisin.getNeighbor(i) == tri1.getId()){
                voisin.setNeighbor(tri2.getId(), i);
                break;
            }
        }
    }
    if(idVoisin2 > 0){
        TriangleTopo& voisin = triangles[idVoisin2 - 1];
        for(int i = 0; i < 3; i++) {
            if (voisin.getNeighbor(i) == tri2.getId()){
                voisin.setNeighbor(tri1.getId(), i);
                break;
            }
        }
    }


    bool t1Ext = tri1.estExterieur() != -1;
    bool t2Ext = tri2.estExterieur() != -1;

    bool okt1 = false;
    bool okt2 = false;

    int t1Id = tri1.getId();
    int t2Id = tri2.getId();

    for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
        if((*it) == t1Id){
            okt1 = true;
            if(!t1Ext)
                idExterieur.erase(it);
        }

        if((*it) == t2Id){
            okt2 = true;
            if(!t2Ext)
                idExterieur.erase(it);
        }
        if(okt1 && okt2)
            break;
    }

    if(!okt1 && t1Ext)
        idExterieur.push_back(t1Id);
    if(!okt2 && t2Ext)
        idExterieur.push_back(t2Id);
    return true;
}

Sommet* Triangulation::getVertex(){
    return vertex.data();
}

int* Triangulation::getIndex() {
    int* ret = new int(3 * sizeof(int) * nbFaces);
    int i = 0;
    for(TriangleTopo t : triangles){
        ret[i] = t.getIdSommet(0);
        i++;
        ret[i] = t.getIdSommet(1);
        i++;
        ret[i] = t.getIdSommet(2);
        i++;
    }
    return ret;
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
    //BUG SI MODIFICATION DU CUR TRIANGLE
//    if (changeTriangle) {

        //std::cout << curTriangle->getIndexInTriangle(startIndex) + 1 << std::endl;

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
//    }
//    else{
//        changeTriangle = true;
//        curIndex = curTriangle->getId();
//    }
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
