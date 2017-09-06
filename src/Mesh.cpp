//
// Created by Julien on 04/09/2017.
//

#include "Mesh.h"
#include <iostream>
#include <GL/gl.h>

bool Mesh::loadOFF(char* filename) {
    std::ifstream ifs;
    ifs.open(filename);

    if(ifs.bad()){
        std::cout<<"Impossible d'ouvrir le fichier "<<filename<<" en lecture \n";
        return false;
    }

    std::string str;
    ifs >> str;
    float temp;
    ifs >> temp;
    nbVertex = (int) temp;
    ifs >> temp;
    nbFaces = (int) temp;
    ifs >> temp;

    vertex = new vector3[nbVertex];
    faces = new int*[nbFaces];
    for(int i = 0; i < nbVertex; i++){
        ifs >> temp;
        vertex[i].x = temp;
        ifs >> temp;
        vertex[i].y = temp;
        ifs >> temp;
        vertex[i].z = temp;
    }

    for(int i = 0; i < nbFaces; i++){
        int tmp;
        ifs >> temp;
        tmp = (int) temp;
        faces[i] = new int [tmp + 1];
        faces[i][0] = tmp;
        for(int j = 1; j < tmp + 1; j++){
            ifs >> temp;
            faces[i][j] = (int) temp;
        }
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

vector3 Mesh::getVertex(int id) const {
    if(id >= 0 && id < nbVertex)
        return vertex[id];
    return vertex[0];
}

vector3* Mesh::getVertex() {
    return vertex;
}

Triangle* Mesh::getTriangle(int id) {
    Triangle* ret = new Triangle();

    ret->v1 = getVertex(getFaces(id)[1]);
    ret->v2 = getVertex(getFaces(id)[2]);
    ret->v3 = getVertex(getFaces(id)[3]);

    return ret;
}

int* Mesh::getFaces(int id) const {
    if(id >= 0 && id < nbFaces)
        return faces[id];
    return 0;
}

int Mesh::getNbVertex() const {
    return nbVertex;
}

int Mesh::getNbFaces() const {
    return nbFaces;
}

void Mesh::draw() {

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < getNbFaces(); i++) {
        //glVertex3f(points[i].x, points[i].y, points[i].z);
        int i1, i2, i3;
        i1 = getFaces(i)[1];
        i2 = getFaces(i)[2];
        i3 = getFaces(i)[3];

        vector3 v1 = getVertex(i1);
        vector3 v2 = getVertex(i2);
        vector3 v3 = getVertex(i3);

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
        int tmp = getFaces(i)[0];

        std::cout << tmp << std::endl;

        int *id = new int[tmp];
        vector3 *v = new vector3[tmp];

        for (int j = 0; j < tmp; j++){
            id[j] = getFaces(i)[j];
        //for(int j = 0; j < tmp; j++)
            v[j] = getVertex(id[j]);
        //for(int j = 0; j < tmp; j++)
            glVertex3f(v[j].x, v[j].y, v[j].z);
        }
    }

    glEnd();

}