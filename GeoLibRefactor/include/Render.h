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
    class Render {

      public:
        Render() = default;
        Render(std::vector<Maillage*> _mesh, std::vector<Color> _c);
        void draw(Orbiter cam, bool update, std::vector<bool> affiche);
        void destroy();

      protected:
        void initBuffer();
        void updateBuffer();

        std::vector<Color> c;
        std::vector<Maillage*> mesh;
        std::vector<GLuint> indexBuffer;
        std::vector<GLuint> vao;
        std::vector<GLuint> buffer;
        GLuint program;
    };

}

#endif //GEOALGO_RENDER_H
