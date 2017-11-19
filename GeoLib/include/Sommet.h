//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_SOMMET_H
#define GEOALGO_SOMMET_H

#include "iostream"
#include "Triplet.h"

/*!
 * \file Sommet.h
 * \brief Un triangle est définie par 3 sommets, un sommet à pour id le dernier triangle avec lequel il est définie
 * \author FLECKINGER Julien
 */
namespace GeoLib {
    /*!\class Sommet
     * \brief Triplet de double + id, id = id du dernier triangle définie à partir du sommet
     */
    class Sommet : public Triplet<double> {
    public:
        /*!
         * \brief Constructeur par défaut
         */
        Sommet() = default;
        Sommet(double x, double y, double z);
        Sommet(const Triplet &vec);

        /*!
         * \brief setter id
         */
        void setIdTriangle(int _id) { idTriangle = _id; }
        /*!
         * \brief getter id
         */
        int getIdTriangle() const { return idTriangle; }

        Sommet &operator=(const Triplet &vec);
        Sommet &operator=(const Sommet &vec);

        Sommet operator-(const Sommet& s);

        friend std::ostream &operator<<(std::ostream &, const Sommet &);
    private:
        int idTriangle;
    };
}

#endif //GEOALGO_SOMMET_H
