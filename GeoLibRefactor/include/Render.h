//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_RENDER_H
#define GEOALGO_RENDER_H

#include <GL/glew.h>
#include "Triangulation2D.h"
#include "../../GeoLib/gKit/src/orbiter.h"

namespace GeoLib {

    class Render {
      public:
        Render() = default;
        Render(TriangulationDelaunay2D *_sp);
        void setProgram(GLuint p){program = p;};
        void draw(Orbiter cam, bool update, bool voronoi, bool maillage);
      protected:
        GLuint initBuffer(bool voronoi);
        GLuint updateBuffer(bool voronoi);

        TriangulationDelaunay2D* mesh;
        GLuint indexBuffer;
        GLuint program;
        GLuint vao[2];
        GLuint buffer[2];
        int cptVoronoi;
    };

}

#endif //GEOALGO_RENDER_H
