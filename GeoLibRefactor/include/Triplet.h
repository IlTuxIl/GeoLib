//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_TRIPLET_H
#define GEOALGO_TRIPLET_H

/*!
 * \file Triplet.h
 * \brief Triplet
 * \author FLECKINGER Julien
 */
namespace GeoLib {

    /*!\class Triplet
     * \brief Template permettant d'avoir un triplet d'entier, double ,float ...
     */
    template<class T>
    class Triplet {
      public:
        Triplet() = default;

        Triplet(T a, T b, T c = 0) {
            val[0] = a;
            val[1] = b;
            val[2] = c;
        };

        T x() const { return val[0]; }

        T y() const { return val[1]; }

        T z() const { return val[2]; }

        void setX(T v) { val[0] = v; }

        void setY(T v) { val[1] = v; }

        void setZ(T v) { val[2] = v; }

        T& operator[](int i) { return val[i % 3]; };
        const T& operator[](int i) const { return val[i % 3]; };
      protected:
        T val[3];
    };

}
#endif //GEOALGO_TRIPLET_H
