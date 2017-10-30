//
// Created by julien on 23/10/17.
//

#ifndef GEOALGO_MAILLAGE_H
#define GEOALGO_MAILLAGE_H

#include "ScatterPlot.h"
#include "couple.h"
#include "Triplet.h"

namespace GeoLib {
    class Maillage{

      public:
        void setNbIndiceFace(int v);
        void setVertexBuffer(ScatterPlot& p);
        void setIndiceBuffer(const std::vector<unsigned int>& p);

        int getNbIndiceFace() const;
        std::vector<float> getVertex();
        std::vector<unsigned int> getIndice();
        size_t getVertexBufferSize() const;
        size_t getIndiceBufferSize() const;

    protected:
        ScatterPlot vertex;
        std::vector<unsigned int> indice;
        int nbIndicesFace;
    };
}

#endif //GEOALGO_MAILLAGE_H
