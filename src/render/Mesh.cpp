#include "rubik/render/Mesh.hpp"
#include <glad/glad.h>
#include <cstddef>
#include <iostream>
#include <stddef.h>
#include <utility>

namespace rubik::render {

    Mesh::Mesh() {

        glGenVertexArrays(1, &vao_);

        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);

        glBindVertexArray(vao_);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

        configureAttributes();

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Mesh::~Mesh(){
        if(ebo_ != 0){
            glDeleteBuffers(1, &ebo_);
            ebo_=0;
        }
        if(vbo_ != 0) {
            glDeleteBuffers(1, &vbo_);
            vbo_ = 0;
        }
        if(vao_ != 0){
            glDeleteVertexArrays(1, &vao_);
            vao_ = 0;
        }
    }

    Mesh::Mesh(Mesh&& other) noexcept
        : vao_(other.vao_)
        , vbo_(other.vbo_)
        , ebo_(other.ebo_)
        , vertexCount_(other.vertexCount_)
        , indexCount_(other.indexCount_) {
            other.vao_ = 0;
            other.vbo_ = 0;
            other.ebo_ = 0;
            other.vertexCount_ = 0;
            other.indexCount_ = 0;
    }

    Mesh& Mesh::operator=(Mesh &&other) noexcept {
        if (this != &other) {

            if(ebo_ != 0) glDeleteBuffers(1, &ebo_);
            if(vbo_ != 0) glDeleteBuffers(1, &vbo_);
            if(vao_ != 0) glad_glDeleteVertexArrays(1, &vao_);

            vao_ = other.vao_;
            vbo_ = other.vbo_;
            ebo_ = other.ebo_;
            vertexCount_ = other.vertexCount_;
            indexCount_ = other.indexCount_;
        }
        return *this;
    }

    void Mesh::setVertices(const std::vector<Vertex>& vertices) {
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
            vertices.data(),
            GL_STATIC_DRAW
        );

        vertexCount_ = vertices.size();

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Mesh::setIndices(const std::vector<std::uint32_t>& indices) {
        glBindVertexArray(vao_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(indices.size() * sizeof(std::uint32_t)),
            indices.data(),
            GL_STATIC_DRAW
        );

        indexCount_ = indices.size();

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Mesh::bind() const noexcept {
        glBindVertexArray(vao_);
    }

    void Mesh::unbind() noexcept {
        glBindVertexArray(0);
    }

    void Mesh::draw() const noexcept {
        if (indexCount_ == 0 && vertexCount_ == 0) {
            return;
        }

        glBindVertexArray(vao_);

        if(indexCount_ > 0) {
            glDrawElements (
                GL_TRIANGLES,
                static_cast<GLsizei>(indexCount_),
                GL_UNSIGNED_INT,
                nullptr
            );
        } else {
            glDrawArrays (
                GL_TRIANGLES,
                0,
                static_cast<GLsizei>(vertexCount_)
            );
        }

        glBindVertexArray(0);
    }

    void Mesh::configureAttributes() const noexcept {

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, position))
        );

      glEnableVertexAttribArray(1);
      glVertexAttribPointer (
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, color))
      );

      glEnableVertexAttribArray(2);
      glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, normal))
      );
    }
}
