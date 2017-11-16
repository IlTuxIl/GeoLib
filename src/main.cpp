
#include "gKit/app.h"
#include "gKit/vec.h"
#include "Render.h"

using namespace GeoLib;
class Framebuffer : public App {
  public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    Framebuffer() : App(1024, 640) {}

    void moveCam(){
        if(key_state(SDLK_UP))         // le bouton du milieu est enfonce
            m_camera.translation((float) 0 / (float) window_width(), (float) 50 / (float) window_height());
        if(key_state(SDLK_DOWN))         // le bouton du milieu est enfonce
            m_camera.translation((float) 0 / (float) window_width(), (float) -50 / (float) window_height());
        if(key_state(SDLK_LEFT))         // le bouton du milieu est enfonce
            m_camera.translation((float) 50 / (float) window_width(), (float) 0 / (float) window_height());
        if(key_state(SDLK_RIGHT))         // le bouton du milieu est enfonce
            m_camera.translation((float) -50 / (float) window_width(), (float) 0 / (float) window_height());
    }

    int init() {
        ruppert2.setNbIndiceFace(2);
        tri.loadPoint("test.points", 3);
        //tri = tmp.makeDelaunay();
        voronoMesh = tri.getVoronoiMesh();
        triangu = tri.getMaillage();
        m_camera.lookat(Point(-5, -5), Point(5, 5));

        std::vector<Maillage *> mesh;
        std::vector<Color> c;
//        pointsRuppert.addPlot(vector3(-4.3934, 3.66117));
//        pointsRuppert.addPlot(vector3(-6.09584, -4.21468e-07));
//        pointsRuppert.addPlot(vector3(-2.65434, -1.35463));
//        pointsRuppert.addPlot(vector3(-2.17839, -3.62455));
//        pointsRuppert.addPlot(vector3(1.8672, -3.80761));
//
//        pointsRuppert.addPlot(vector3(3.11199, -1.28141));
//        pointsRuppert.addPlot(vector3(4.75952, 2.25162));
//        pointsRuppert.addPlot(vector3(-1.46447, 2.78249));
//        pointsRuppert.addPlot(vector3(-2.37976, -0.3112));
//        pointsRuppert.addPlot(vector3(1.33633, -1.53769));
//
//        pointsRuppert.addPlot(vector3(2.70926, 1.2631));
//        nbpointRuppert = 11;
//        idDebutForme = 11;
//
//        for (int i = 0; i < 6; i++) {
//            containtesRuppert.push_back((unsigned int) i);
//            containtesRuppert.push_back((unsigned int) i+1);
//        }
//        containtesRuppert.push_back(6);
//        containtesRuppert.push_back(0);
//        for (int i = 7; i < 10; i++) {
//            containtesRuppert.push_back((unsigned int) i);
//            containtesRuppert.push_back((unsigned int) i+1);
//        }
//        containtesRuppert.push_back(10);
//        containtesRuppert.push_back(7);

        mesh.push_back(&triangu);
        mesh.push_back(&voronoMesh);
        mesh.push_back(&crustMesh);
        mesh.push_back(&ruppert);
        mesh.push_back(&ruppert2);
        mesh.push_back(&ruppert3);
        c.push_back(Color(1,0,0));
        c.push_back(Color(0,0,1));
        c.push_back(Color(0,1,0));
        c.push_back(Color(1,1,0));
        c.push_back(Color(0,1,1));
        c.push_back(Color(1,0,0));
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

    bool fctRuppert(){
        int mx, my, x, y;
        double dx, dy;
        bool update = false;
        unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
        SDL_GetMouseState(&x, &y);
        isRuppert = false;
        dx = x;
        dy = y;
        if (mb && SDL_BUTTON(1) && can_add) {              // le bouton gauche est enfonce
            Transform PvpInv = (Viewport(window_width(), window_height()) *
                                m_camera.projection(window_width(), window_height(), 45)).inverse();
            Point res = PvpInv(Point(dx, dy));
            vector3 point(-res.x * (m_camera.position().z) / res.z, res.y * (m_camera.position().z) / res.z);
            std::cout << "pointsRuppert.addPlot(vector3(" << point.x() << ", " << point.y() << "));"<< std::endl;
            pointsRuppert.addPlot(point);
            if(nbpointRuppert > 1 && !changeForme) {
                containtesRuppert.push_back((unsigned int) nbpointRuppert-1);
                containtesRuppert.push_back((unsigned int) nbpointRuppert);
            }
            else if(nbpointRuppert == 1){
                containtesRuppert.push_back(0);
                containtesRuppert.push_back(1);
            }
            update = true;
            nbpointRuppert++;
            can_add = false;
            changeForme = false;
            haveToUpdateRuppert = true;
        }

        if (key_state(' '))
            can_add = true;
        if (key_state('p') && can_add) {
            ruppertMode = false;
            can_add = false;
        }
        if (key_state('r')) {
            if (haveToUpdateRuppert) {
                update = true;
                ruppert3 = tri.ruppert(pointsRuppert.getVector(), containtesRuppert, 25).getMaillage();
                haveToUpdateRuppert = false;
            }
            isRuppert = true;
        }
        if(key_state('n') && can_add){
            containtesRuppert.push_back((unsigned  int)nbpointRuppert-1);
            containtesRuppert.push_back((unsigned  int)idDebutForme);
            idDebutForme = nbpointRuppert;
            update = true;
            can_add = false;
            changeForme = true;
        }
        if (key_state('v'))
            update = true;
        if(update) {
            ruppert2.setVertexBuffer(pointsRuppert);
            ruppert2.setIndiceBuffer(containtesRuppert);
        }
        return update;
    }

    // dessiner une nouvelle image
    int render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // deplace la camera
        moveCam();
        int mx, my, x, y;
        double dx, dy;
        bool update = false;
        bool afficheVoronoi = false;
        isMaillage = false;
        isRuppert = false;
        unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
        SDL_GetMouseState(&x, &y);

        dx = x;
        dy = y;
        if (mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            m_camera.move(mx);
        else if (mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());

        if(!ruppertMode){
            if (mb && SDL_BUTTON(1) && can_add) {              // le bouton gauche est enfonce
                Transform PvpInv = (Viewport(window_width(), window_height()) *
                                    m_camera.projection(window_width(), window_height(), 45)).inverse();
                Point res = PvpInv(Point(dx, dy));

                if (tri.addPoint(-res.x * (m_camera.position().z) / res.z, res.y * (m_camera.position().z) / res.z)) {
                    std::cout << -res.x * (m_camera.position().z) / res.z << ", " << res.y * (m_camera.position().z) / res.z
                              << std::endl;
                    update = true;
                    can_add = false;
                    haveToUpdateMaillage = true;
                    haveToUpdateRuppert = true;
                    voronoMesh = tri.getVoronoiMesh();
                    triangu = tri.getMaillage();
                }
            }

            if (key_state(' '))
                can_add = true;
            if (key_state('v'))
                afficheVoronoi = true;
            if (key_state('p') && can_add) {
                ruppertMode = true;
                can_add = false;
            }
            if (key_state('m')) {
                if (haveToUpdateMaillage) {
                    update = true;
                    crustMesh = tri.crust();
                    haveToUpdateMaillage = false;
                }
                isMaillage = true;
            }
            if (key_state('r')) {
                if (haveToUpdateRuppert) {
                    update = true;
                    ruppert = tri.ruppert(20).getMaillage();
                    haveToUpdateRuppert = false;
                }
                isRuppert = true;
            }
        }
        else{
            update = fctRuppert();
        }
        std::vector<bool> affiche;

        affiche.push_back(!isMaillage && !isRuppert && !ruppertMode);
        affiche.push_back(afficheVoronoi && !isMaillage && !ruppertMode);
        affiche.push_back(isMaillage && !ruppertMode);
        affiche.push_back(isRuppert && !ruppertMode);
        affiche.push_back(ruppertMode && !isRuppert);
        affiche.push_back(ruppertMode && isRuppert);
        r.draw(m_camera, update, affiche);

        return 1;
    }

  protected:
    Orbiter m_camera;
    Render r;
    Triangulation2D tmp;
    TriangulationDelaunay2D tri;
    bool can_add = false;
    bool haveToUpdateMaillage = true;
    bool haveToUpdateRuppert = true;
    bool isMaillage = false;
    bool isRuppert = false;
    bool changeForme = false;
    bool afficheRuppert = false;
    Maillage voronoMesh;
    Maillage triangu;
    Maillage crustMesh;
    Maillage ruppert;
    Maillage ruppert2;
    Maillage ruppert3;
    ScatterPlot pointsRuppert;
    std::vector<unsigned int> containtesRuppert;
    int nbpointRuppert = 0;
    int idDebutForme = 0;
    bool ruppertMode = true;
};


int main(int argc, char **argv) {

    Framebuffer tp;
    tp.run();

    return 0;
}

