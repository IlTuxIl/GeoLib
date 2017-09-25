//
// Created by Julien on 04/09/2017.
//

#include "Mesh.h"
#include <iostream>
#include <GL/gl.h>
#include <map>

void Mesh::linkTriangle(int idNewTriangle, int idOldTriangle, const couple& c){
    //std::cout << "check " << idNewTriangle << " " << idOldTriangle << std::endl;
    TriangleTopo* t = &triangles[idOldTriangle-1];
    int s1,s2;
    s1 = faces[idOldTriangle-1][0];
    s2 = faces[idOldTriangle-1][1];

    if(s1 == c.p1 || s1 == c.p2){
        if(s2 == c.p1 || s2 == c.p2){
            t->setNeighbor(idNewTriangle, 2);
        }
        else{
            t->setNeighbor(idNewTriangle, 1);
        }
    }
    else{
        t->setNeighbor(idNewTriangle, 0);
    }

}

bool Mesh::loadOFF(char* filename) {
    std::ifstream ifs;
    ifs.open(filename);
    if(ifs.bad()){
        std::cout<<"Impossible d'ouvrir le fichier "<<filename<<" en lecture \n";
        return false;
    }

    std::map<couple,int> map;

    std::string str;
    ifs >> str;
    int trash;
    ifs >> nbVertex >> nbFaces >> trash;
////////////////////// INIT ///////////////////
    triangles.reserve((unsigned long) nbFaces);
    vertex.reserve((unsigned long) nbFaces);
    faces.reserve((unsigned long) nbFaces);
    infinite.setId(0);
////////////////////////////////////////////////////////
////////////////////// Lecture vertex et faces /////////

    for(int i = 0; i < nbVertex; i++){
        Sommet s;
        ifs >> s.x >> s.y >> s.z;
        vertex.push_back(s);
    }


    for(int i = 0; i < nbFaces; i++) {
        int id1, id2, id3;
        int min1, min2, min3;
        int max1, max2, max3;

        ifs >> trash >> id1 >> id2 >> id3;

        min1 = std::min(id1, id2);
        min2 = std::min(id2, id3);
        min3 = std::min(id1, id3);

        max1 = std::max(id1, id2);
        max2 = std::max(id2, id3);
        max3 = std::max(id1, id3);

        faces[i].x = id1;
        faces[i].y = id2;
        faces[i][2] = id3;

        vertex[id1].setIdTriangle(i+1);
        vertex[id2].setIdTriangle(i+1);
        vertex[id3].setIdTriangle(i+1);

        triangles.push_back(TriangleTopo(faces[i][0], faces[i][1], faces[i][2], i+1));

        if(map[{min1, max1}] == 0) {
            map[{min1, max1}] = i + 1;
        }
        else{
            triangles[i].setNeighbor(map[{min1, max1}], 2);
            linkTriangle(i + 1, map[{min1, max1}], {min1, max1});
        }

        if(map[{min2, max2}] == 0) {
            map[{min2, max2}] = i + 1;
        }
        else{
            triangles[i].setNeighbor(map[{min2, max2}], 0);
            linkTriangle(i + 1, map[{min2, max2}], {min2, max2});
        }

        if(map[{min3, max3}] == 0) {
            map[{min3, max3}] = i + 1;
        }
        else{
            triangles[i].setNeighbor(map[{min3, max3}], 1);
            linkTriangle(i + 1, map[{min3, max3}], {min3, max3});
        }
    }
/////////////////////////////////////////////////////////

    for(int i = 0; i < triangles.size(); i++){
        if(triangles[i].getNeighbor(0) == -1)
            triangles[i].setNeighbor(infinite.getId(), 0);
        if(triangles[i].getNeighbor(1) == -1)
            triangles[i].setNeighbor(infinite.getId(), 1);
        if(triangles[i].getNeighbor(2) == -1)
            triangles[i].setNeighbor(infinite.getId(), 2);
    }

    ifs.close();
    return true;
}

bool Mesh::saveOFF(char* filename) {

    std::ofstream ofs;
    ofs.open(filename);
    if(ofs.bad()){
        std::cout<<"Impossible d'ouvrir le fichier "<<filename<<" en ecriture \n";
        return false;
    }

    std::string str = "OFF";
    ofs << str << std::endl;

    ofs << nbVertex << ' ' << nbFaces << ' ' << 0 << std::endl;

    for(int i = 0; i < nbVertex; i++){
        ofs << vertex[i].x << ' ' << vertex[i].y << ' ' << vertex[i].z << std::endl;
    }

    for(int i = 0; i < nbFaces; i++){
        ofs << faces[i][0] << ' ' << faces[i][1] << ' ' << faces[i][2] << ' ' << faces[i][3] << std::endl;
    }

    ofs << '\0';
    ofs.close();
}

Sommet& Mesh::getVertex(int id) {
    if(id >= 0 && id < nbVertex)
        return vertex[id];
    return vertex[0];
}

vector3 Mesh::getFaces(int id) const {
    if(id >= 0 && id < nbFaces)
        return faces[id];
}

int Mesh::getNbVertex() const {
    return nbVertex;
}

int Mesh::getNbFaces() const {
    return nbFaces;
}

TriangleTopo& Mesh::getTriangles(int index) {
    if(index >= 0 && index < nbFaces)
        return triangles[index];
}

void Mesh::draw() {

    glBegin(GL_TRIANGLES);

    //std::cout << getNbFaces() << getNbVertex() << triangles.size() << std::endl;

    for (int i = 0; i < getNbFaces(); i++) {
        int i1, i2, i3;
        i1 = getFaces(i).x;
        i2 = getFaces(i).y;
        i3 = getFaces(i).z;

        vector3 v1 = getVertex(i1);
        vector3 v2 = getVertex(i2);
        vector3 v3 = getVertex(i3);

        glColor3f((float) (rand() * 100)/100, (float) (rand() * 100)/100, (float) (rand() * 100)/100);

        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);

    }

    glEnd();

}

