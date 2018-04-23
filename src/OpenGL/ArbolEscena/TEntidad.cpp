#include "TEntidad.hpp"

std::stack<glm::mat4> TEntidad::matStack;
glm::mat4 TEntidad::modelMatrix;
glm::mat4 TEntidad::viewMatrix;
glm::mat4 TEntidad::projectionMatrix;
//glm::mat4 TEntidad::MVPmatrix;

void TEntidad::setModelMatrix(glm::mat4 modelMatrix) {
	this->modelMatrix = modelMatrix;
}

void TEntidad::setViewMatrix(glm::mat4 viewMatrix)
{
	this->viewMatrix = viewMatrix;
}

void TEntidad::setProjectionMatrix(glm::mat4 projectionMatrix) {
	this->projectionMatrix = projectionMatrix;
}

glm::mat4 TEntidad::getModelViewMatrix() {
	return viewMatrix * modelMatrix;
}

glm::mat4 TEntidad::getProjectionMatrix() {
	return projectionMatrix;
}

glm::mat4 TEntidad::getViewMatrix() {
	return viewMatrix;
}

glm::mat4 TEntidad::getModelViewProjectionMatrix() {
	return projectionMatrix * viewMatrix * modelMatrix;
}

