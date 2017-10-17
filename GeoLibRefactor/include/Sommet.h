//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_SOMMET_H
#define GEOALGO_SOMMET_H

#include "iostream"
#include "Triplet.h"

namespace GeoLib {
    class Sommet : public Triplet<double> {
    public:
        Sommet() = default;
        Sommet(const Sommet &_sommet);
        Sommet(double x, double y, double z);
        Sommet(const Triplet &vec);

        void setIdTriangle(int _id) { idTriangle = _id; }
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
