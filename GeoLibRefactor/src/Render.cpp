//
// Created by julien on 17/10/17.
//

#include "Render.h"
#include "../../GeoLib/gKit/src/program.h"
#include "../../GeoLib/gKit/src/mat.h"
#include "../../GeoLib/gKit/src/uniforms.h"

namespace GeoLib {

    Render1D::Render1D(Maillage1D *_sp){
        mesh = _sp;
        program = read_program("data/shader1.glsl");
        initBuffer();
        glUseProgram(0);
    }

    Render2D::Render2D(Maillage2D *_sp){
        mesh = _sp;
        program = read_program("data/shader1.glsl");
        initBuffer();
        glUseProgram(0);
    }

    void Render1D::draw(Orbiter cam, bool update) {
        Transform model = Identity();
        Transform view = cam.view();
        Transform projection = cam.projection(1024, 640, 45);
        Transform mv= view * model;
        Transform mvp= projection * mv;

        if(update) {
            updateBuffer();
        }

        glBindVertexArray(vao);
        glUseProgram(program);
        program_uniform(program, "mvpMatrix", mvp);
        program_uniform(program, "color", c);

//        std::cout << mesh->getIndiceBufferSize()/(sizeof(int) * 3) << std::endl;

        glDrawElements(GL_LINES, mesh->getIndiceBufferSize()/(sizeof(int)), GL_UNSIGNED_INT, 0);
    }

    void Render2D::draw(Orbiter cam, bool update) {
        Transform model = Identity();
        Transform view = cam.view();
        Transform projection = cam.projection(1024, 640, 45);
        Transform mv= view * model;
        Transform mvp= projection * mv;

        if(update) {
            updateBuffer();
        }

        glBindVertexArray(vao);
        glUseProgram(program);
        program_uniform(program, "mvpMatrix", mvp);
        program_uniform(program, "color", c);

        glDrawElements(GL_TRIANGLES, mesh->getIndiceBufferSize()/(sizeof(int)), GL_UNSIGNED_INT, 0);
    }

    template<class T>
    void Render<T>::initBuffer() {

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        size_t sizeBuffer = static_cast<T*>(mesh)->getVertexBufferSize();
        std::cout << sizeBuffer << std::endl;

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeBuffer, nullptr, GL_DYNAMIC_DRAW);

        size_t offset = 0;
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeBuffer, static_cast<T*>(mesh)->getVertexBuffer());

        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (const void *) offset);
        glEnableVertexAttribArray(0);

        sizeBuffer = static_cast<T*>(mesh)->getIndiceBufferSize();
        std::cout << sizeBuffer << std::endl;

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBuffer, static_cast<T*>(mesh)->getIndiceBuffer(), GL_DYNAMIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    template<class T>
    void Render<T>::updateBuffer() {

        glBindVertexArray(vao);
        size_t sizeBuffer = static_cast<T*>(mesh)->getVertexBufferSize();
        std::cout << sizeBuffer << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeBuffer, nullptr, GL_DYNAMIC_DRAW);

        size_t offset = 0;
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeBuffer, static_cast<T*>(mesh)->getVertexBuffer());

        sizeBuffer = static_cast<T*>(mesh)->getIndiceBufferSize();
        std::cout << sizeBuffer << std::endl;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBuffer, static_cast<T*>(mesh)->getIndiceBuffer(), GL_DYNAMIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
}