
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
        tri.loadPoints("test.points");
        r = Render("data/shader1.glsl", tri);
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
        // deplace la camera
        int mx, my;
        unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
        if(mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
            m_camera.rotation(mx, my);
        else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            m_camera.move(mx);
        else if(mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());

        r.draw(m_camera);

        return 1;
    }

protected:
    Orbiter m_camera;
    Render r;
    Triangulation tri;
};


int main( int argc, char **argv )
{
    Framebuffer tp;
    tp.run();

    return 0;
}
    