//
// Created by julien on 17/10/17.
//

#include <cmath>
#include "Triangle.h"
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
        if(id == -1)
            return idVoisin[2];
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

    double TriangleTopo::computeAspectRation(vector3 a, vector3 b, vector3 c) {
        double A = (b-a).length();
        double B = (c-a).length();
        double C = (c-b).length();

        double s = (A+B+C)/2;

        return (A * B * C) / (8 * (s - A) * (s - B) * (s - C));
    }

    double TriangleTopo::minAngle(vector3 a, vector3 b, vector3 c) const{
        double A = (b-c).length();
        double B = (c-a).length();
        double C = (b-a).length();

        double angleA = acos(((B*B) + (C*C)- (A*A)) / (2 * B * C)) * (180/3.14);
        double angleB = acos(((C*C) + (A*A) - (B*B)) / (2 * C * A)) * (180/3.14);
        double angleC = 180 - angleA - angleB;

        //std::cout << angleA << " " << angleB << " " << angleC << " " << angleA+angleB+angleC << std::endl;

        return std::min(std::min(angleA, angleB), angleC);
    }

    double TriangleTopo::maxAngle(vector3 a, vector3 b, vector3 c) const{
        double A = (b-c).length();
        double B = (c-a).length();
        double C = (b-a).length();

        double angleA = acos(((B*B) + (C*C)- (A*A)) / (2 * B * C)) * (180/3.14);
        double angleB = acos(((C*C) + (A*A) - (B*B)) / (2 * C * A)) * (180/3.14);
        double angleC = 180 - angleA - angleB;

        //std::cout << angleA << " " << angleB << " " << angleC << " " << angleA+angleB+angleC << std::endl;

        return std::max(std::max(angleA, angleB), angleC);
    }
}