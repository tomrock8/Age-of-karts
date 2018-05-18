#include "Shader.hpp"

//------------------------------------------------------------------------------------------------
// CONSTRUCTOR QUE CREA UN SHADER A PARTIR DE LOS ARCHIVOS DE LOS DISTINTOS SHADERS QUE LO FORMAN
//------------------------------------------------------------------------------------------------

Shader::Shader(const char* n, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	//Guardamos el nombre del shader
	shaderName = n;
	//Crear el program object
	program = glCreateProgram();

	//**********************************************************************************
	//**********************************************************************************
	// 1. Recuperar el vertex y fragment shader a partir de fichero
	//**********************************************************************************
	//**********************************************************************************
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile(vertexPath);
	std::ifstream fShaderFile(fragmentPath);

	try
	{
		//Abrimos los archivos
		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		//Leemos el contenido de los archivos y lo guardamos en el stream
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//Cerramos los manejadores de los archivos
		vShaderFile.close();
		fShaderFile.close();
		//Convertimos los streams a strings
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//Si tiene geometry shader, lo utilizamos
		if (geometryPath != nullptr){
			std::ifstream gShaderFile(geometryPath);
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR: No se han podido leer los archivos del SHADER" << std::endl;
	}

	//Convertimos los strings a const char
	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	//**********************************************************************************
	//**********************************************************************************
	// 2. Compilar los shaders recogidos de fichero
	//**********************************************************************************
	//**********************************************************************************

	int success; //Para almacenar el exito o fracaso de la operacion
	char infoLog[512]; //para almacenar el error obtenido en caso de fracaso en la creacion de cada shader

	//======================================================================================================
	//Compilar el VERTEX SHADER recogido antes mediante un objeto shader
	//======================================================================================================

	unsigned int vertexShader; //Para almacenar el vertex shader creado

	//Le decimos a OpenGL que se trata de un vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Adjuntamos el codigo del vertex shader que hemos leido anteriormente
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	//Compilamos el vertex shader
	glCompileShader(vertexShader);

	//Funcion que devuelve el resultado de la operacion
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//En caso de error, recogemos el error y lo mostramos por pantalla
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR en compilacion de VERTEX SHADER: " << infoLog << std::endl;
	}

	//======================================================================================================
	//Compilar el FRAGMENT SHADER recogido antes mediante un objeto shader
	//======================================================================================================

	unsigned int fragmentShader; //Para almacenar el fragment shader creado

	//Le decimos a OpenGL que se trata de un fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Adjuntamos el codigo del fragment shader que hemos leido anteriormente
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	//Compilamos el fragment shader
	glCompileShader(fragmentShader);

	//Funcion que devuelve el resultado de la operacion
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	//En caso de error, recogemos el error y lo mostramos por pantalla
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR en compilacion de FRAGMENT SHADER: " << infoLog << std::endl;
	}

	//======================================================================================================
	//Compilar el GEOMETRY SHADER recogido antes mediante un objeto shader (si hay geometry shader)
	//======================================================================================================

	unsigned int geometryShader; //Para almacenar el geometry shader creado

	if (geometryPath != nullptr){ //Si existe geometry shader...
		//Convertimos el string a char
		const char* gShaderCode = geometryCode.c_str();

		//Le decimos a OpenGL que se trata de un geometry shader
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		//Adjuntamos el codigo del geometry shader que hemos leido anteriormente
		glShaderSource(geometryShader, 1, &gShaderCode, NULL);
		//Compilamos el geometry shader
		glCompileShader(geometryShader);

		//Funcion que devuelve el resultado de la operacion
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			//En caso de error, recogemos el error y lo mostramos por pantalla
			glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
			std::cout << "ERROR en compilacion de GEOMTRY SHADER: " << infoLog << std::endl;
		}
	}

	//******************************************************************************************
	//******************************************************************************************
	// 3. Creamos el programa Shader linkeando los shaders que hemos creado en el paso anterior
	//******************************************************************************************
	//******************************************************************************************

	//Creamos el programa shader guardando su id
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);    //|
	glAttachShader(ID, fragmentShader);  //|Adjuntamos el vertex y shader fragment
	if (geometryPath != nullptr){		 // Si hay geometry shader, lo adjuntamos tambien
		glAttachShader(ID, geometryShader); 
	}
	glLinkProgram(ID); //Linkeamos el shader despues de adjuntar los distintos tipos de shaders

	//Funcion que devuelve el resultado de la operacion
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		//En caso de error, recogemos el error y lo mostramos por pantalla
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR en linkeado del SHADER: " << infoLog << std::endl;
	}

	//Una vez linkeados, eliminamos los distintos shaders creados
	glDeleteShader(vertexShader); 
	glDeleteShader(fragmentShader);
	if (geometryPath != nullptr){	
		glDeleteShader(geometryShader); 
	}
}

//-----------------------------
// FUNCIONES DE USO DEL SHADER
//-----------------------------

//Funcion para activar el shader antes de su uso
void Shader::use(){
	glUseProgram(ID);
}
//Funcion que devuelve la ID del shader
GLuint Shader::getID(){
	return ID;
}
//Funcion que devuelve el numero de programa del shader
GLuint Shader::getProgram(){
	return program;
}
//Funcion que devuelve el nombre del shader
const char* Shader::getName(){
	return shaderName;
}

//---------------------------------------------
// FUNCIONES PARA EL PASO DE UNIFORMS AL SHADER
//---------------------------------------------

//Funcion que establece un uniform de tipo boolean
void Shader::setBool(const std::string &name, bool value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
//Funcion que establece un uniform de tipo int
void Shader::setInt(const std::string &name, int value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
//Funcion que establece un uniform de tipo float
void Shader::setFloat(const std::string &name, float value) const{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
//Funcion que establece un uniform de tipo vec2
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
//Funcion que establece un uniform de tipo vec3
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
//Funcion que establece un uniform de tipo vec4
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
//Funcion que establece un uniform de tipo mat2
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
//Funcion que establece un uniform de tipo mat2
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
//Funcion que establece un uniform de tipo mat2
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

