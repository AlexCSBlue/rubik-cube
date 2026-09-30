#include "rubik/render/Shader.hpp"
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace rubik::render {
    namespace {
        std::unordered_map<unsigned int, std::unordered_map<std::string, int>> &uniformCache(){
            static std::unordered_map<unsigned int, std::unordered_map<std::string, int>> cache;
            return cache;
        }
    }

    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath){
        const std::string vertexSource = readFile(vertexPath);
        const std::string fragmentSource = readFile(fragmentPath);

        const unsigned int vertexShader = compile(GL_VERTEX_SHADER, vertexSource);
        const unsigned int fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentSource);

        try {
            programId_ = link(vertexShader, fragmentShader);
        } catch (...){
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            throw;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        std::cout << "[SHADER] Compiled and linked: " << vertexPath << " + " << fragmentPath << " (id=" << programId_ << ")\n";
    }

    Shader::~Shader(){
        if(programId_ != 0){
            glDeleteProgram(programId_);
            uniformCache().erase(programId_);
            programId_ = 0;
        }
    }

    Shader::Shader(Shader&& other) noexcept : programId_(other.programId_){
        other.programId_ = 0;
    }

    Shader& Shader::operator=(Shader&& other) noexcept {
        if(this != &other) {
            if(programId_ != 0){
                glDeleteProgram(programId_);
                uniformCache().erase(programId_);
            }

            programId_ = other.programId_;
            other.programId_ = 0;
        }
        return *this;
    }

    void Shader::bind() const noexcept {
        glUseProgram(programId_);
    }

    void Shader::unbind() noexcept {
        glUseProgram(0);
    }

    void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(getUniformLocation(name), 1, &value[0]);
    }

    void Shader::setFloat(const std::string& name, float value) const{
        glUniform1f(getUniformLocation(name), value);
    }

    void Shader::setInt(const std::string &name, int value) const {
        glUniform1i(getUniformLocation(name), value);
    }

    std::string Shader::readFile(const std::string& path){
        std::ifstream file(path, std::ios::in | std::ios::binary);
        if(!file.is_open()){
            throw std::runtime_error("Shader: failed to open file: " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    unsigned int Shader::compile(unsigned int type, const std::string &source) {
        const unsigned int shader = glCreateShader(type);

        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            char infoLog[1024];
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

            const char* typeName = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";

            std::cerr << "[Shader] " << typeName << "  compilation failed:\n" << infoLog << '\n';

            glDeleteShader(shader);
            throw std::runtime_error(std::string("Shader compilation failed(") + typeName + ")");
        }

        return shader;
    }

    unsigned int Shader::link(unsigned int vertexShader, unsigned int fragmentShader) {
        const unsigned int program = glCreateProgram();

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        int success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success) {
            char infoLog[1024];
            glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);

            std::cerr << "[Shader] Program linking failed:\n" << infoLog << '\n';

            glDeleteProgram(program);
            throw std::runtime_error("Shader program linking failed");
        }

        return program;
    }

    int Shader::getUniformLocation(const std::string &name) const {
        auto& cache = uniformCache()[programId_];

        const auto it = cache.find(name);
        if(it != cache.end()){
            return it -> second;
        }

        const int location = glGetUniformLocation(programId_, name.c_str());
        if(location == -1){
            std::cerr << "[Shader] Warning: uniform '" << name << "' not found in program " << programId_ << '\n';
        }

        cache.emplace(name, location);
        return location;
    }
}
