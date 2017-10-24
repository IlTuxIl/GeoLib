//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_RENDER_H
#define GEOALGO_RENDER_H

#include <GL/glew.h>
#include "Triangulation2D.h"
#include "../../GeoLib/gKit/src/orbiter.h"
#include "../gKit/src/color.h"

namespace GeoLib {
    template <class T>
    class Render {
      public:
        Render() = default;

      protected:
        void initBuffer();
        void updateBuffer();

        Color c;
        T* mesh;
        GLuint indexBuffer;
        GLuint program;
        GLuint vao;
        GLuint buffer;
    };

    class Render1D : Render<Maillage1D>{
      public:
        Render1D() = default;
        Render1D(Maillage1D* _mesh);
        void draw(Orbiter cam, bool update);

        void setColor(Color col){c = col;}
        void setProgram(GLuint p){program = p;};

    };

    class Render2D : Render<Maillage2D>{
    public:
        Render2D() = default;
        Render2D(Maillage2D* _mesh);
        void draw(Orbiter cam, bool update);

        void setColor(Color col){c = col;}
        void setProgram(GLuint p){program = p;};

    };

}

#endif //GEOALGO_RENDER_H
