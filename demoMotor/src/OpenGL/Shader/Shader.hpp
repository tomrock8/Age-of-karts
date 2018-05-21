#ifndef SHADER_H
#define SHADER_H

#include "libGL.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	Shader(const char* n, const char* vertexPath, const char* fragmentPath, const char* geometryPath); //Constructor

	//Funciones de uso del shader
	void use();
	GLuint getID();
	GLuint getProgram();
	const char* getName();

	// Funciones para asignar uniforms al shader
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	

private:
	// --- VARIABLES ---
	GLuint ID; // ID del programa shader
	GLuint program; // Numero de programa del shader
	const char* shaderName; //Nombre del shader


};

#endif