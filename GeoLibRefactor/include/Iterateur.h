//
// Created by julien on 10/10/17.
//

#ifndef GEOALGO_ITERATEUR_H
#define GEOALGO_ITERATEUR_H

#include <vector>
#include "Triangle.h"
#include "Sommet.h"
#include "ScatterPlot.h"

/*!
 * \file Iterateur.h
 * \brief Itérateur et circulateur pour la triangulation.
 * \author FLECKINGER Julien
 */
namespace GeoLib {
    /*!\class faceCirculator
     * \brief Permet de circuler et de récupérer tous les triangles,
     * dans le sens trigonométrique, autour d'un point.
     */
    class faceCirculator {
    public:
        /*!
         * \brief Construteur uniquement appelé par les class Triangulation
         * @param tri La liste des Triangles de la triangulation
         * @param _sv  La liste des Points de la triangulation
         * @param idSommet l'indice du point de départ
         */
        faceCirculator(std::vector<TriangleTopo> *tri, ScatterVertex* _sv, int idSommet);

        /*!
         * Permet d'itérer sur la face suivante.
         */
        void nextFace();

        /*!
         * @return l'indice du triangle courant.
         */
        int getIndex();

        /*!
         * @return un pointeur sur le triangle courant.
         */
        TriangleTopo *getFace();

        /*!
         * @return un pointeur sur le triangle courant.
         */
        TriangleTopo *operator*();

        /*!
         * Permet d'itérer sur la face suivante.
         */
        faceCirculator operator++(int);

        /*!
         * @return vrai si les 2 itérateurs pointent sur le même triangle.
         */
        bool operator!=(const faceCirculator &fc);

    private:
        ScatterVertex* sv;
        std::vector<TriangleTopo> *mesh;
        TriangleTopo *curTriangle;
        int curIndex;
        int startIndex;
    };

    /*!\class vertexCirculator
     * \brief Permet de circuler et de récupérer tous les points,
     * dans le sens trigonométrique, autour d'un point.
     */
    class vertexCirculator {
    public:
        /*!
         * \brief Construteur uniquement appelé par les class Triangulation
         * @param tri La liste des Triangles de la triangulation
         * @param _sv  La liste des Points de la triangulation
         * @param idSommet l'indice du point de départ
         */
        vertexCirculator(std::vector<TriangleTopo> *_mesh, int idSommet);

        /*!
         * Permet d'itérer sur le point suivante.
         */
        void nextVertex();

        /*!
         * @return l'indice du point courant.
         */
        int getIndex();

        /*!
         * @return un pointeur sur le point courant.
         */
        Sommet *getVertex();

        /*!
         * @return un pointeur sur le point courant.
         */
        Sommet *operator*();

        /*!
         * Permet d'itérer sur le point suivante.
         */
        vertexCirculator operator++(int);

        /*!
         * @return vrai si les 2 itérateurs pointent sur le même triangle.
         */
        bool operator!=(const vertexCirculator &vc);

    private:
        bool changeTriangle;
        std::vector<TriangleTopo> *mesh;
        TriangleTopo *curTriangle;
        int curIndex;
        int startIndex;
    };

    /*!\class faceExtIterator
     * \brief Permet de circuler et de récupérer tous les triangles extérieurs (qui ont moins de 3 voisins),
     * dans une triangulation.
     */
    class faceExtIterator {
    public:
        /*!
         * \brief Construteur uniquement appelé par les class Triangulation
         * @param tri La liste des Triangles de la triangulation
         * @param tab  La liste des index des triangles extérieurs
         * @param idSommet l'indice du triangle de départ
         */
        faceExtIterator(std::vector<TriangleTopo> *tri, std::vector<int> *tab, int startIndex);

        /*!
         * Permet d'itérer sur la face suivante.
         */
        void nextFace();

        /*!
         * @return un pointeur sur le triangle courant.
         */
        TriangleTopo *getFace();

        /*!
         * @return un pointeur sur le triangle courant.
         */
        TriangleTopo *operator*();

        /*!
         * Permet d'itérer sur la face suivante.
         */
        faceExtIterator operator++(int);

        /*!
         * Permet d'itérer sur la face précédente.
         */
        faceExtIterator operator--(int);

        /*!
         * Pour les boucles for
         */
        bool operator<(const faceExtIterator &fi);

    private:
        std::vector<TriangleTopo> *tabTriangle;
        std::vector<int> *tabIndex;
        int index = 0;
    };

}
#endif //GEOALGO_ITERATEUR_H
