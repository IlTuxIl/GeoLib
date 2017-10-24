//
// Created by julien on 17/10/17.
//

#ifndef GEOALGO_SCATTERPLOT_H
#define GEOALGO_SCATTERPLOT_H


#include "Vector.h"
#include "Sommet.h"
#include <vector>

namespace GeoLib {
    class ScatterPlot {
      public :
        void addPlot(vector3 p) { plot.push_back(p); }
        vector3& getPlot(int i) { return plot[i]; }
        int getSize() const {return plot.size();}
        vector3&operator[](int i){return plot[i];}
        void reserve(int a){plot.reserve(a);};
        std::vector<vector3> getVector(){ return plot;};
        const std::vector<vector3> getVector() const { return plot;};

    private:
        std::vector<vector3> plot;
    };

    class ScatterVertex {
      public :
        void addPlot(Sommet p) { plot.push_back(p); }
        void reserve(int a){plot.reserve(a);};

        Sommet& getPlot(int i) { return plot[i]; }
        Sommet&operator[](int i){return plot[i];}
        const Sommet& operator[](int i)const {return plot[i];}

        std::vector<Sommet> getVector(){ return plot;};

        int getSize() const {return plot.size();}

      private:
        std::vector<Sommet> plot;
    };
}
#endif //GEOALGO_SCATTERPLOT_H
