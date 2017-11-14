//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_VECTOR_H
#define GEOALGO_VECTOR_H


#include "Triplet.h"
#include "Sommet.h"
#include <iostream>

/*!
 * \file Vector.h
 * \brief vector et index
 * \author FLECKINGER Julien
 */
namespace GeoLib {

    /*!\class vector3
     * \brief vector3 = triplet de double
     */
    class vector3 : public Triplet<double> {
      public:
        vector3(double x = 0, double y = 0, double z = 0);
        vector3(const vector3&);
        vector3(const Sommet& s);

        vector3 operator+(const vector3&);
        vector3 operator-(const vector3&)const;
        vector3 operator-(const vector3&);
        vector3 operator*(float);
        vector3 operator/(float);
        vector3& operator=(const vector3&);
        bool operator==(const vector3&);
        double& operator[](int);

        friend std::ostream& operator<< (std::ostream&, const vector3&);

        double dot(const vector3&) const;
        vector3 cross(const vector3&) const;
        double length();
        double length2();
        vector3 normalize();
    };

    /*!\class index
     * \brief index = triplet d'entier
     */
    class index : public Triplet<int>{
      public:
        index(int x = 0, int y = 0, int z = 0) : Triplet(x,y,z){};
        int operator[](int id) {return Triplet::operator[](id);};
        const int operator[](int id) const {return Triplet::operator[](id);};
    };

}
#endif //GEOALGO_VECTOR_H
