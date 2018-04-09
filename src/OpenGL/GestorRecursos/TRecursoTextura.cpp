#include  "TRecursoTextura.hpp"

TRecursoTextura::TRecursoTextura(const aiMaterial *mat) {
	
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &name) == AI_SUCCESS) {
		this->setNombre(name.C_Str());
	}
}

TRecursoTextura::~TRecursoTextura() {
	text = NULL;
}

void TRecursoTextura::activeTexture(Shader *shader) {
	if (textures.size() > 0) {
		//Recuperamos el id de la textura difusa
		GLuint id = textures.at("texture_diffuse");
		//Activamos la textura difusa -> siempre es la primera == GL_TEXTURE0
		glActiveTexture(GL_TEXTURE0);
		//Enlazamos la textura difusa a partir de su id recuperado
		glBindTexture(GL_TEXTURE_2D, id);
		//Le pasamos al shader la textura difusa a utilizar
		glUniform1i(glGetUniformLocation(shader->ID, "texture_diffuse"), 0);

		if (textures.size() > 1) {
			//Recuperamos el id de la textura especular
			id = textures.at("texture_specular");
			//Activamos la textura especular -> siempre es la segunda == GL_TEXTURE1
			glActiveTexture(GL_TEXTURE1);
			//Enlazamos la textura especular a partir de su id recuperado
			glBindTexture(GL_TEXTURE_2D, id);
			//Le pasamos al shader la textura especular a utilizar
			glUniform1i(glGetUniformLocation(shader->ID, "texture_specular"), 1);
		}

		if (textures.size() > 2) {
			//Recuperamos el id de la textura especular
			id = textures.at("texture_normal");
			//Activamos la textura especular -> siempre es la segunda == GL_TEXTURE1
			glActiveTexture(GL_TEXTURE2);
			//Enlazamos la textura especular a partir de su id recuperado
			glBindTexture(GL_TEXTURE_2D, id);
			//Le pasamos al shader la textura especular a utilizar
			glUniform1i(glGetUniformLocation(shader->ID, "texture_normal"), 2);
		}
	}

}


void TRecursoTextura::disableTexture() {
	for (size_t i = 0; i < 8; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

// METODOS SET
void TRecursoTextura::setNombre(const char *s) { nombre = new std::string(s); }
void TRecursoTextura::setTexture(std::string texturePath, GLuint texture) {
	textures.insert(
		{
			texturePath,
			texture
		}
	);
}

// METODOS GET
const char *TRecursoTextura::getNombre() { return nombre->c_str(); }
GLuint TRecursoTextura::getDiffuseTextureID() { return diffuseTexture; }
GLuint TRecursoTextura::getSpecularTextureID() { return specularTexture; }
GLuint TRecursoTextura::getNormalTextureID() { return normalTexture; }
