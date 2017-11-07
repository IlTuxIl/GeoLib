//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_TRIANGULATION2D_H
#define GEOALGO_TRIANGULATION2D_H

#include <string>
#include <queue>
#include "Iterateur.h"
#include "ScatterPlot.h"
#include "Triangle.h"
#include "Maillage.h"

namespace GeoLib {

    /*!
     * \struct structure utilisé en interne
     */
    struct couple{
        int p1,p2; //!< couple d'entier
        bool operator< (const couple& c2) const //!< surcharge de l'opérateur < pour être utilisé dans les maps.
        {return p1 < c2.p1 || p2 < c2.p2;}
    };

    class Triangulation2D {
      public:

        bool loadPoint(std::string filename, int nbDim = 3);

        Sommet& getVertex(int index){ return vertex.getPlot(index);};
        TriangleTopo& getTriangles(int index) { return triangles[index];};

        int getNbVertex() const {return vertex.getSize();};
        int getNbFaces() const {return triangles.size();};

        void addPoint(double x, double y);
        std::vector<double> getVertex();
        std::vector<unsigned int> getIndex();

        Maillage getMaillage();
        faceExtIterator faceExtBegin() {return faceExtIterator(&triangles, &idTriExtern, 0);};
        faceExtIterator faceExtEnd() {return faceExtIterator(&triangles, &idTriExtern, idTriExtern.size());};
        faceCirculator faceAround(int p) {return faceCirculator(&triangles, &vertex, p);};
        vertexCirculator vertexAround(int p) {return vertexCirculator(&triangles, p);};


      protected:
        void perturbe(int idPoint);
        virtual void addTriangleIntern(int idTri, int idPoint);
        virtual void addTriangleExtern(int idPoint);

        bool estTrigo(const vector3& a, const vector3& b){ return a.cross(b).z() > 0;};

        void addPoint(int idPoint);
        bool appartientTriangle(int idTri, int idPoint) const;
        double appartientCercle(int idTri, int s) const;
        int appartientMesh(int idPoint);

        void splitTriangle(int idTri, int idSommet);
        void linkTriangle(int idNewTriangle, int idOldTriangle, const couple& c);
        couple getArreteAdjacent(const TriangleTopo& t1, const TriangleTopo& t2) const;
        couple getPointsAdjacent(const TriangleTopo& t1, const TriangleTopo& t2) const;

        void checkExterieur(int idTri);

        ScatterVertex vertex;
        std::vector<TriangleTopo> triangles;
        std::vector<int> idTriExtern;
        double epsilon = 0.0001;
    };

    class TriangulationDelaunay2D : public Triangulation2D{
      public:
        std::vector<float> getVoronoi();
        Maillage getVoronoiMesh();
        Maillage crust();
      protected:
        void addTriangleIntern(int idTri, int idPoint);
        void addTriangleExtern(int idPoint);
        bool flipTriangle(int idTri1, int idTri2);
        void lawson(std::queue<int>& queue);
    };

}

#endif //GEOALGO_TRIANGULATION2D_H
