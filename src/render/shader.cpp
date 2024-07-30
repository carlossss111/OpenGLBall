#include "render/shader.h"

std::string readShader(std::string filename) {
    try {
        std::ifstream sFile;
        sFile.open(std::string(SHADER_DIR) + filename);
        if (!sFile) {
            throw std::iostream::failure(std::string(SHADER_DIR) + filename + " does not exist.");
        }
        std::stringstream sStream;
        sStream << sFile.rdbuf();
        sFile.close();
        return sStream.str();
    }
    catch (std::ifstream::failure& e) {
        fprintf(stderr, "Failed to read shader file: %s\n", e.what());
        exit(1);
    }
}

Shader::Shader(std::string vsFilename, std::string fsFilename) {
    char infoLog[512];
    int success;
    
    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertText = readShader(vsFilename);
    const char* vertSrc = vertText.c_str();
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Failed to compile Vertex Shader\n%s", infoLog);
        exit(1);
    }

    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragText = readShader(fsFilename);
    const char* fragSrc = fragText.c_str();
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Failed to compile Fragment Shader\n%s", infoLog);
        exit(1);
    }

    // Create shader program
    mProgramID = glCreateProgram();
    glAttachShader(mProgramID, vertexShader);
    glAttachShader(mProgramID, fragmentShader);
    glLinkProgram(mProgramID);
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(mProgramID, 512, NULL, infoLog);
        fprintf(stderr, "Failed to link Shader\n%s", infoLog);
        exit(1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

Shader::~Shader() {
    glDeleteProgram(mProgramID);
}

void Shader::use() const {
    glUseProgram(mProgramID);
}

GLuint Shader::getHandle() const {
    return mProgramID;
}

void Shader::setInt(const std::string& name, GLint value) {
    glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, GLfloat value) {
    glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::setBool(const std::string& name, GLboolean value) {
    glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::setMat2(const std::string& name, glm::mat2 value) {
    glUniformMatrix2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, glm::mat3 value) {
    glUniformMatrix3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec2(const std::string& name, glm::vec2 value) {
    glUniform2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setLightUniforms(const Light& light) {
    setVec3("light.position", light.pos);
    setVec3("light.direction", light.direction);
    setVec3("light.ambient", light.ambient);
    setVec3("light.diffuse", light.diffuse);
    setVec3("light.specular", light.specular);
}
