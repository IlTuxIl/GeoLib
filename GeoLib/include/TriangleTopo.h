//
// Created by julien on 07/09/17.
//

#ifndef GEOLIB_TRIANGLETOPO_H
#define GEOLIB_TRIANGLETOPO_H

#include "Triangle.h"
#include "Sommet.h"

class TriangleTopo : public Triangle{
public:
    TriangleTopo(){id = -1;};
    TriangleTopo(int _s1, int _s2, int s3, int _id);

    int getIndexInTriangle(int idSommet);
    void setId(int _id){id = _id;};
    int getId()const{return this->id;}
    void setNeighbor(int idtri, int id);
    int getNeighbor(int id) const;
    vector3 getVoronoi(){return voronoi;};
    vector3 computeVoronoi(vector3 a, vector3 b, vector3 c);
    int estExterieur();
    friend std::ostream& operator<< (std::ostream&, const TriangleTopo&);

private:
    int id = -1;
    int t[3] = {-1, -1, -1};
    vector3 voronoi;
};


#endif //GEOLIB_TRIANGLETOPO_H
