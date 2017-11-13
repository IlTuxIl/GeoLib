//
// Created by julien on 17/10/17.
//

#include <cmath>
#include <map>
#include <fstream>
#include <stack>
#include "Triangulation2D.h"

namespace GeoLib{

    bool Triangulation2D::loadPoint(std::string filename, int nbDim) {
        int nbVertex;

        std::ifstream ifs;
        ifs.open(filename);
        if(ifs.bad()){
            std::cout<<"Impossible d'ouvrir le fichier "<<filename<<" en lecture \n";
        }
        ifs >> nbVertex;
        vertex.reserve(nbVertex);
        triangles.reserve(nbVertex);

        for(int i = 0; i < nbVertex; i ++){
            double a,b;
            double c = 0.0;
            if(nbDim == 2)
                ifs >> a >> b;
            else if(nbDim == 3)
                ifs >> a >> b >> c;
            Sommet s(a,b,c);
            vertex.addPlot(s);
        }

        ifs.close();

        //1er Triangle
        if(estTrigo((vertex[0] - vertex[1]), (vertex[0] - vertex[2]))){
            TriangleTopo t(0,1,2,1);
            t.setNeighbor(0, 0);
            t.setNeighbor(0, 1);
            t.setNeighbor(0, 2);
            vertex[0].setIdTriangle(1);
            vertex[1].setIdTriangle(1);
            vertex[2].setIdTriangle(1);
            triangles.push_back(t);
            idTriExtern.push_back(t.getId());
        }
        else{
            TriangleTopo t(0,2,1,1);
            t.setNeighbor(0, 0);
            t.setNeighbor(0, 1);
            t.setNeighbor(0, 2);
            vertex[0].setIdTriangle(1);
            vertex[1].setIdTriangle(1);
            vertex[2].setIdTriangle(1);
            triangles.push_back(t);
            idTriExtern.push_back(t.getId());
        }

        for(int i = 3; i < nbVertex; i++)
            addPoint(i);
    }

    bool Triangulation2D::addPoint(double x, double y, double _epsilon) {
        epsilon = _epsilon;
        if(checkRange(x, y, 0.05)) {
            Sommet s = vector3(x, y, 0.0);
            vertex.addPlot(s);
            addPoint(vertex.getSize() - 1);
            return true;
        }
        return false;
    }

    void Triangulation2D::addPoint(int idPoint) {
        perturbe(idPoint);
        int estDans = appartientMesh(idPoint);
        if (estDans != -1) {
            addTriangleIntern(estDans, idPoint);
        }
        else{
            addTriangleExtern(idPoint);
        }

        for(int i = 0; i < idTriExtern.size(); i++)
            checkExterieur(idTriExtern[i]);
    }

    bool Triangulation2D::appartientTriangle(int idTri, int idPoint) const {

        TriangleTopo t = triangles[idTri-1];

        vector3 s1 = vertex[t.getIdSommet(0)];
        vector3 s2 = vertex[t.getIdSommet(1)];
        vector3 s3 = vertex[t.getIdSommet(2)];
        vector3 p = vertex[idPoint];

        bool a = ((s1 - s2).cross(s1 - p)).dot(((s1 - p).cross(s1 - s3))) >= 0;
        bool b = ((s2 - s1).cross(s2 - p)).dot(((s2 - p).cross(s2 - s3))) >= 0;
        bool c = ((s3 - s1).cross(s3 - p)).dot(((s3 - p).cross(s3 - s2))) >= 0;

        return a&&b&&c;
    }

    double Triangulation2D::appartientCercle(int idTri, int idPoint) const {
        TriangleTopo tri = triangles[idTri-1];

        for(int i = 0; i < 3; i++){
            if(tri.getIdSommet(i) == idPoint)
                return 0;
        }

        Sommet s = vertex[idPoint];
        Sommet p1 = vertex[tri.getIdSommet(0)];
        Sommet p2 = vertex[tri.getIdSommet(1)];
        Sommet p3 = vertex[tri.getIdSommet(2)];

        p1.setZ(pow(p1.x() ,2) + pow(p1.y(), 2));
        p2.setZ(pow(p2.x() ,2) + pow(p2.y(), 2));
        p3.setZ(pow(p3.x() ,2) + pow(p3.y(), 2));
        s.setZ(pow(s.x() ,2) + pow(s.y(), 2));

        return vector3(vector3(p2-p1).cross(p3 - p1)).dot(s-p1);
    }

