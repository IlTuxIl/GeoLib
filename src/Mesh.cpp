//
// Created by Julien on 04/09/2017.
//

#include "Mesh.h"
#include <iostream>

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

vector3 Mesh::getVertex(int id) const {
    if(id >= 0 && id < nbVertex)
        return vertex[id];
    return vector3();
}

int* Mesh::getFaces(int id) const {
    if(id >= 0 && id < nbFaces)
        return faces[id];
    return nullptr;
}

int Mesh::getNbVertex() const {
    return nbVertex;
}

int Mesh::getNbFaces() const {
    return nbFaces;
}
