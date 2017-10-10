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
    Render(char* shaderName, Triangulation* m);
    void draw(Orbiter cam, bool voronoi, bool update);
private:
    GLuint updateBuffer(bool voronoi);
    GLuint initBuffer(bool isVoronoi);

    Triangulation* mesh;
    GLuint indexBuffer;
    GLuint program[2];
    GLuint vao[2];
    GLuint buffer[2];
};


#endif //GEOALGO_RENDER_H
