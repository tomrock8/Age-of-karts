#include "mesh.hpp"

mesh::mesh() {
	meshe = nullptr;
	mat = nullptr;
	text = nullptr;
}
mesh::~mesh() {
	//std::cout << "Destructor de mesh\n";
	meshe =nullptr;
	mat=nullptr;
	text=nullptr;
}

void mesh::setMesh(TRecursoMalla *m) {
	meshe = m;
	meshe->inicializar();
}

void mesh::setMat(TRecursoMaterial *m) { mat = m; }
void mesh::setTex(TRecursoTextura *t) { text = t; }
TRecursoMalla *mesh::getMesh() { return meshe; }
TRecursoMaterial *mesh::getMat() { return mat; }
TRecursoTextura *mesh::getText() { return text; }