void Mesh::drawAll() {

    glBegin(GL_TRIANGLES);

//    std::cout << getNbFaces() << std::endl;
    for (int i = 0; i < getNbFaces(); i++) {
        int tmp = getFaces(i).x;

        std::cout << tmp << std::endl;

        int *id = new int[tmp];
        vector3 *v = new vector3[tmp];

        for (int j = 0; j < tmp; j++){
            id[j] = getFaces(i)[j];
            for(int j = 0; j < tmp; j++)
                v[j] = getVertex(id[j]);
            for(int j = 0; j < tmp; j++)
                glVertex3f(v[j].x, v[j].y, v[j].z);
        }
    }

    glEnd();

}

Mesh::~Mesh() {
}

bool Mesh::appartient(int idTri, int idPoint) {
    TriangleTopo t = triangles[idTri-1];
    Sommet s1 = getVertex(t.getIdSommet(0));
    Sommet s2 = getVertex(t.getIdSommet(1));
    Sommet s3 = getVertex(t.getIdSommet(2));
    Sommet p = getVertex(idPoint);

    bool a = (vector3(s1 - s2).cross(vector3(s1 - p))).dot((vector3(s1 - p).cross(vector3(s1 - s3)))) >= 0;
    bool b = (vector3(s2 - s1).cross(vector3(s2 - p))).dot((vector3(s2 - p).cross(vector3(s2 - s3)))) >= 0;
    bool c = (vector3(s3 - s1).cross(vector3(s3 - p))).dot((vector3(s3 - p).cross(vector3(s3 - s2)))) >= 0;
    return a&&b&&c;
}

int Mesh::appartientMesh(Mesh &m, int idPoint) {
    for(int i = 1; i < nbFaces + 1; i++){
        if(appartient(i, idPoint))
            return i;
    }
    return -1;
}

//Uniquement si idSommet appartient à idTri
void Mesh::splitTriangle(int idTri, int idSommet) {
    TriangleTopo& triangleASplit = triangles[idTri-1];
    TriangleTopo triangleB = TriangleTopo(idSommet, triangleASplit.getIdSommet(1), triangleASplit.getIdSommet(2), triangles.size()+1);
    TriangleTopo triangleC = TriangleTopo(idSommet, triangleASplit.getIdSommet(2), triangleASplit.getIdSommet(0), triangles.size()+2);

    triangleB.setNeighbor(idTri, 2);
    triangleB.setNeighbor(triangles.size()+2, 1);
    triangleB.setNeighbor(triangleASplit.getNeighbor(0), 0);
    if(triangleASplit.getNeighbor(0) != 0) {
        TriangleTopo &voisinB = triangles[triangleASplit.getNeighbor(0) - 1];

        for (int i = 0; i < 3; i++) {
            if (voisinB.getNeighbor(i) == idTri) {
                voisinB.setNeighbor(triangles.size() + 1, i);
            }
        }
    }
    triangleC.setNeighbor(idTri, 1);
    triangleC.setNeighbor(triangles.size()+1, 2);
    triangleC.setNeighbor(triangleASplit.getNeighbor(1), 0);

    if(triangleASplit.getNeighbor(1) != 0) {
        TriangleTopo &voisinC = triangles[triangleASplit.getNeighbor(1) - 1];
        for (int i = 0; i < 3; i++) {
            if (voisinC.getNeighbor(i) == idTri) {
                voisinC.setNeighbor(triangles.size() + 2, i);
            }
        }
    }

    triangleASplit.setNeighbor(triangles.size()+1 ,0);
    triangleASplit.setNeighbor(triangles.size()+2 ,1);

    vector3 p1, p2;
    p1.x = idSommet;
    p1.y = triangleASplit.getIdSommet(1);
    p1.z = triangleASplit.getIdSommet(2);
    faces.push_back(p1);

    p2.x = idSommet;
    p2.y= triangleASplit.getIdSommet(2);
    p2.z = triangleASplit.getIdSommet(0);
    faces.push_back(p2);

    faces[idTri-1][2] = idSommet;

    triangles.push_back(triangleB);
    triangles.push_back(triangleC);
    triangleASplit.setIdSommet(idSommet, 2);
    nbFaces+=2;
}

couple getArreteAdjacent(const TriangleTopo& t1, const TriangleTopo& t2){
    int i;
    for(i = 0; i < 3; i++){
        if(t1.getNeighbor(i) == t2.getId())
            break;
    }
    switch (i){
        case 0:
            return couple{1, 2};
        case 1:
            return couple{2, 0};
        case 2:
            return couple{0, 1};
        default:
            return couple{-1,-1};
    }
}

couple getPointsAdjacent(const TriangleTopo& t1, const TriangleTopo& t2){
    int i;
    for(i = 0; i < 3; i++){
        if(t1.getNeighbor(i) == t2.getId())
            break;
    }
    switch (i){
        case 0:
            return couple{t1.getIdSommet(1), t1.getIdSommet(2)};
        case 1:
            return couple{t1.getIdSommet(2), t1.getIdSommet(0)};
        case 2:
            return couple{t1.getIdSommet(0), t1.getIdSommet(1)};
        default:
            return couple{-1,-1};
    }
}

//Tri1 et Tri2 adjacent
void Mesh::flipTriangle(int idTri1, int idTri2) {
    TriangleTopo& tri1 = triangles[idTri1-1];
    TriangleTopo& tri2 = triangles[idTri2-1];

    couple arrete1 = getArreteAdjacent(tri1, tri2);
    couple arrete2 = getArreteAdjacent(tri2, tri1);

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
}
