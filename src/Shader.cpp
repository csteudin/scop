#include "../inc/scop.hpp"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);
    GLuint vertexShader = compile(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compile(fragmentCode, GL_FRAGMENT_SHADER);
    this->_id = glCreateProgram();
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, fragmentShader);
    glLinkProgram(_id);
    checkCompileErrors(_id, "PROGRAM");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    LOG("! Shader Created");
}

Shader::~Shader()
{
    glDeleteProgram(_id);
    LOG("! Shader Destroyed");
}

//public
void Shader::use() const
{
    glUseProgram(_id);    
}

GLuint Shader::getID() const
{
    return _id;
}

void Shader::setMat4(const std::string &name, const Mat4 &mat) const
{
    GLint location = glGetUniformLocation(_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_TRUE, &mat.m[0][0]);
}


//private
std::string Shader::readFile(const std::string &path)
{
    std::ifstream file(path);
    std::stringstream buff;

    if (!file.is_open())
        std::cerr << "Error: cannot open " << path << std::endl;
    buff << file.rdbuf();

    return (buff.str());
}

GLuint Shader::compile(const std::string &source, GLenum type)
{
    if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER )
        std::cerr << "Error: compiling wrong type" << std::endl;
    
    const char *src = source.c_str();
    GLuint id = glCreateShader(type);
    
    glShaderSource(id, 1, &src, nullptr);
    LOG("! Shaders Compiling");
    glCompileShader(id);
    
    if (type == GL_VERTEX_SHADER)
        checkCompileErrors(id, "VERTEX");
    else if (type == GL_FRAGMENT_SHADER)
        checkCompileErrors(id, "FRAGMENT");
    
    return (id);
}

void Shader::checkCompileErrors(GLuint shader, const std::string&type)
{
    int success;

    if (type == "VERTEX" || type == "FRAGMENT")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infolog[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, infolog);
            std::cerr << "Error: " << infolog << std::endl;
        }
    }
    else if (type == "PROGRAM")
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infolog[1024];
            glGetProgramInfoLog(shader, 1024, nullptr, infolog);
            std::cerr << "Error: " << infolog << std::endl;
        }
    }

}
