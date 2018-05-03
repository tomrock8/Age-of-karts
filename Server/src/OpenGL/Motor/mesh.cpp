#include "mesh.hpp"


mesh::mesh() {
	meshe = NULL;
	mat = NULL;
	text = NULL;
}
mesh::~mesh() {
	delete meshe;
}

void mesh::setMesh(TRecursoMalla *m) {
	meshe = m;
	meshe->inicializar();
}
void mesh::setMat(TRecursoMaterial *m) {
	mat = m;
}
void mesh::setTex(TRecursoTextura *t) {
	text = t;

}
TRecursoMalla *mesh::getMesh() {
	return meshe;
}
TRecursoMaterial *mesh::getMat() {
	return mat;
}
TRecursoTextura *mesh::getText() {
	return text;
}