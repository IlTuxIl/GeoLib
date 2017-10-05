//
// Created by Julien on 05/10/2017.
//

#ifndef GEOALGO_RENDER_H
#define GEOALGO_RENDER_H

#include <GL/glew.h>
#include "Triangulation.h"
#include "../gKit/src/orbiter.h"

class Render {
public:
    Render() = default;
    Render(char* shaderName, Triangulation m);
    void draw(Orbiter cam);
private:
    GLuint initBuffer();
    Triangulation mesh;

    GLuint program;
    GLuint vao;
};


#endif //GEOALGO_RENDER_H
