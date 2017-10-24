//
// Created by julien on 23/10/17.
//

#ifndef GEOALGO_MAILLAGE_H
#define GEOALGO_MAILLAGE_H

#include "ScatterPlot.h"
#include "couple.h"
#include "Triplet.h"

namespace GeoLib {
    template<class T>
    class Maillage{
      public:
        void setVertexBuffer(const ScatterPlot& p){vertex = p;};
        void setIndiceBuffer(const std::vector<T>& p){ indice = p;};

        vector3* getVertexBuffer() { return vertex.getVector().data();}
        T* getIndiceBuffer() { return indice.data();}
        int getVertexBufferSize() const { return  vertex.getSize() * sizeof(double) * 3;};
        virtual int getIndiceBufferSize() const = 0;
      protected:
        ScatterPlot vertex;
        std::vector<T> indice;
    };

    class Maillage1D : public Maillage<couple> {
      public:
        int getIndiceBufferSize() const { return indice.size() * sizeof(int) * 2;};
    };

    class Maillage2D : public Maillage<GeoLib::index>{
      public:
        int getIndiceBufferSize() const { return indice.size() * sizeof(int) * 3;};
    };

}

#endif //GEOALGO_MAILLAGE_H
