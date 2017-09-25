//
// Created by julien on 07/09/17.
//

#ifndef GEOLIB_SOMMET_H
#define GEOLIB_SOMMET_H

#include "Vector.h"

class Sommet : public vector3 {
public:
    Sommet() = default;
    Sommet(const Sommet& _sommet);
    Sommet(const vector3& vec);
    void setIdTriangle(int _id){idTriangle = _id;}
    int getIdTriangle(){return idTriangle;}
    Sommet& operator=(const vector3& vec);
    Sommet& operator=(const Sommet& vec);
    friend std::ostream& operator<< (std::ostream&, const Sommet&);

private:
    int idTriangle;
};


#endif //GEOLIB_SOMMET_H
