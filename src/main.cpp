
//! \file tuto_framebuffer.cpp utilisation de framebuffers, fbo.


#include <Render.h>
#include "../GeoLib/gKit/src/orbiter.h"
#include "../GeoLib/gKit/src/app.h"        // classe Application a deriver

class Framebuffer : public App
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    Framebuffer( ) : App(1024, 640) {}

    int init( )
    {
        tri.loadPointsDelaunay("test.points");
        r = Render("data/shader1.glsl", &tri);

        m_camera.lookat(Point(-5,-5), Point(5,5));

        glPointSize(20);
        glClearColor(0.2,0.2,0.2, 1.f);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CCW);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        return 0;   // ras, pas d'erreur
    }

    // destruction des objets de l'application
    int quit( )
    {

        return 0;
    }

    int update( const float time, const float delta )
    {
        return 0;
    }

    // dessiner une nouvelle image
    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // deplace la camera
        int mx, my, x, y;
        double dx, dy;
        bool update = false;
        bool voronoi = false;
        unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
        SDL_GetMouseState(&x, &y);

        dx = x;
        dy = y;

        if(mb && SDL_BUTTON(1) && can_add) {              // le bouton gauche est enfonce
            Transform MVP = m_camera.projection(1024, 640, 45);
            MVP = MVP.inverse();
            vec4 p((2*dx/1024)-1, 1-(2*dy/640), -1, 1);
            p = MVP(p);
            vec4 v(p.x*p.w, p.y*p.w, p.z*p.w, 0.0);
            v = m_camera.view().inverse()(v);
            Vector v2(v.x, v.y, v.z);
            v2 = normalize(v2);

            std::cout << -v2.x*(m_camera.position().z)/v2.z << ", " << -v2.y*(m_camera.position().z)/v2.z << std::endl;

            tri.addPoint(-v2.x*(m_camera.position().z)/v2.z, -v2.y*(m_camera.position().z)/v2.z);
            update = true;
            can_add = false;
        }
        if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            m_camera.move(mx);
        else if(mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());

        if(key_state(' '))
            can_add = true;
        if(key_state('v'))
            voronoi = true;

        r.draw(m_camera, voronoi, update);
        return 1;
    }

protected:
    Orbiter m_camera;
    Render r;
    Triangulation tri;
    bool can_add = true;
};


int main( int argc, char **argv )
{
    Framebuffer tp;
    tp.run();

    return 0;
}

