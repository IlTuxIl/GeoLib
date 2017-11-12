
#include "gKit/app.h"
#include "gKit/vec.h"
#include "Render.h"

using namespace GeoLib;
class Framebuffer : public App {
  public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    Framebuffer() : App(1024, 640) {}

    int init() {
        tmp.loadPoint("test3.points", 3);
        tri = tmp.makeDelaunay();
        voronoMesh = tri.getVoronoiMesh();
        triangu = tri.getMaillage();
        m_camera.lookat(Point(-5, -5), Point(5, 5));

//        crustMesh = tri.crust();
        std::vector<Maillage*> mesh;
        std::vector<Color> c;

        mesh.push_back(&triangu);
        mesh.push_back(&voronoMesh);
        mesh.push_back(&crustMesh);
        c.push_back(Color(1,0,0));
        c.push_back(Color(0,0,1));
        c.push_back(Color(0,1,0));
        r = Render(mesh, c);

        glPointSize(20);
        glClearColor(0.2, 0.2, 0.2, 1.f);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CCW);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        return 0;   // ras, pas d'erreur
    }

    // destruction des objets de l'application
    int quit() {
        r.destroy();
        return 0;
    }

    int update(const float time, const float delta) {
        return 0;
    }

    // dessiner une nouvelle image
    int render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // deplace la camera
        int mx, my, x, y;
        double dx, dy;
        bool update = false;
        bool afficheVoronoi = false;
        isMaillage = false;
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
            haveToUpdateMaillage = true;
            voronoMesh = tri.getVoronoiMesh();
            triangu = tri.getMaillage();
        }
        if (mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            m_camera.move(mx);
        else if (mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());

        if (key_state(' ')) {
            can_add = true;
        }
        if (key_state('v'))
            afficheVoronoi = true;
        if (key_state('m')){
            if (haveToUpdateMaillage) {
                update = true;
                crustMesh = tri.crust();
                haveToUpdateMaillage = false;
            }
            isMaillage = true;
        }

        std::vector<bool> affiche;

        affiche.push_back(!isMaillage);
        affiche.push_back(afficheVoronoi && !isMaillage);
        affiche.push_back(isMaillage);
        r.draw(m_camera, update, affiche);

        return 1;
    }

  protected:
    Orbiter m_camera;
    Render r;
    Triangulation2D tmp;
    TriangulationDelaunay2D tri;
    bool can_add = true;
    bool haveToUpdateMaillage = true;
    bool isMaillage = false;
    Maillage voronoMesh;
    Maillage triangu;
    Maillage crustMesh;
};


int main(int argc, char **argv) {

        Framebuffer tp;
        tp.run();

        return 0;
    }

