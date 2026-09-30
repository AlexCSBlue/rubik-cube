#pragma once

#include <glm/glm.hpp>
#include <string>

namespace rubik::render {
    class Shader {
      public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);

        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator = (const Shader&) = delete;
        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        void bind() const noexcept;

        static void unbind() noexcept;

        void setMat4(const std::string& name, const glm::mat4& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setFloat(const std::string& name, float value) const;
        void setInt(const std::string& name, int value) const;

        /// @return Underlying OpenGL program ID
        [[nodiscard]] unsigned int id() const noexcept { return programId_; }

    private:
        static std::string readFile(const std::string& path);
        static unsigned int compile(unsigned int type, const std::string& source);
        static unsigned int link(unsigned int vertexShader, unsigned int fragmentShader);
        int getUniformLocation(const std::string& name) const;
        unsigned int programId_{0};
    };
}
