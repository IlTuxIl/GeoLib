//
// Created by julien on 15/10/17.
//

#include "RenderMaillage2D.h"
#include "../gKit/src/program.h"
#include "../gKit/src/uniforms.h"

RenderMaillage2D::RenderMaillage2D(char *filename, Maillage2D *maillage) {
    mesh = maillage;
    program = read_program(filename);
    vao = initBuffer();
}

GLuint RenderMaillage2D::initBuffer(){
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    size_t sizeBuffer;
    nbIndex = mesh->getArretes().size();
    sizeBuffer = nbIndex * sizeof(float);// + mesh->getVoronoi().size() * sizeof(float) * 3;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, nullptr, GL_DYNAMIC_DRAW);

    size_t offset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeBuffer, &mesh->getArretes()[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *) offset);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vao;
}

GLuint RenderMaillage2D::updateBuffer(){
    glBindVertexArray(vao);

    size_t sizeBuffer;
    nbIndex = mesh->getArretes().size();
    sizeBuffer = nbIndex * sizeof(float);// + mesh->getVoronoi().size() * sizeof(float) * 3;

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, nullptr, GL_DYNAMIC_DRAW);

    size_t offset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeBuffer, &mesh->getArretes()[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *) offset);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vao;
}

void RenderMaillage2D::draw(Orbiter cam, bool update) {
    Transform model = Identity();
    Transform view = cam.view();
    Transform projection = cam.projection(1024, 640, 45);
    Transform mv= view * model;
    Transform mvp= projection * mv;

    if(update)
        vao = updateBuffer();

    glBindVertexArray(vao);
    glUseProgram(program);
    program_uniform(program, "mvpMatrix", mvp);

    glDrawArrays(GL_LINES, 0, nbIndex/2);
}