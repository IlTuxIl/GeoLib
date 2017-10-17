//
// Created by julien on 10/10/17.
//

#ifndef GEOALGO_ITERATEUR_H
#define GEOALGO_ITERATEUR_H

#include <vector>
#include "Triangle.h"
#include "Sommet.h"
#include "ScatterPlot.h"

namespace GeoLib {
    class faceCirculator {
    public:
        faceCirculator(std::vector<TriangleTopo> *tri, ScatterVertex* _sv, int idSommet);

        void nextFace();

        int getIndex();

        TriangleTopo *getFace();

        TriangleTopo *operator*();

        faceCirculator operator++(int);

        bool operator!=(const faceCirculator &fc);

    private:
        ScatterVertex* sv;
        std::vector<TriangleTopo> *mesh;
        bool changeTriangle;
        TriangleTopo *curTriangle;
        int curIndex;
        int startIndex;
    };

    class vertexCirculator {
    public:
        vertexCirculator(std::vector<TriangleTopo> *_mesh, int idSommet);

        void nextVertex();

        int getIndex();

        Sommet *getVertex();

        Sommet *operator*();

        vertexCirculator operator++(int);

        bool operator!=(const vertexCirculator &vc);

    private:
        bool changeTriangle;
        std::vector<TriangleTopo> *mesh;
        TriangleTopo *curTriangle;
        int curIndex;
        int startIndex;
    };

    class faceExtIterator {
    public:
        faceExtIterator(std::vector<TriangleTopo> *tri, std::vector<int> *tab, int startIndex);

        void nextFace();

        TriangleTopo *getFace();

        TriangleTopo *operator*();

        faceExtIterator operator++(int);

        faceExtIterator operator--(int);

        bool operator<(const faceExtIterator &fi);

    private:
        std::vector<TriangleTopo> *tabTriangle;
        std::vector<int> *tabIndex;
        int index = 0;
    };

}
#endif //GEOALGO_ITERATEUR_H