    int Triangulation2D::appartientMesh(int idPoint) {

        Sommet but = vertex[idPoint];
        TriangleTopo cur = triangles[rand() % triangles.size()];

        while(appartientTriangle(cur.getId(), idPoint) == 0) {
            for (int i = 0; i < 3; i++) {
                Sommet s1 = vertex[cur.getIdSommet(i + 1)];
                Sommet s2 = vertex[cur.getIdSommet(i + 2)];

                if (estTrigo((but - s2), (but - s1))) {
                    if (cur.getNeighbor(i) != 0){
                        cur = triangles[cur.getNeighbor(i) - 1];
                        break;
                    }
                }
                if(i == 2)
                    return -1;
            }
        }
        return cur.getId();
    }

    void Triangulation2D::splitTriangle(int idTri, int idSommet) {
        TriangleTopo triangleASplit = triangles[idTri-1];
        TriangleTopo triangleB = TriangleTopo(idSommet, triangleASplit.getIdSommet(1), triangleASplit.getIdSommet(2), triangles.size()+1);
        TriangleTopo triangleC = TriangleTopo(idSommet, triangleASplit.getIdSommet(2), triangleASplit.getIdSommet(0), triangles.size()+2);

        vertex[idSommet].setIdTriangle(triangleC.getId());
        vertex[triangleASplit.getIdSommet(1)].setIdTriangle(triangleB.getId());
        vertex[triangleASplit.getIdSommet(2)].setIdTriangle(triangleC.getId());

        triangleB.setNeighbor(idTri, 2);
        triangleB.setNeighbor(triangleC.getId(), 1);
        triangleB.setNeighbor(triangleASplit.getNeighbor(0), 0);
        if(triangleASplit.getNeighbor(0) != 0) {
            TriangleTopo &voisinB = triangles[triangleASplit.getNeighbor(0) - 1];

            for (int i = 0; i < 3; i++) {
                if (voisinB.getNeighbor(i) == idTri) {
                    voisinB.setNeighbor(triangles.size() + 1, i);
                }
            }
        }
        triangleC.setNeighbor(idTri, 1);
        triangleC.setNeighbor(triangleB.getId(), 2);
        triangleC.setNeighbor(triangleASplit.getNeighbor(1), 0);

        if(triangleASplit.getNeighbor(1) != 0) {
            TriangleTopo &voisinC = triangles[triangleASplit.getNeighbor(1) - 1];
            for (int i = 0; i < 3; i++) {
                if (voisinC.getNeighbor(i) == idTri) {
                    voisinC.setNeighbor(triangles.size() + 2, i);
                }
            }
        }

        triangles[triangleASplit.getId() - 1].setNeighbor(triangles.size()+1 ,0);
        triangles[triangleASplit.getId() - 1].setNeighbor(triangles.size()+2 ,1);

//    triangleASplit.setNeighbor(triangles.size()+1 ,0);
//    triangleASplit.setNeighbor(triangles.size()+2 ,1);

        triangles.push_back(triangleB);
        triangles.push_back(triangleC);
        triangles[triangleASplit.getId() - 1].setIdSommet(idSommet, 2);

    }

    void Triangulation2D::linkTriangle(int idNewTriangle, int idOldTriangle, const couple &c) {
        TriangleTopo* t = &triangles[idOldTriangle-1];
        int s1,s2;
        s1 = triangles[idOldTriangle-1].getIdSommet(0);;
        s2 = triangles[idOldTriangle-1].getIdSommet(1);

        if(s1 == c.p1 || s1 == c.p2){
            if(s2 == c.p1 || s2 == c.p2){
                t->setNeighbor(idNewTriangle, 2);
            }
            else{
                t->setNeighbor(idNewTriangle, 1);
            }
        }
        else{
            t->setNeighbor(idNewTriangle, 0);
        }
    }

    couple Triangulation2D::getArreteAdjacent(const TriangleTopo &t1, const TriangleTopo &t2) const {
        int i;
        for(i = 0; i < 3; i++){
            if(t1.getNeighbor(i) == t2.getId())
                break;
        }
        switch (i){
            case 0:
                return couple{1, 2};
            case 1:
                return couple{2, 0};
            case 2:
                return couple{0, 1};
            default:
                return couple{-1,-1};
        }
    }

