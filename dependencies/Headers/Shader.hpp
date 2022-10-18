#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>

class Shader 
{
public:
    unsigned int shaderProgramID;
    Shader(std::string_view vertexShaderPath, std::string_view fragmentShaderPath);
    void use();

    void setUniform(std::string_view uniformName, int value);
    void setUniform(std::string_view uniformName, bool value);
    void setUniform(std::string_view uniformName, float value);
    void setUniform(std::string_view uniformName, int value1, int value2, int value3, int value4);
    void setUniform(std::string_view uniformName, const glm::vec2& value);
    void setUniform(std::string_view uniformName, float value1, float value2);
    void setUniform(std::string_view uniformName, const glm::vec3& value);
    void setUniform(std::string_view uniformName, float value1, float value2, float value3);
    void setUniform(std::string_view uniformName, const glm::vec4& value);
    void setUniform(std::string_view uniformName, float value1, float value2, float value3, float value4);
    void setUniform(std::string_view uniformName, const glm::mat2& value);
    void setUniform(std::string_view uniformName, const glm::mat3& value);
    void setUniform(std::string_view uniformName, const glm::mat4& value);
    ~Shader();
private:
    enum statusCheck {VertexShader, FragmentShader, Linker};
    void CheckStatus(unsigned int shader, statusCheck t);
};


#endif