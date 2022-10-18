#include "Shader.hpp"

Shader::Shader(std::string_view vertexShaderPath, std::string_view fragmentShaderPath): shaderProgramID{glCreateProgram()}
{
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    std::string temp_fragmentShaderCode;
    std::string temp_vertexShaderCode;

    vertexShaderFile.exceptions   (std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexShaderFile.open(vertexShaderPath.data());
        std::ostringstream vShaderStream;
        vShaderStream << vertexShaderFile.rdbuf();
        temp_vertexShaderCode = vShaderStream.str();
        
        fragmentShaderFile.open(fragmentShaderPath.data());
        std::ostringstream fShaderStream;
        fShaderStream << fragmentShaderFile.rdbuf();
        temp_fragmentShaderCode = fShaderStream.str();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << "An error occured while reading from the shader program" << e.what() << '\n';
    }
    const char* vertexShaderCode{temp_vertexShaderCode.c_str()};
    unsigned int vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);
    CheckStatus(vertexShader, VertexShader);
    
    const char* fragmentShaderCode{temp_fragmentShaderCode.c_str()};
    unsigned int fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};

    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);
    CheckStatus(fragmentShader, FragmentShader);

    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);
    CheckStatus(shaderProgramID, Linker);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::CheckStatus(GLuint shader, statusCheck t)
{
    int success;
    char infolog[1024];
    switch (t)
    {
        case VertexShader:
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infolog);
                std::cerr << "PROGRAM VERTEX COMPILE ERROR: " << infolog << "\n";
            }
            break;
        case FragmentShader:
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infolog);
                std::cerr << "PROGRAM FRAGMENT COMPILE ERROR: " << infolog << "\n";
            }
            break;
        case Linker:
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infolog);
                std::cerr << "PROGRAM LINK ERROR: "<< infolog << "\n";
            }
            break;
        default:
            std::cout << "Never Meant to be here \n";
    }
}

void Shader::use()
{
    glUseProgram(shaderProgramID);
}

Shader::~Shader()
{
    glDeleteProgram(shaderProgramID);
}
#pragma region setUniformOverloads

void Shader::setUniform(std::string_view uniformName, int value)
{
    glUniform1i( glGetUniformLocation(shaderProgramID, uniformName.data()) , value);
}


void Shader::setUniform(std::string_view uniformName, bool value)
{
    glUniform1i( glGetUniformLocation(shaderProgramID, uniformName.data()) , static_cast<int>(value) );
}


void Shader::setUniform(std::string_view uniformName, float value)
{
    glUniform1f( glGetUniformLocation(shaderProgramID, uniformName.data() ), value);
}


void Shader::setUniform(std::string_view uniformName, int value1, int value2, int value3, int value4)
{
    glUniform4i( glGetUniformLocation(shaderProgramID, uniformName.data()) , value1, value2, value3, value4);
}


void Shader::setUniform(std::string_view uniformName, const glm::vec2& value)
{
    glUniform2fv(glGetUniformLocation(shaderProgramID, uniformName.data()), 1, &value[0]);
}


void Shader::setUniform(std::string_view uniformName, float value1, float value2)
{
    glUniform2f(glGetUniformLocation(shaderProgramID, uniformName.data()), value1, value2);
}


void Shader::setUniform(std::string_view uniformName, const glm::vec3& value)
{
    glUniform3fv(glGetUniformLocation(shaderProgramID, uniformName.data()), 1, &value[0]);
}


void Shader::setUniform(std::string_view uniformName, float value1, float value2, float value3)
{
    glUniform3f(glGetUniformLocation(shaderProgramID, uniformName.data()), value1, value2, value3);
}


void Shader::setUniform(std::string_view uniformName, const glm::vec4& value)
{
    glUniform4fv(glGetUniformLocation(shaderProgramID, uniformName.data()), 1, &value[0]);
}


void Shader::setUniform(std::string_view uniformName, float value1, float value2, float value3, float value4)
{
    glUniform4f(glGetUniformLocation(shaderProgramID, uniformName.data()), value1, value2, value3, value4);
}


void Shader::setUniform(std::string_view uniformName, const glm::mat2& value)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, uniformName.data()), 1, GL_FALSE, &value[0][0]);
}


void Shader::setUniform(std::string_view uniformName, const glm::mat3& value)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, uniformName.data()), 1, GL_FALSE, &value[0][0]);
}


void Shader::setUniform(std::string_view uniformName, const glm::mat4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, uniformName.data()), 1, GL_FALSE, &value[0][0]);
}

#pragma endregion