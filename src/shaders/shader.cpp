#include "shader.h"

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

void Shader::use() {
    glUseProgram(mProgramID);
}
