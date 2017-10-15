//
// Created by julien on 15/10/17.
//

#ifndef GEOALGO_RENDERMAILLAGE2D_H
#define GEOALGO_RENDERMAILLAGE2D_H

#include <GL/glew.h>
#include "Maillage2D.h"
#include "../gKit/src/orbiter.h"

class RenderMaillage2D{
public :
    RenderMaillage2D(){};
    RenderMaillage2D(char* filename, Maillage2D* maillage);
    void draw(Orbiter cam, bool update);
    void setMesh(Maillage2D* maillage){mesh = maillage;}


private:
    GLuint initBuffer();
    GLuint updateBuffer();
    int nbIndex;
    Maillage2D* mesh;
    GLuint program;
    GLuint vao;
    GLuint buffer;
};


#endif //GEOALGO_RENDERMAILLAGE2D_H
