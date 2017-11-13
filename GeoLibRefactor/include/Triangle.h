//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_TRIANGLE_H
#define GEOALGO_TRIANGLE_H

#include "Sommet.h"
#include "Vector.h"

/*!
 * \file Triangle.h
 * \brief Définition des triangle et triangleTopo utilisé pour la triangulation.
 * \author FLECKINGER Julien
 */
namespace GeoLib {
    /*!\class Triangle
    * \brief définition d'un triangle simple (non utilisé)
    */
    class Triangle {
    public:
        /*!
         * \brief Constructeur par défaut
         */
        Triangle() = default;
        /*!
         * \brief Constructeur avec initialisation
         */
        Triangle(int x, int y, int z) : idSommet(x, y, z){};
        /*!
         * \brief getter
         */
        int getIdSommet(int id) const;
        /*!
         * \brief setter
         */
        void setIdSommet (int sommet, int indexDansTriangle);
        /*!
         * \brief getter
         */
        int getIndexInTriangle(int _idSommet);

    protected:
        index idSommet;
    };

    /*!\class TriangleTopo
    * \brief définition d'un triangle topologique (Triangle + voisinage, voronoi, id) utilisé par les Triangulation2D
    */
    class TriangleTopo : public Triangle{
    public:
        /*!
         * \brief constructeur par défaut
         */
        TriangleTopo();
        /*!
         * \brief constructeur avec initialisation
         */
        TriangleTopo(int x, int y, int z, int _id);
        /*!
         *
         * @return true sur le triangle à moins de 3 voisins
         */
        bool estExterieur();
        /*!
         * \brief setter
         */
        void setIdSommet(int sommet, int indexDansTriangle);
        /*!
         * \brief setter
         */
        void setId(int _id){id = _id;};
        /*!
         * \brief getter
         */
        int getId()const{return this->id;}
        /*!
         * \brief setter
         */
        void setNeighbor(int idtri, int id);
        /*!
         * \brief getter
         */
        int getNeighbor(int id) const ;
        /*!
         * \brief calcul le centre de voronoi uniquement si les points sont modifié.
         * @param abc les coordonnées des sommets du triangle
         * @return le centre de voronoi
         */
        vector3 computeVoronoi(vector3 a, vector3 b, vector3 c);

        double computeAspectRation(vector3 a, vector3 b, vector3 c);

        double minAngle(vector3 a, vector3 b, vector3 c);

    private:
        bool updateVoronoi = true;
        vector3 voronoi;
        index idVoisin;
        int id = -1;
    };
}

#endif //GEOALGO_TRIANGLE_H
