//
////! \file tuto_framebuffer.cpp utilisation de framebuffers, fbo.
//
//
//
//#include "../../GeoLib/gKit/src/app.h"
//#include "../../GeoLib/gKit/src/vec.h"
//#include "../../GeoLibRefactor/include/Render.h"
//
//namespace GeoLib {
//    class Framebuffer : public App {
//    public:
//        // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
//        Framebuffer() : App(1024, 640) {}
//
//        int init() {
//            tri.loadPoint("test.points", 3);
//
//            m_camera.lookat(Point(-5, -5), Point(5, 5));
//
//            r = Render(&tri);
//            glPointSize(20);
//            glClearColor(0.2, 0.2, 0.2, 1.f);
//            glDepthFunc(GL_LESS);
//            glEnable(GL_DEPTH_TEST);
//            glFrontFace(GL_CCW);
//
//            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//            return 0;   // ras, pas d'erreur
//        }
//
//        // destruction des objets de l'application
//        int quit() {
//
//            return 0;
//        }
//
//        int update(const float time, const float delta) {
//            return 0;
//        }
//
//        // dessiner une nouvelle image
//        int render() {
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            // deplace la camera
//            int mx, my, x, y;
//            double dx, dy;
//            bool update = false;
//            bool voronoi = false;
//            unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
//            SDL_GetMouseState(&x, &y);
//
//            dx = x;
//            dy = y;
//
//            if (mb && SDL_BUTTON(1) && can_add) {              // le bouton gauche est enfonce
//                Transform PvpInv = (Viewport(window_width(), window_height()) *
//                                    m_camera.projection(window_width(), window_height(), 45)).inverse();
//                Point res = PvpInv(Point(dx, dy));
//
//                std::cout << -res.x * (m_camera.position().z) / res.z << ", " << res.y * (m_camera.position().z) / res.z
//                          << std::endl;
//                tri.addPoint(-res.x * (m_camera.position().z) / res.z, res.y * (m_camera.position().z) / res.z);
//                update = true;
//                can_add = false;
//            }
//            if (mb & SDL_BUTTON(3))         // le bouton droit est enfonce
//                m_camera.move(mx);
//            else if (mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
//                m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());
//
//            if (key_state(' ')) {
//                can_add = true;
//                can_crust = true;
//            }
//            if (key_state('v'))
//                voronoi = true;
//
//            r.draw(m_camera, voronoi, update, false);
//            return 1;
//        }
//
//    protected:
//        Orbiter m_camera;
//        Render r;
//        TriangulationDelaunay2D tri;
//        bool can_add = true;
//        bool can_crust = true;
//        bool isMaillage = false;
//    };
//
//
//    int main(int argc, char **argv) {
//        Framebuffer tp;
//        tp.run();
//
//        return 0;
//    }
//
//}