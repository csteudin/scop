#include "../inc/scop.hpp"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);
    GLuint vertexShader = compile(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compile(fragmentCode, GL_FRAGMENT_SHADER);
    checkCompileErrors(vertexShader, "VERTEX");
    checkCompileErrors(fragmentShader, "FRAGMENT");
    this->_id = glCreateProgram();
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, fragmentShader);
    glLinkProgram(_id);
    checkCompileErrors(_id, "PROGRAM");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    std::cout << "! Shader Created" << std::endl;
}

Shader::~Shader()
{
    glDeleteProgram(_id);
    std::cout << "! Shader Destroyed" << std::endl;
}

//public
void Shader::use() const
{
    glUseProgram(_id);    
    std::cout << "! Shader Used" << std::endl;
}

GLuint Shader::getID() const
{
    return _id;
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
    std::cout << "! Shaders Compiling" << std::endl;
    glCompileShader(id);
    
    if (type == GL_VERTEX_SHADER)
        checkCompileErrors(id, "VERTEX");
    else if (type == GL_FRAGMENT_SHADER)
        checkCompileErrors(id, "FRAGEMENT");
    
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
