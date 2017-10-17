//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_TRIANGLE_H
#define GEOALGO_TRIANGLE_H

#include "Sommet.h"
#include "Vector.h"

namespace GeoLib {

    class Triangle {
    public:
        Triangle() = default;
        Triangle(int x, int y, int z) : idSommet(x, y, z){};
        int getIdSommet(int id) const;
        void setIdSommet (int sommet, int indexDansTriangle);
        int getIndexInTriangle(int _idSommet);

    protected:
        index idSommet;
    };

    class TriangleTopo : public Triangle{
    public:
        TriangleTopo();
        TriangleTopo(int x, int y, int z, int _id);
        bool estExterieur();
        void setIdSommet(int sommet, int indexDansTriangle);

        void setId(int _id){id = _id;};
        int getId()const{return this->id;}

        void setNeighbor(int idtri, int id);
        int getNeighbor(int id) const ;

        vector3 getVoronoi(){return voronoi;};
        vector3 computeVoronoi(vector3 a, vector3 b, vector3 c);

    private:
        bool updateVoronoi = true;
        vector3 voronoi;
        index idVoisin;
        int id = -1;
    };






}

#endif //GEOALGO_TRIANGLE_H
