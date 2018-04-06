#include  "TRecursoMaterial.hpp"


TRecursoMaterial::TRecursoMaterial(const aiMaterial *mat) {
	if (mat->Get(AI_MATKEY_NAME, name) == AI_SUCCESS) {
		setNombre(name.C_Str());
	}
	if (mat->Get(AI_MATKEY_SHININESS, shiny) != AI_SUCCESS) {
		setShiny(128.0);// si no hay brillo se inicializa a un valor por defecto
	}
	if (mat->Get(AI_MATKEY_SHININESS_STRENGTH, hardShiny) != AI_SUCCESS) {
		setHardShiny(1.0);// lo mismo para la fuerza 
	}
	//tratamiento para el color ambient
	if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient) == AI_SUCCESS) {
		setColorAmbient(ambient.r, ambient.g, ambient.b, ambient.a);
	}
	//tratamiento para el color difuso
	if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS) {
		setColorDiffuse(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	}
	//tratamiento para el color specular
	if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular) == AI_SUCCESS) {
		setColorSpecular(specular.r, specular.g, specular.b, specular.a);
	}
}

TRecursoMaterial::~TRecursoMaterial() {
	delete[] color_ambient;
	delete[] color_diffuse;
	delete[] color_specular;
	
}

//PASARLE AL SHADER EL MATERIAL ANTES DEL DIBUJADO
void TRecursoMaterial::activeMaterial(Shader *shader){
	shader->setVec4("material.color_ambient", color_ambient[0], color_ambient[1], color_ambient[2], color_ambient[3]);
	shader->setVec4("material.color_diffuse", color_diffuse[0], color_diffuse[1], color_diffuse[2], color_diffuse[3]);
	shader->setVec4("material.color_specular", color_specular[0], color_specular[1], color_specular[2], color_specular[3]);
	shader->setFloat("material.brightness", shiny);
}

//--------------------------------//
//------HERENCIA DE TRECURSO------//
//--------------------------------//
const char *TRecursoMaterial::getNombre() {
	return nombre;
}
void TRecursoMaterial::setNombre(const char *s) {
	nombre = s;
}

// METODOS GET
float TRecursoMaterial::getShiny() { return shiny; }
float TRecursoMaterial::getHardShiny() { return hardShiny; }

// METODOS SET
void TRecursoMaterial::setShiny(float n) { shiny = n; }
void TRecursoMaterial::setHardShiny(float n) { hardShiny = n; }
void TRecursoMaterial::setColorAmbient(float r, float g, float b, float a) {
	color_ambient[0] = r;
	color_ambient[1] = g;
	color_ambient[2] = b;
	color_ambient[3] = a;
}
void TRecursoMaterial::setColorDiffuse(float r, float g, float b, float a) {
	color_diffuse[0] = r;
	color_diffuse[1] = g;
	color_diffuse[2] = b;
	color_diffuse[3] = a;
}
void TRecursoMaterial::setColorSpecular(float r, float g, float b, float a) {
	color_specular[0] = r;
	color_specular[1] = g;
	color_specular[2] = b;
	color_specular[3] = a;
}



