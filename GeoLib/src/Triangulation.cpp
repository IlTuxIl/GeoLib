//
// Created by julien on 08/09/17.
//

#include <iostream>
#include <map>
#include <queue>
#include "Triangulation.h"
#include "Iterateur.h"


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

//            if(triangles[estDans - 1].estExterieur() == -1){
//                for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
//                    if((*it) == estDans) {
//                        idExterieur.erase(it);
//                    }
//                }
//            }
//            checkExterieur(estDans);
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
//                                    if(triangles[idTri-1].estExterieur() == -1){
//                                        for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
//                                            if((*it) == idTri) {
//                                                idExterieur.erase(it);
//                                            }
//                                        }
//                                    }
//                                    checkExterieur(idTri);
                                }

                                if(map[{min2, max2}] == 0) {
                                    map[{min2, max2}] = static_cast<int>(triangles.size() + 1);
                                }
                                else{
                                    t.setNeighbor(map[{min2, max2}], 2);
                                    int idTri = map[{min2, max2}];
                                    linkTriangle(static_cast<int>(triangles.size() + 1), idTri, {min2, max2});
//                                    if(triangles[idTri-1].estExterieur() == -1){
//                                        for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
//                                            if((*it) == idTri) {
//                                                idExterieur.erase(it);
//                                            }
//                                        }
//                                    }
//                                    checkExterieur(idTri);
                                }
                                nbFaces++;
                                triangles.push_back(t);
                                idExterieur.push_back(t.getId());
//                                if((*tri)->estExterieur() == -1){
//                                    for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
//                                        if((*it) == (*tri)->getId()) {
//                                            idExterieur.erase(it);
//                                        }
//                                    }
//                                }
//                                checkExterieur((*tri)->getId());
                            }
                        }
                    }
                }
                else{
//                    for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
//                        if((*it) == (*tri)->getId()) {
//                            idExterieur.erase(it);
//                        }
//                    }
//                    checkExterieur((*tri)->getId());
                }
            }
            for(int i = 0; i < idExterieur.size(); i++)
                checkExterieur(idExterieur[i]);
        }
    }
}

bool Triangulation::loadPointsDelaunay(char *filename, int nbCoords) {
        std::ifstream ifs;
        ifs.open(filename);
        if(ifs.bad()){
            std::cout<<"Impossible d'ouvrir le fichier "<<filename<<" en lecture \n";
            return false;
        }
        ifs >> nbVertex;
        vertex.reserve(static_cast<unsigned long>(nbVertex));
        triangles.reserve(static_cast<unsigned long>(nbVertex));

        for(int i = 0; i < nbVertex; i ++) {
            Sommet s;
            if (nbCoords == 3){
                ifs >> s.x >> s.y >> s.z;
            }
            else if (nbCoords == 2){
                ifs >> s.x >> s.y;
                s.z = 0.0;
            }

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
        for(int i = 0; i < vertex.size(); i++)
            perturbe(i, epsilon);
        for(int i = 3; i < nbVertex; i++)
            addPointDelaunay(i);

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
    std::queue<int> file;
    int test = appartientMesh(idPoint);
    if(test != -1){
        splitTriangle(test, idPoint);

        checkExterieur(test);

        if(triangles[triangles.size()-1].estExterieur() != -1)
            idExterieur.push_back(triangles[triangles.size()-1].getId());

        if(triangles[triangles.size()-2].estExterieur() != -1)
            idExterieur.push_back(triangles[triangles.size() - 2].getId());
        vertex[idPoint].setIdTriangle(triangles.size() - 1);

        file.push(test);
        file.push(triangles.size() - 1);
        file.push(triangles.size());
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
                            }

                            if(map[{min2, max2}] == 0) {
                                map[{min2, max2}] = static_cast<int>(triangles.size() + 1);
                            }
                            else{
                                t.setNeighbor(map[{min2, max2}], 2);
                                int idTri = map[{min2, max2}];
                                linkTriangle(static_cast<int>(triangles.size() + 1), idTri, {min2, max2});
                            }
                            nbFaces++;
                            triangles.push_back(t);
                            file.push(t.getId());
                            idExterieur.push_back(t.getId());
                        }
                    }
                }
            }
        }

    }

    for(int i = 0; i < idExterieur.size(); i++)
        checkExterieur(idExterieur[i]);

    while(file.size() > 0){
        int top = file.front();
        for(int k = 0; k < 3; k++) {
            TriangleTopo triTopo = triangles[top - 1];
            for(int l = 0; l < 3; l++){
                if(triTopo.getNeighbor(k) != 0) {
                    int p = triangles[triTopo.getNeighbor(k) - 1].getIdSommet(l);
                    if (appartientCercle(top, p) < 0.0f) {
                        //int idTri = vertex[p].getIdTriangle();
                        faceCirculator fc = faceAround(p);
                        bool fin = false;
                        do {
                            //FAUX -> si plusieurs triangles adjacent
                            if (getArreteAdjacent(triangles[top - 1], *(*fc)).p1 != -1 &&
                                (*fc)->getIndexInTriangle(p) != -1) {
                                //                        ok = true;
                                fin = true;
                                flipTriangle(top, (*fc)->getId());
                                for (int voisin = 0; voisin < 3; voisin++) {
                                    if (triangles[top - 1].getNeighbor(voisin) >
                                        0/* && triangles[top - 1].getNeighbor(voisin) != (*fc)->getId()*/)
                                        file.push(triangles[top - 1].getNeighbor(voisin));

                                    if ((*fc)->getNeighbor(voisin) > 0/* && (*fc)->getNeighbor(voisin) != top*/)
                                        file.push((*fc)->getNeighbor(voisin));
                                }
                            }
                            if (fin)
                                break;
                            fc++;
                        } while (fc != faceAround(p) || fin);
                    }
                }
            }
        }
        file.pop();
    }
}

