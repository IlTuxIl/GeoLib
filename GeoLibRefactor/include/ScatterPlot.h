//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_SCATTERPLOT_H
#define GEOALGO_SCATTERPLOT_H


#include "Vector.h"
#include "Sommet.h"
#include <vector>


/*!
 * \file ScatterPlot.h
 * \brief tableau de points/sommets
 * \author FLECKINGER Julien
 */
namespace GeoLib {
    /*!\class ScatterPlot
     * \brief Permet de stocker un tableau de vector3 (point), utilisé dans maillage
     */
    class ScatterPlot {
      public :
        /*!
         * \brief Ajoute un point dans le tableau
         * @param p Le nouveau point
         */
        void addPlot(vector3 p) { plot.push_back(p); }
        /*!
         *
         * @param i L'index du vector3 dans le tableau
         * @return le ieme vector3
         */
        vector3& getPlot(int i) { return plot[i]; }
        /*!
         *
         * @return le nombre d'elements dans le tableau
         */
        int getSize() const {return plot.size();}
        /*!
         *
         * @param i L'index du vector3 dans le tableau
         * @return le ieme vector3
         */
        vector3&operator[](int i){return plot[i];}
        void reserve(int a){plot.reserve(a);};
        /*!
         *
         * @return le tableau complet
         */
        std::vector<vector3> getVector(){ return plot;};
        const std::vector<vector3> getVector() const { return plot;};

    private:
        std::vector<vector3> plot;
    };
    /*!\class ScatterPlot
     * \brief Permet de stocker un tableau de sommet (point + id), utilisé dans les class Triangulation2D
     */
    class ScatterVertex {
      public :
        /*!
         * \brief Ajoute un sommet dans le tableau
         * @param p Le nouveau sommet
         */
        void addPlot(Sommet p) { plot.push_back(p); }
        void reserve(int a){plot.reserve(a);};

        /*!
         * @param i L'index du sommet dans le tableau
         * @return le ieme sommet
         */
        Sommet& getPlot(int i) { return plot[i]; }
        /*!
         * @param i L'index du sommet dans le tableau
         * @return le ieme sommet
         */
        Sommet&operator[](int i){return plot[i];}
        const Sommet& operator[](int i)const {return plot[i];}

        /*!
         * @return le tableau complet
         */
        std::vector<Sommet> getVector(){ return plot;};

        int getSize() const {return plot.size();}

      private:
        std::vector<Sommet> plot;
    };
}
#endif //GEOALGO_SCATTERPLOT_H
