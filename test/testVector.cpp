////
//// Created by Julien on 04/09/2017.
////

#include <cassert>
#include "../src/Vector.h"
#include <iostream>

void constructor(){
    vector2 v11;
    assert(v11.x == 0);
    assert(v11.y == 0);

    vector2 v12(1);
    assert(v12.x == 1);
    assert(v12.y == 0);

    vector2 v13(4.9f,7);
    assert(v13.x == 4.9f);
    assert(v13.y == 7);

    vector2 v14 = v13;
    assert(v14.x == 4.9f);
    assert(v14.y == 7);

    assert (v13 == v14);

    vector3 v21;
    assert(v21.x == 0);
    assert(v21.y == 0);
    assert(v21.z == 0);

    vector3 v22(2);
    assert(v22.x == 2);
    assert(v22.y == 0);
    assert(v22.z == 0);

    vector3 v23(4,99);
    assert(v23.x == 4);
    assert(v23.y == 99);
    assert(v23.z == 0);

    vector3 v24(7,2.7f,9);
    assert(v24.x == 7);
    assert(v24.y == 2.7f);
    assert(v24.z == 9);

    vector3 v25 = v24;
    assert(v25.x == 7);
    assert(v25.y == 2.7f);
    assert(v25.z == 9);

    assert (v25 == v24);

    vector3 v26 = v13;
    assert(v26.x == 4.9f);
    assert(v26.y == 7);
    assert(v26.z == 0);

}

void op(){

    vector2 v11(7,12);
    vector2 v12(2,9);
    vector3 v13 = v11 + v12;
    vector3 v14 = v11 - v12;
    vector3 v15 = v11 * 2;
    vector3 v16 = v11 / 2;
    assert(v13.x == 9);
    assert(v13.y == 21);
    assert(v14.x == 5);
    assert(v14.y == 3);
    assert(v15.x == 14);
    assert(v15.y == 24);
    assert(v16.x == 3.5f);
    assert(v16.y == 6);

    vector2 v17(1,1);
    std::cout << v17.normalize() << std::endl;
    //assert(v17.normalize() == vector2(0.5f,0.5f));

}

int main(){
    constructor();
    op();
    return 0;
}