#include "Shader.hpp"


//CONSTRUCTOR DEL SHADER: RECOGE DE FICHERO Y COMPILA VERTEX Y FRAGMENT
//SHADER PARA CREAR EL PROGRAMA SHADER QUE SE VA A USAR

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    //**********************************************************************************
    //**********************************************************************************
    // 1. Recuperar el vertex y fragment shader a partir de fichero
    //**********************************************************************************
    //**********************************************************************************
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);
    // los ifstream lanzan excepciones en caso de fallo
    //vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    //fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // abrir los archivos
        std::stringstream vShaderStream, fShaderStream;
        // leer el contenido del buffer de los archivos en los streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();	
        // cerrar los manejadores de los archivos
        vShaderFile.close();
        fShaderFile.close();
        // convertir stream a string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //**********************************************************************************
    //**********************************************************************************
    // 2. Compilar los shaders recogidos de fichero
    //**********************************************************************************
    //**********************************************************************************

    //======================================================================================================
    //Compilar el VERTEX SHADER recogido antes mediante un objeto shader
    //======================================================================================================

    unsigned int vertexShader; //para almacenar el shader
    int success;
    char infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);    //decimos que se trata de un vertex shader
                                                        //en el momento de crearlo
    glShaderSource(vertexShader, 1, &vShaderCode, NULL); //adjuntamos el codigo fuente del shader al objeto creado
    glCompileShader(vertexShader); //compilamos el shader

    //comprobacion de errores a la hora del compilado
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //======================================================================================================
    //Compilar el FRAGMENT SHADER recogido antes mediante un objeto shader
    //======================================================================================================

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //diferencia con el vertex shader
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    //comprobacion de errores a la hora del compilado
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //======================================================================================================
    //Creamos un SHADER PROGRAM linkeando los dos shader anteriores (tras compilarlos) y que usaremos para 
    //el renderizado
    //======================================================================================================

    ID = glCreateProgram(); // creamos el shader program
    glAttachShader(ID, vertexShader);    //|adjuntamos ambos shaders
    glAttachShader(ID, fragmentShader);  //|
    glLinkProgram(ID); //y los linkeamos
    
    //comprobacion de errores a la hora del linkeado
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);   //|eliminamos los shader objects
    glDeleteShader(fragmentShader); //|ya no los necesitamos tras linkearlos

}

//USAR EL PROGRAMA SHADER CREADO EN EL CONSTRUCTOR

void Shader::use() 
{ 
    glUseProgram(ID);
} 

//FUNCIONES PARA ESTABLECER LOS VALORES DE LOS UNIFORM

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{ 
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void Shader::setVec2(const std::string &name, float x, float y) const
{ 
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{ 
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{ 
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{ 
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) 
{ 
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}