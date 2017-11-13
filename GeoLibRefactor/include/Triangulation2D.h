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

/*!
 * \file Triangulation2D.h
 * \brief structure couple + Triangulation2D + TriangulationDelaunay2D
 * \author FLECKINGER Julien
 */
namespace GeoLib {

    /*!
     * \struct structure utilisé en interne
     */
    struct couple{
        int p1,p2; //!< couple d'entier
        bool operator< (const couple& c2) const //!< surcharge de l'opérateur < pour être utilisé dans les maps.
        {return p1 < c2.p1 || p2 < c2.p2;}
    };

    class TriangulationDelaunay2D;
    /*!\class Triangulation2D
     * \brief Permet de trianguler des points de façon naive + passage en triangulation de delaunay
     */
    class Triangulation2D {
      public:
        friend class TriangulationDelaunay2D;
        /*!
         * \brief charge des points depuis un fichier
         * @param filename nom du fichier
         * @param nbDim format du fichier 2 = x,y 3 = x,y,z
         */
        bool loadPoint(std::string filename, int nbDim = 3);

        /*!
         * \brief getter
         */
        Sommet& getVertex(int index){ return vertex.getPlot(index);};
        /*!
         * \brief getter
         */
        TriangleTopo& getTriangles(int index) { return triangles[index];};

        /*!
         * @return nombre de points de la triangulation.
         */
        int getNbVertex() const {return vertex.getSize();};
        /*!
         * @return nombre de triangles (faces) de la triangulation.
         */
        int getNbFaces() const {return triangles.size();};

        /*!
         * \brief ajoute un point avec les coordonnées x,y de façon naïve.
         */
        void addPoint(double x, double y, double epsilon = 0.001);

        /*!
         * \brief renvoie un Maillage pour affichage de la triangulation
         * @return un Maillage
         */
        Maillage getMaillage();

        TriangulationDelaunay2D makeDelaunay();

        faceExtIterator faceExtBegin() {return faceExtIterator(&triangles, &idTriExtern, 0);};
        faceExtIterator faceExtEnd() {return faceExtIterator(&triangles, &idTriExtern, idTriExtern.size());};
        faceCirculator faceAround(int p) {return faceCirculator(&triangles, &vertex, p);};
        vertexCirculator vertexAround(int p) {return vertexCirculator(&triangles, p);};

      protected:
        /*!
         * \brief déplace légerement un point pour éviter les problemes.
         */
        void perturbe(int idPoint);
        /*!
         * \brief Fonction virtuelle qui ajoute un point qui ce trouve à l'intérieur d'un triangle.
         */
        virtual void addTriangleIntern(int idTri, int idPoint);
        /*!
         * \brief Fonction virtuelle qui ajoute un point qui ce trouve à l'extérieur de la triangulation.
         */
        virtual void addTriangleExtern(int idPoint);
        /*!
         * \brief Prédicat d'orientation.
         */
        bool estTrigo(const vector3& a, const vector3& b){ return a.cross(b).z() > 0;};
        void addPoint(int idPoint);
        /*!
         *  \brief prédicat qui est vrai si le point idPoint appartient au triangle idTri
         */
        bool appartientTriangle(int idTri, int idPoint) const;
        /*!
         *  \brief prédicat qui renvoie < 0 si le point s est dans le cercle circonsrit au triangle idTri
         */
        double appartientCercle(int idTri, int s) const;
        /*!
         *  \brief Algo marche orientation
         *  @return -1 si hors de la triangulation, l'id du triangle sinon
         */
        int appartientMesh(int idPoint);
        /*!
         *  \brief Découpe le triangle idTri en 3 avec le point idSommet
         */
        void splitTriangle(int idTri, int idSommet);
        void linkTriangle(int idNewTriangle, int idOldTriangle, const couple& c);
        /*!
         * @return les Id des sommets communs entre t1 et t2
         */
        couple getArreteAdjacent(const TriangleTopo& t1, const TriangleTopo& t2) const;
        /*!
         * @return les coordonnées des sommets communs entre t1 et t2
         */
        couple getPointsAdjacent(const TriangleTopo& t1, const TriangleTopo& t2) const;
        /*!
         *  \brief vérifie que le triangle idTri est toujours à l'exterieur (modifie le tableau idTriExtern)
         */
        void checkExterieur(int idTri);

        /*!
         * \brief Algo de lawson
         */
        void lawson(std::queue<int>& queue);

        /*!
         * \brief flip l'arrete entre tri1 et tri2
         */
        bool flipTriangle(int idTri1, int idTri2);

        ScatterVertex vertex;
        std::vector<TriangleTopo> triangles;
        std::vector<int> idTriExtern;
        double epsilon = 0.0001;
    };

    /*!\class TriangulationDelaunay2D
     * \brief Permet de trianguler des points de façon unique avec delaunay + voronoi
     */
    class TriangulationDelaunay2D : public Triangulation2D{
      public:
        TriangulationDelaunay2D() = default;
        TriangulationDelaunay2D(Triangulation2D& tri);
        /*!
         * \brief renvoie un Maillage pour affichage des cellules de voronoi
         * @return un Maillage
         */
        Maillage getVoronoiMesh();
        /*!
         * \brief renvoie un Maillage pour affichage de la courbe
         * @return un Maillage
         */
        Maillage crust();
        TriangulationDelaunay2D ruppert(float maxAspectRatio);
    protected:
        /*!
         * @return un tableau contenant les coordonnées des sommets de voronoi
         */
        std::vector<float> getVoronoi();
        /*!
         *  \brief surcharge de la fonction addTriangleIntern qui permet de faire du delaunay incremental
         */
        void addTriangleIntern(int idTri, int idPoint);
        /*!
         *  \brief surcharge de la fonction addTriangleExtern qui permet de faire du delaunay incremental
         */
        void addTriangleExtern(int idPoint);

        bool checkAspectRation(int idTri, float maxAR);

        bool checkAngle(int idTri, float minAngle);

    };
}

#endif //GEOALGO_TRIANGULATION2D_H