    couple Triangulation2D::getPointsAdjacent(const TriangleTopo &t1, const TriangleTopo &t2) const {
        int i;
        for(i = 0; i < 3; i++){
            if(t1.getNeighbor(i) == t2.getId())
                break;
        }
        switch (i){
            case 0:
                return couple{t1.getIdSommet(1), t1.getIdSommet(2)};
            case 1:
                return couple{t1.getIdSommet(2), t1.getIdSommet(0)};
            case 2:
                return couple{t1.getIdSommet(0), t1.getIdSommet(1)};
            default:
                return couple{-1,-1};
        }
    }

    void Triangulation2D::addTriangleIntern(int idTri, int idPoint) {
        splitTriangle(idTri, idPoint);

        if(triangles[triangles.size()-1].estExterieur() != -1)
            idTriExtern.push_back(triangles[triangles.size()-1].getId());

        if(triangles[triangles.size()-2].estExterieur() != -1) {
            idTriExtern.push_back(triangles[triangles.size() - 2].getId());
        }
    }

    void Triangulation2D::addTriangleExtern(int idPoint) {
        std::map<couple,int> map;
        for(faceExtIterator tri = faceExtBegin(); tri < faceExtEnd(); tri++) {
            int test = (*tri)->estExterieur();

            if (test != -1) {
                for (int k = 0; k < 3; k++) {
                    if ((*tri)->getNeighbor(k) == 0) {
                        int id1, id2;
                        switch (k) {
                            case 0:
                                id1 = (*tri)->getIdSommet(1);
                                id2 = (*tri)->getIdSommet(2);
                                break;
                            case 1:
                                id1 = (*tri)->getIdSommet(2);
                                id2 = (*tri)->getIdSommet(0);
                                break;
                            case 2:
                                id1 = (*tri)->getIdSommet(0);
                                id2 = (*tri)->getIdSommet(1);
                                break;
                            default :
                                id1 = -1;
                                id2 = -1;
                                break;
                        }
                        if (estTrigo((vertex[idPoint] - vertex[id2]), (vertex[idPoint] - vertex[id1]))) {
                            TriangleTopo t(idPoint, id2, id1, static_cast<int>(triangles.size() + 1));
                            int min1 = std::min(idPoint, id1);
                            int max1 = std::max(idPoint, id1);

                            int min2 = std::min(idPoint, id2);
                            int max2 = std::max(idPoint, id2);

                            vertex[id2].setIdTriangle(triangles.size() + 1);
                            vertex[id1].setIdTriangle(triangles.size() + 1);
                            vertex[idPoint].setIdTriangle(triangles.size() + 1);

                            t.setNeighbor(0, 1);
                            t.setNeighbor(0, 2);
                            t.setNeighbor((*tri)->getId(), 0);
                            triangles[(*tri)->getId() - 1].setNeighbor(static_cast<int>(triangles.size() + 1), k);

                            if (map[{min1, max1}] == 0) {
                                map[{min1, max1}] = static_cast<int>(triangles.size() + 1);
                            } else {
                                t.setNeighbor(map[{min1, max1}], 1);
                                int idTri = map[{min1, max1}];
                                linkTriangle(static_cast<int>(triangles.size() + 1), idTri, {min1, max1});
                            }

                            if (map[{min2, max2}] == 0) {
                                map[{min2, max2}] = static_cast<int>(triangles.size() + 1);
                            } else {
                                t.setNeighbor(map[{min2, max2}], 2);
                                int idTri = map[{min2, max2}];
                                linkTriangle(static_cast<int>(triangles.size() + 1), idTri, {min2, max2});
                            }
                            triangles.push_back(t);
                            idTriExtern.push_back(t.getId());
                        }
                    }
                }
            }
        }
    }

    void Triangulation2D::checkExterieur(int idTri) {
        if(triangles[idTri - 1].estExterieur() == -1){
            for(std::vector<int>::const_iterator it = idTriExtern.begin(); it < idTriExtern.end(); it++){
                if((*it) == idTri) {
                    idTriExtern.erase(it);
                }
            }
        }
    }

