//
// Created by Julien on 05/10/2017.
//

#include "Render.h"
#include "../gKit/src/program.h"
#include "../gKit/src/uniforms.h"

GLuint Render::initBuffer(bool voronoi){

    int index = 0;
    if(voronoi)
        index = 1;

    glGenVertexArrays(1, &vao[index]);
    glBindVertexArray(vao[index]);

    size_t sizeBuffer;

    if(!voronoi)
        sizeBuffer = mesh->getNbVertex() * sizeof(float) * 3;// + mesh->getVoronoi().size() * sizeof(float) * 3;
    else {
        cptVoronoi = mesh->getVoronoi().size();
        sizeBuffer = cptVoronoi * sizeof(float) * 3;
    }
    glGenBuffers(1, &buffer[index]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[index]);
    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, nullptr, GL_DYNAMIC_DRAW);

    size_t offset = 0;
    if(!voronoi)
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeBuffer, &mesh->getVertex()[0]);
    else
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeBuffer, &mesh->getVoronoi()[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *) offset);
    glEnableVertexAttribArray(0);

    if(!voronoi) {
        sizeBuffer = mesh->getNbFaces() * sizeof(unsigned int) * 3;

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBuffer, &mesh->getIndex().front(), GL_DYNAMIC_DRAW);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao[index];
}

GLuint Render::updateBuffer(bool voronoi){

    int index = 0;
    if(voronoi)
        index = 1;

    glBindVertexArray(vao[index]);
    size_t sizeBuffer;


    if(!voronoi)
        sizeBuffer = mesh->getNbVertex() * sizeof(float) * 3;
    else{
        cptVoronoi = mesh->getVoronoi().size();
        sizeBuffer = cptVoronoi * sizeof(float) * 3;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer[index]);
    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, nullptr, GL_DYNAMIC_DRAW);

    size_t offset = 0;
    if(!voronoi)
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeBuffer, &mesh->getVertex()[0]);
    else
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeBuffer, &mesh->getVoronoi()[0]);

    if(!voronoi) {
        sizeBuffer = mesh->getNbFaces() * sizeof(unsigned int) * 3;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBuffer, &mesh->getIndex().front(), GL_DYNAMIC_DRAW);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao[0];
}

Render::Render(char *shaderName, Triangulation* m) {
    program[0] = read_program(shaderName);
    program[1] = read_program("data/shaderVoronoi.glsl");
    mesh = m;
    vao[0] = initBuffer(false);
    vao[1] = initBuffer(true);
    glUseProgram(0);
}

void Render::draw(Orbiter cam, bool voronoi, bool update) {

    Transform model = Identity();
    Transform view = cam.view();
    Transform projection = cam.projection(1024, 640, 45);
    Transform mv= view * model;
    Transform mvp= projection * mv;

    if(update) {
        updateBuffer(false);
        updateBuffer(true);
    }

    glBindVertexArray(vao[0]);
    glUseProgram(program[0]);
    program_uniform(program[0], "mvpMatrix", mvp);

    glDrawElements(GL_TRIANGLES, mesh->getIndex().size(), GL_UNSIGNED_INT, 0);
    if(voronoi){
        glBindVertexArray(vao[1]);
        glUseProgram(program[1]);
        program_uniform(program[1], "mvpMatrix", mvp);
        glDrawArrays(GL_LINES, 0, cptVoronoi/3);
    }
}
