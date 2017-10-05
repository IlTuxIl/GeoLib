//
// Created by Julien on 05/10/2017.
//

#include "Render.h"
#include "../gKit/src/program.h"
#include "../gKit/src/uniforms.h"

GLuint Render::initBuffer(){

    GLuint tmpBuffer, indexBuffer;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    size_t sizeBuffer = mesh.getNbVertex() * sizeof(double) * 3;
    glGenBuffers(1, &tmpBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tmpBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, nullptr, GL_STATIC_DRAW);

//    sizeBuffer = mesh.getNbVertex() * sizeof(double) * 3;
    size_t offset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeBuffer, mesh.getVertex());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *) offset);
    glEnableVertexAttribArray(0);


    sizeBuffer = mesh.getNbFaces() * sizeof(int) * 3;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBuffer, mesh.getIndex(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vao;
}

Render::Render(char *shaderName, Triangulation m) {
    program = read_program(shaderName);
    mesh = m;
    vao = initBuffer();
}

void Render::draw(Orbiter cam) {

    Transform model = Identity();
    Transform view = cam.view();
    Transform projection = cam.projection(1024, 640, 45);
    Transform mv= view * model;
    Transform mvp= projection * mv;

    program_uniform(program, "mvpMatrix", mvp);

    glBindVertexArray(vao);
    glUseProgram(program);

    glDrawArrays(GL_TRIANGLES, 0, mesh.getNbVertex());

}