    Maillage Triangulation2D::getMaillage() {
        Maillage ret;
        ScatterPlot SP;
        std::vector<unsigned int> VC;
        for(vector3 s : vertex.getVector()){
            SP.addPlot(s);
        }
        for(TriangleTopo t : triangles){
            VC.push_back((unsigned int) t.getIdSommet(1));
            VC.push_back((unsigned int) t.getIdSommet(0));
            VC.push_back((unsigned int) t.getIdSommet(2));
        }

        ret.setIndiceBuffer(VC);
        ret.setVertexBuffer(SP);
        ret.setNbIndiceFace(3);
        return ret;
    }

    void Triangulation2D::perturbe(int idPoint) {
        Sommet s = vertex[idPoint];

        double p1 = (double) rand() / RAND_MAX;
        double p2 = (double) rand() / RAND_MAX;

        vertex[idPoint].setX(s.x() + -(epsilon/2) + p1 * epsilon);
        vertex[idPoint].setY(s.y() + -(epsilon/2) + p2 * epsilon);
    }

    void Triangulation2D::lawson(std::queue<int>& file) {
        while(file.size() > 0){
            int top = file.front();
            TriangleTopo triTopo = triangles[top - 1];
            for(int k = 0; k < 3; k++) {
                if(triTopo.getNeighbor(k) != 0) {
                    couple tmp = getArreteAdjacent(triangles[triTopo.getNeighbor(k) - 1], triTopo);
                    int p = triangles[triTopo.getNeighbor(k) - 1].getIdSommet(3 - tmp.p1 - tmp.p2);
                    if (appartientCercle(top, p) < 0.0f) {
                        faceCirculator fc = faceAround(p);
                        bool fin = false;
                        do {
                            if (getArreteAdjacent(triangles[top - 1], *(*fc)).p1 != -1 &&
                                (*fc)->getIndexInTriangle(p) != -1) {
                                //                        ok = true;
                                fin = true;
                                flipTriangle(top, (*fc)->getId());
                                for (int voisin = 0; voisin < 3; voisin++) {
                                    if (triangles[top - 1].getNeighbor(voisin) > 0)
                                        file.push(triangles[top - 1].getNeighbor(voisin));

                                    if ((*fc)->getNeighbor(voisin) > 0)
                                        file.push((*fc)->getNeighbor(voisin));
                                }
                            }
                            if (fin)
                                break;
                            fc++;
                        } while (fc != faceAround(p) || fin);
                    }
                }
            }
            file.pop();
        }
    }

    bool Triangulation2D::flipTriangle(int idTri1, int idTri2) {
        TriangleTopo& tri1 = triangles[idTri1-1];
        TriangleTopo& tri2 = triangles[idTri2-1];

        couple arrete1 = getArreteAdjacent(tri1, tri2);
        couple arrete2 = getArreteAdjacent(tri2, tri1);

        if(arrete1.p1 == -1 || arrete2.p1 == -1)
            return false;

        int indexT1 = 3 - arrete1.p1 - arrete1.p2;
        int indexT2 = 3 - arrete2.p1 - arrete2.p2;

        int idVoisin1 = tri1.getNeighbor(arrete1.p2);
        int idVoisin2 = tri2.getNeighbor(arrete2.p2);

        tri1.setIdSommet(tri2.getIdSommet(indexT2), arrete1.p1);
        tri2.setIdSommet(tri1.getIdSommet(indexT1), arrete2.p1);

        tri1.setNeighbor(tri2.getId(), arrete1.p2);
        tri1.setNeighbor(idVoisin2, indexT1);

        tri2.setNeighbor(tri1.getId(), arrete2.p2);
        tri2.setNeighbor(idVoisin1, indexT2);

        if(idVoisin1 > 0){
            TriangleTopo& voisin = triangles[idVoisin1 - 1];
            for(int i = 0; i < 3; i++) {
                if (voisin.getNeighbor(i) == tri1.getId()){
                    voisin.setNeighbor(tri2.getId(), i);
                    break;
                }
            }
        }
        if(idVoisin2 > 0){
            TriangleTopo& voisin = triangles[idVoisin2 - 1];
            for(int i = 0; i < 3; i++) {
                if (voisin.getNeighbor(i) == tri2.getId()){
                    voisin.setNeighbor(tri1.getId(), i);
                    break;
                }
            }
        }

        bool t1Ext = tri1.estExterieur() != -1;
        bool t2Ext = tri2.estExterieur() != -1;

        bool okt1 = false;
        bool okt2 = false;

        int t1Id = tri1.getId();
        int t2Id = tri2.getId();

        for(std::vector<int>::const_iterator it = idTriExtern.begin(); it < idTriExtern.end(); it++){
            if((*it) == t1Id){
                okt1 = true;
                if(!t1Ext)
                    idTriExtern.erase(it);
            }

            if((*it) == t2Id){
                okt2 = true;
                if(!t2Ext)
                    idTriExtern.erase(it);
            }
            if(okt1 && okt2)
                break;
        }

        for(int i = 0; i < 3; i++){
            vertex[tri1.getIdSommet(i)].setIdTriangle(tri1.getId());
            vertex[tri2.getIdSommet(i)].setIdTriangle(tri2.getId());
        }

        if(!okt1 && t1Ext)
            idTriExtern.push_back(t1Id);
        if(!okt2 && t2Ext)
            idTriExtern.push_back(t2Id);
        return true;
    }

