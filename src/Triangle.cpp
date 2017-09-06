//
// Created by julien on 05/09/17.
//

#include "Triangle.h"

Triangle::Triangle() {

}

Triangle::Triangle(const vector3 &vec1, const vector3 &vec2, const vector3 &vec3) {
    v1 = vec1;
    v2 = vec2;
    v3 = vec3;
}

Triangle::Triangle(const Triangle &tri) {
    v1 = tri.v1;
    v2 = tri.v2;
    v3 = tri.v3;
}

Triangle Triangle::operator=(const Triangle &tri) {
    return Triangle(tri);
}
