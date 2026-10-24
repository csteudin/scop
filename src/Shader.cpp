#include "../inc/scop.hpp"

/*  SHADER::SHADER
    - reads both shader files, compiles them and links them into one program
    - the single shaders are deleted after linking, the program keeps what it needs
*/
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

/*  SHADER::~SHADER
    - deletes the shader program
*/
Shader::~Shader()
{
    glDeleteProgram(_id);
    LOG("! Shader Destroyed");
}

//public

/*  SHADER::USE
    - makes this program the active one: everything drawn afterwards uses it
*/
void Shader::use() const
{
    glUseProgram(_id);    
}

/*  SHADER::GETID
    - returns the OpenGL id of the program
*/
GLuint Shader::getID() const
{
    return _id;
}

/*  SHADER::SETMAT4
    - sends a 4x4 matrix to the uniform "name"
    - GL_TRUE = transpose: our matrices are row-major, OpenGL expects column-major
*/
void Shader::setMat4(const std::string &name, const Mat4 &mat) const
{
    GLint location = glGetUniformLocation(_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_TRUE, &mat.m[0][0]);
}

/*  SHADER::SETINT
    - sends an int to the uniform "name" (e.g. the texture unit of the sampler)
*/
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

/*  SHADER::SETFLOAT
    - sends a float to the uniform "name" (e.g. uBlend, uLight)
*/
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

//private

/*  SHADER::READFILE
    - returns the whole content of a text file, throws if it cannot be opened
*/
std::string Shader::readFile(const std::string &path)
{
    std::ifstream file(path);
    std::stringstream buff;

    if (!file.is_open())
        throw std::runtime_error("Shader: cannot open " + path);
    buff << file.rdbuf();

    return (buff.str());
}

/*  SHADER::COMPILE
    - compiles one shader (vertex or fragment) from its source text
    - a compile error is thrown with the OpenGL log
*/
GLuint Shader::compile(const std::string &source, GLenum type)
{  
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

/*  SHADER::CHECKCOMPILEERRORS
    - checks the compile status (VERTEX / FRAGMENT) or the link status (PROGRAM)
    - on failure it throws a runtime_error that contains the OpenGL info log
*/
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
            throw std::runtime_error(type + " error:\n" + infolog);
        }
    }
    else if (type == "PROGRAM")
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infolog[1024];
            glGetProgramInfoLog(shader, 1024, nullptr, infolog);
            throw std::runtime_error(type + " error:\n" + infolog);
        }
    }

}