    TriangulationDelaunay2D Triangulation2D::makeDelaunay() {
        std::queue<int> file;
        for(int i = 1; i < triangles.size() + 1; i++)
            file.push(i);
        lawson(file);

        TriangulationDelaunay2D ret(*this);
        return ret;
    }

    bool Triangulation2D::checkRange(double x, double y, double range) {
        vector3 newPoint(x,y,0);
        for(Sommet s : vertex.getVector()){
            vector3 p(s.x(), s.y(), 0.0);
            if((newPoint - p).length() < range){
                return false;
            }
        }
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TriangulationDelaunay2D::TriangulationDelaunay2D(Triangulation2D &tri) {
        vertex.reserve(tri.getNbVertex());
        triangles.reserve(tri.getNbFaces());
        idTriExtern.reserve(tri.idTriExtern.size());


        for(int i = 0; i < tri.getNbVertex(); i++)
            vertex.addPlot(tri.vertex[i]);

        for(TriangleTopo t : tri.triangles)
            triangles.push_back(t);

        for(int i : tri.idTriExtern)
            idTriExtern.push_back(i);
    }

    std::vector<float> TriangulationDelaunay2D::getVoronoi() {
        std::vector<float> ret;
        std::map<couple, int> map;
        ret.reserve(vertex.getSize() * 6);

        for(int i = 0; i < vertex.getSize(); i++) {
            faceCirculator fc = faceAround(i);
            bool fin = false;

            while (!fin) {
                TriangleTopo t1 = *(*fc);
                fc++;
                TriangleTopo t2 = *(*fc);

                if (!(fc != faceAround(i)))
                    fin = true;

                int p1 = std::min(t1.getId(), t2.getId());
                int p2 = std::max(t1.getId(), t2.getId());
                if (map[{p1, p2}] == 0) {
                    map[{p1, p2}] = 1;
//                std::cout << p1 << " " << p2 << std::endl;
                    vector3 v1 = vertex[t1.getIdSommet(0)];
                    vector3 v2 = vertex[t1.getIdSommet(1)];
                    vector3 v3 = vertex[t1.getIdSommet(2)];

                    vector3 vor = t1.computeVoronoi(v1, v2, v3);
                    ret.push_back((float) vor.x());
                    ret.push_back((float) vor.y());
                    ret.push_back((float) vor.z());

                    v1 = vertex[t2.getIdSommet(0)];
                    v2 = vertex[t2.getIdSommet(1)];
                    v3 = vertex[t2.getIdSommet(2)];

                    vor = t2.computeVoronoi(v1, v2, v3);
                    ret.push_back((float) vor.x());
                    ret.push_back((float) vor.y());
                    ret.push_back((float) vor.z());

                }
            }
        }
        return ret;
    }

    Maillage TriangulationDelaunay2D::getVoronoiMesh() {

        Maillage ret;
        ScatterPlot SP;
        std::vector<unsigned int> VC;

        std::map<couple, int> map;

        for(TriangleTopo t : triangles){
            vector3 v1 = vertex[t.getIdSommet(0)];
            vector3 v2 = vertex[t.getIdSommet(1)];
            vector3 v3 = vertex[t.getIdSommet(2)];

            vector3 vor = t.computeVoronoi(v1, v2, v3);
            SP.addPlot(vor);
        }

        for(int i = 0; i < vertex.getSize(); i++) {
            faceCirculator fc = faceAround(i);
            bool fin = false;

            while (!fin) {
                TriangleTopo t1 = *(*fc);
                fc++;
                TriangleTopo t2 = *(*fc);

                if (!(fc != faceAround(i)))
                    fin = true;

                int p1 = std::min(t1.getId(), t2.getId());
                int p2 = std::max(t1.getId(), t2.getId());
                if (map[{p1, p2}] == 0) {
                    map[{p1, p2}] = 1;

                    VC.push_back(p1-1);
                    VC.push_back(p2-1);
                }
            }
        }

        ret.setIndiceBuffer(VC);
        ret.setVertexBuffer(SP);
        ret.setNbIndiceFace(2);
        return ret;
    }

    void TriangulationDelaunay2D::addTriangleIntern(int idTri, int idPoint) {
        std::queue<int> queue;
        Triangulation2D::addTriangleIntern(idTri, idPoint);
        queue.push(idTri);
        queue.push(triangles.size() - 1);
        queue.push(triangles.size());
        lawson(queue);
    }

    void TriangulationDelaunay2D::addTriangleExtern(int idPoint) {
        std::queue<int> queue;

        int nbTri = triangles.size();

        Triangulation2D::addTriangleExtern(idPoint);
        for(int i = nbTri+1; i < triangles.size()+1; i++)
            queue.push(i);
        lawson(queue);
    }

    Maillage TriangulationDelaunay2D::crust() {
        Maillage ret;
        ScatterPlot SP;
        int nbVertexOrig = getNbVertex();
        std::vector<unsigned int> VC;
        std::vector<float> pVoronoi = getVoronoi();

        TriangulationDelaunay2D tmp(*this);

        for(vector3 s : vertex.getVector()){
            SP.addPlot(s);
        }

        for(int i = 0; i < pVoronoi.size(); i++){
            tmp.addPoint(pVoronoi[i], pVoronoi[i+1]);
            i++;
            i++;
        }

        for(TriangleTopo t : tmp.triangles){
            int cpt = 0;
            for(int i = 0; i < 3; i++)
                if(t.getIdSommet(i) < nbVertexOrig)
                    cpt++;
            if(cpt == 2){
                for(int i = 0; i < 3; i++)
                    if(t.getIdSommet(i) < nbVertexOrig)
                        VC.push_back(t.getIdSommet(i));
            }
        }

        ret.setIndiceBuffer(VC);
        ret.setVertexBuffer(SP);
        ret.setNbIndiceFace(2);
        return ret;
    }

    TriangulationDelaunay2D TriangulationDelaunay2D::ruppert(float minAngle) {
        TriangulationDelaunay2D ret(*this);
        std::stack<int> badTri;
        for(int i = 1; i < ret.triangles.size() + 1; i++)
            if(checkAngle(i, minAngle))
                badTri.push(i);

        while(badTri.size() > 0){
            int idTri = badTri.top();
            int nbTri = ret.triangles.size();
            TriangleTopo tri = ret.triangles[idTri - 1];
            vector3 a = ret.vertex[tri.getIdSommet(0)];
            vector3 b = ret.vertex[tri.getIdSommet(1)];
            vector3 c = ret.vertex[tri.getIdSommet(2)];
            vector3 vor = tri.computeVoronoi(a,b,c);

            ret.addPoint(vor.x(), vor.y(), 0.0);

            if(ret.checkAngle(idTri, minAngle))
                badTri.push(idTri);

            for(int i = nbTri + 1; i < ret.triangles.size() + 1; i++)
                if(ret.checkAngle(i, minAngle))
                    badTri.push(i);

            badTri.pop();
        }

        return ret;
    }

    bool TriangulationDelaunay2D::checkAspectRation(int idTri, float maxAR) {
        TriangleTopo t = triangles[idTri - 1];
        vector3 a = vertex[t.getIdSommet(0)];
        vector3 b = vertex[t.getIdSommet(1)];
        vector3 c = vertex[t.getIdSommet(2)];

        float AR = t.computeAspectRation(a,b,c);

        if(AR > maxAR)
            return true;
        return false;
    }

    bool TriangulationDelaunay2D::checkAngle(int idTri, float minAngle) {
        TriangleTopo t = triangles[idTri - 1];
        vector3 a = vertex[t.getIdSommet(0)];
        vector3 b = vertex[t.getIdSommet(1)];
        vector3 c = vertex[t.getIdSommet(2)];

        float angle = t.minAngle(a,b,c);
        //std::cout << angle << std::endl;
        if(angle < minAngle)
            return true;
        return false;
    }

}


