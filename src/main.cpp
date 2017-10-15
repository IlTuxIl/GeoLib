
//! \file tuto_framebuffer.cpp utilisation de framebuffers, fbo.


#include <Render.h>
#include <RenderMaillage2D.h>
#include "../GeoLib/gKit/src/orbiter.h"
#include "../GeoLib/gKit/src/app.h"        // classe Application a deriver

class Framebuffer : public App{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    Framebuffer( ) : App(1024, 640){}

    int init( )
    {
        tri.loadPointsDelaunay("test.points");

        m_camera.lookat(Point(-5,-5), Point(5,5));

//        tri.addPoint(-0.951903, 1.15327);
//        tri.addPoint(0.0915303, 1.50108);
//        tri.addPoint(0.860375, 1.46447);
//        tri.addPoint(1.42786, 1.02513);
//        tri.addPoint(1.62922, 0.256281);
//        tri.addPoint(1.46447, -0.695622);
//        tri.addPoint(0.659011, -1.18988);
//        tri.addPoint(-0.585786, -1.2448);
//        tri.addPoint(-1.29971, -0.732234);
        r = Render("data/shader1.glsl", &tri);
        m = RenderMaillage2D("data/shader1.glsl", &m2d);
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
    int render( ) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // deplace la camera
        int mx, my, x, y;
        double dx, dy;
        bool update = false;
        bool voronoi = false;
        unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
        SDL_GetMouseState(&x, &y);

        dx = x;
        dy = y;

        if (mb && SDL_BUTTON(1) && can_add) {              // le bouton gauche est enfonce
            Transform PvpInv = (Viewport(window_width(), window_height()) *
                                m_camera.projection(window_width(), window_height(), 45)).inverse();
            Point res = PvpInv(Point(dx, dy));

            std::cout << -res.x * (m_camera.position().z) / res.z << ", " << res.y * (m_camera.position().z) / res.z
                      << std::endl;
            tri.addPoint(-res.x * (m_camera.position().z) / res.z, res.y * (m_camera.position().z) / res.z);
            update = true;
            can_add = false;
        }
        if (mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            m_camera.move(mx);
        else if (mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());

        if (key_state(' ')){
            can_add = true;
            can_crust = true;
        }
        if(key_state('v'))
            voronoi = true;
        if(key_state('m') && can_crust){
            update = true;
            can_crust = false;
            m2d = tri.crust();
            m.setMesh(&m2d);
            isMaillage = true;
        }

        if(!isMaillage)
            r.draw(m_camera, voronoi, update);
        else
            m.draw(m_camera, update);
        return 1;
    }

protected:
    Orbiter m_camera;
    Render r;
    RenderMaillage2D m;
    Triangulation tri;
    Maillage2D m2d;
    bool can_add = true;
    bool can_crust = true;
    bool isMaillage = false;
};


int main( int argc, char **argv ){
    Framebuffer tp;
    tp.run();

//    Triangulation tri;
//    tri.loadPointsDelaunay("test.points");
//    tri.addPoint(-0.951903, 1.15327);
//    tri.addPoint(0.0915303, 1.50108);
//    tri.addPoint(0.860375, 1.46447);
//    tri.addPoint(1.42786, 1.02513);
//    tri.addPoint(1.62922, 0.256281);
//    tri.addPoint(1.46447, -0.695622);
//    tri.addPoint(0.659011, -1.18988);
//    tri.addPoint(-0.585786, -1.2448);
//    tri.addPoint(-1.29971, -0.732234);

//    tri.getMaillage();

    return 0;
}