void Triangulation::addPoint(float x, float y){
    Sommet s = vector3(x,y,0.0);
    vertex.push_back(s);
    perturbe(vertex.size()-1, epsilon);
    nbVertex++;
    addPointDelaunay(vertex.size()-1);
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

    for(int i = 0; i < 3; i++){
        vertex[tri1.getIdSommet(i)].setIdTriangle(tri1.getId());
        vertex[tri2.getIdSommet(i)].setIdTriangle(tri2.getId());
    }

    if(!okt1 && t1Ext)
        idExterieur.push_back(t1Id);
    if(!okt2 && t2Ext)
        idExterieur.push_back(t2Id);
    return true;
}

std::vector<float> Triangulation::getVertex(){

    std::vector<float> ret;
    ret.reserve(nbVertex * 3);
    for(Sommet s : vertex){
        ret.push_back((float)s.x);
        ret.push_back((float)s.y);
        ret.push_back((float)s.z);
    }

    return ret;
}

std::vector<unsigned int> Triangulation::getIndex() {
    std::vector<unsigned int> ret;
    ret.reserve(nbFaces * 3);

    for(TriangleTopo t : triangles){
        ret.push_back((unsigned int) t.getIdSommet(1));
        ret.push_back((unsigned int) t.getIdSommet(0));
        ret.push_back((unsigned int) t.getIdSommet(2));
    }

    return ret;
}

std::vector<float> Triangulation::getVoronoi(){
    std::vector<float> ret;
    std::map<couple, int> map;
    ret.reserve(nbFaces * 6);

    for(int i = 0; i < vertex.size(); i++) {
        faceCirculator fc = faceAround(i);
        bool fin = false;

        while (!fin) {
            TriangleTopo t1 = *(*fc);
            fc++;
            TriangleTopo t2 = *(*fc);

            if (!(fc != faceAround(i)))
                fin = true;

            int p1 = std::min(t1.getId(), t2.getId());
            int p2 = std::max(t1.getId(), t2.getId());
            if (map[{p1, p2}] == 0) {
                map[{p1, p2}] = 1;
//                std::cout << p1 << " " << p2 << std::endl;
                vector3 v1 = vertex[t1.getIdSommet(0)];
                vector3 v2 = vertex[t1.getIdSommet(1)];
                vector3 v3 = vertex[t1.getIdSommet(2)];

                vector3 vor = t1.computeVoronoi(v1, v2, v3);
                ret.push_back((float) vor.x);
                ret.push_back((float) vor.y);
                ret.push_back((float) vor.z);

                v1 = vertex[t2.getIdSommet(0)];
                v2 = vertex[t2.getIdSommet(1)];
                v3 = vertex[t2.getIdSommet(2)];

                vor = t2.computeVoronoi(v1, v2, v3);
                ret.push_back((float) vor.x);
                ret.push_back((float) vor.y);
                ret.push_back((float) vor.z);

            }
        }
    }
    return ret;
}

void Triangulation::checkExterieur(int idTri) {
    if(triangles[idTri - 1].estExterieur() == -1){
        for(std::vector<int>::const_iterator it = idExterieur.begin(); it < idExterieur.end(); it++){
            if((*it) == idTri) {
                idExterieur.erase(it);
            }
        }
    }
}

Maillage2D Triangulation2D::crust() {
    std::vector<int> ret;
    std::vector<Sommet> points = vertex;
    int nbVertexOrig = nbVertex;
    std::vector<float> pVoronoi = getVoronoi();

    for(int i = 0; i < pVoronoi.size(); i++){
        addPoint(pVoronoi[i], pVoronoi[i+1]);
        i++;
        i++;
    }

    for(TriangleTopo t : triangles){
        int cpt = 0;
        for(int i = 0; i < 3; i++){
            if(t.getIdSommet(i) < nbVertexOrig)
                cpt++;
        }
        if(cpt == 2){
            for(int i = 0; i < 3; i++){
                if(t.getIdSommet(i) < nbVertexOrig){
                    ret.push_back(t.getIdSommet(i));
                }
            }
        }
    }

    return Maillage2D(ret, points);
}

void Triangulation::perturbe(int idPoint, double epsilon) {
    Sommet& s = vertex[idPoint];

    double p1 = (double) rand() / RAND_MAX;
    double p2 = (double) rand() / RAND_MAX;
    double p3 = (double) rand() / RAND_MAX;

    s.x += -(epsilon/2) + p1 * epsilon;
    s.y += -(epsilon/2) + p2 * epsilon;
    s.z += -(epsilon/2) + p3 * epsilon;
}

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