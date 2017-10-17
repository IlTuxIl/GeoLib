//
// Created by julien on 17/10/17.
//

#include "../include/Triangle.h"
namespace GeoLib {

    TriangleTopo::TriangleTopo() {
        idSommet.setX(-1);
        idSommet.setY(-1);
        idSommet.setZ(-1);
        id = -1;
    }

    TriangleTopo::TriangleTopo(int x, int y, int z, int _id) {
        idSommet.setX(x);
        idSommet.setY(y);
        idSommet.setZ(z);
        id = _id;
    }

    int Triangle::getIdSommet(int id) const {
        if(id == -1)
            return idSommet[2];
        return idSommet[id%3];
    }

    void Triangle::setIdSommet(int sommet, int indexDansTriangle) {
        int id = indexDansTriangle % 3;
//        idSommet[indexDansTriangle%3] = sommet;

        switch (id){
            case 0 :
                idSommet.setX(sommet);
                break;
            case 1 :
                idSommet.setY(sommet);
                break;
            case 2 :
                idSommet.setZ(sommet);
                break;
        }
    }

    int Triangle::getIndexInTriangle(int _idSommet) {
        for(int i = 0; i < 3; i++){
            if(idSommet[i] == _idSommet)
                return i;
        }
        return -1;
    }

    void TriangleTopo::setIdSommet(int sommet, int indexDansTriangle) {
        int id = indexDansTriangle % 3;
//        idSommet.[indexDansTriangle%3] = sommet;
        switch (id){
            case 0 :
                idSommet.setX(sommet);
                break;
            case 1 :
                idSommet.setY(sommet);
                break;
            case 2 :
                idSommet.setZ(sommet);
                break;
        }
        updateVoronoi = true;
    }

    bool TriangleTopo::estExterieur() {
        for(int i = 0; i < 3; i++){
            if(idVoisin[i] == 0)
                return true;
        }
        return false;
    }

    void TriangleTopo::setNeighbor(int idtri, int id) {
        int i = id % 3;
//        idVoisin[id%3] = idtri;

        switch (id){
            case 0 :
                idVoisin.setX(idtri);
                break;
            case 1 :
                idVoisin.setY(idtri);
                break;
            case 2 :
                idVoisin.setZ(idtri);
                break;
        }
        updateVoronoi = true;
    }

    int TriangleTopo::getNeighbor(int id) const {
        return idVoisin[id%3];
    }

    vector3 TriangleTopo::computeVoronoi(vector3 a, vector3 b, vector3 c) {
        if(updateVoronoi) {
            double A = ((b - c).length2() * (a - b).dot(a - c)) / (2 * (a - b).cross(b - c).length2());
            double B = ((a - c).length2() * (b - a).dot(b - c)) / (2 * (a - b).cross(b - c).length2());
            double C = ((a - b).length2() * (c - a).dot(c - b)) / (2 * (a - b).cross(b - c).length2());
            voronoi = a * A + b * B + c * C;
            updateVoronoi = false;
        }
        return voronoi;
    }

}