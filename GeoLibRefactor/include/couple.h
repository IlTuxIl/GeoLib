//
// Created by julien on 23/10/17.
//

#ifndef GEOALGO_COUPLE_H
#define GEOALGO_COUPLE_H

struct couple{
    int p1,p2;
    bool operator< (const couple& c2) const
    {return p1 < c2.p1 || p2 < c2.p2;}
};

#endif //GEOALGO_COUPLE_H
