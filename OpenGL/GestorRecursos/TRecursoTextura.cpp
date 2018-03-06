#include  "TRecursoTextura.hpp"
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"

TRecursoTextura::TRecursoTextura(const char* path) {
	this->setNombre(path);
	dir = std::string(path).substr(0, std::string(path).find_last_of('/'));

	loadModel(path);
	//inicializar();
}
TRecursoTextura::~TRecursoTextura() {
	
}
//--------------------------------//
//------HERENCIA DE TRECURSO------//
//--------------------------------//
const char *TRecursoTextura::getNombre() {
	return nombre;
}
void TRecursoTextura::setNombre(const char *s) {
	nombre = s;
}
//--------------------------------//
//----ASSIMP LECTURA TEXTURA------//
//--------------------------------//

void TRecursoTextura::loadModel(string path) {
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "(TRecursoTextura::loadModel) Error cargando assimp" << import.GetErrorString() << endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}
void TRecursoTextura::processNode(aiNode *node, const aiScene *scene) {
	//procesar todas las mallas que se encuentren en el fichero
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];		//se recoje la malla de la escena
		processMesh(mesh, scene);								// se guarda en el vector de malla
	}
	//lo mismo para los hijos que pueda tener
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}
void TRecursoTextura::processMesh(aiMesh *mesh, const aiScene *scene) {

	int numMaterials = scene->mNumMaterials;

	//cargar todas las texturas	que hay en la malla con nNumMaterials, que contiene el numero de materiales
	for (GLuint i = 0; i < numMaterials; i++) {
		//crear variables para los tipos de superficie
		aiColor4D diffuse;
		//cargar los materiales
		loadMaterial(scene, mesh, aiTextureType_DIFFUSE, diffuseTexture);
		//Obtener el material que le corresponde al mesh
		if (mesh->mMaterialIndex >= 0) {
			const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];//recogemos el material
			//controlar el brillo
			if (mat->Get(AI_MATKEY_SHININESS, shiny) != AI_SUCCESS) {
				shiny = 128.0;// si no hay brillo se inicializa a un valor por defecto
			}
			if (mat->Get(AI_MATKEY_SHININESS_STRENGTH, hardShiny) != AI_SUCCESS) {
				hardShiny = 1.0;// lo mismo para la fuerza 
			}
			//tratamiento para el color difuso
			if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS) {
				color_diffuse[0] = diffuse.r;
				color_diffuse[1] = diffuse.g;
				color_diffuse[2] = diffuse.b;
				color_diffuse[3] = diffuse.a;

			}
		}
	}
}

void TRecursoTextura::loadMaterial(const aiScene *scene, const aiMesh *mesh, aiTextureType textureType, GLuint &texture) {
	if (mesh->mMaterialIndex >= 0) {
		const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		for (GLuint i = 0; i < mat->GetTextureCount(textureType); i++) {
			aiString path;
			if (mat->GetTexture(textureType, i, &path) == AI_SUCCESS) {
				const string texturePath = path.C_Str();
				std::cout << "(TRecursoTextura::loadMaterial) Estoy cargando la textura : " << dir + "/" + texturePath << std::endl;

				//asignar la textura

				texture = textureFromFile(dir + "/" + texturePath);
				textures.insert(
					{
						texturePath,
						texture
					}
				);
			}
		}
	}
}
//cargar textura y generar identificador
GLuint TRecursoTextura::textureFromFile(const std::string& file) {
	GLuint idTexture = 0;
	glGenTextures(1, &idTexture);

	int width, height, comp;
	unsigned char *imgTexture = stbi_load(file.c_str(), &width, &height, &comp, 3);

	glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexImage2D(GL_TEXTURE_2D, 9, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);//liberamos la textura

	stbi_image_free(imgTexture);

	return idTexture;
}

void TRecursoTextura::activeTexture(GLuint program) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	glUniform1i(glGetUniformLocation(program, "material.diffuse"), 0);
}
void TRecursoTextura::disableTexture() {
	for (size_t i = 0; i < 8; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
