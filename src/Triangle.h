//
// Created by julien on 05/09/17.
//

#ifndef GEOLIB_TRIANGLE_H
#define GEOLIB_TRIANGLE_H

#include "Sommet.h"

class Triangle {
public:
    Triangle(){};
    Triangle(int s1, int s2, int s3);
    int getIdSommet(int) const;
    void setIdSommet(int, int);
    friend std::ostream& operator<< (std::ostream&, const Triangle&);

protected:
    int v[3] = {-1, -1, -1};
};

#endif //GEOLIB_TRIANGLE_H
