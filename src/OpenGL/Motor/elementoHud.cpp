#include "elementoHud.hpp"

//Constructor que crea e inicializa el quad con una textura
elementoHud::elementoHud(float w, float h, const char* n, const char* p){ 
    //Guardamos los datos 
    width = w; 
    height = h; 
    name = n; 
    path = p; 
    color = glm::vec4(0.f, 0.f, 0.f, 0.f);
    transparency = false;
    transLevel = 0.6; //Por defecto

    //Inicializamos y creamos el elemento del hud
    setData();
    setBuffersOpenGL();
}

//Constructor que crea e inicializa el quad con una color
elementoHud::elementoHud(float w, float h, const char* n, glm::vec4 c){ 
    //Guardamos los datos 
    width = w; 
    height = h; 
    name = n; 
    path = ""; 
    color = c;
    transparency = false;
    transLevel = 0.6; //Por defecto

    //Inicializamos y creamos el elemento del hud
    setData();
    setBuffersOpenGL();
}

//Destructor del elemento hud
elementoHud::~elementoHud(){
    //Eliminar los distintos buffers utilizados por OpenGL
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);

}


//Funcion que asigna las posiciones de los vertices del quad a partir del ancho y alto definido
//Tambien, establece las coordenadas de textura para cada punto
void elementoHud::setData(){
    //Dividimos entre 2 para sacar la x e y normalizadas entre [-1, 1]
    float x = width/2.0f;
    float y = height/2.0f;

    array <float, 20> vertices = {
        //Posiciones     //Coordenadas de textura
        x, y, 0.0f,   1.0f, 1.0f,   //Esquina superior derecha
		x, -y, 0.0f,   1.0f, 0.0f,  //Esquina inferior derecha
		-x, -y, 0.0f,   0.0f, 0.0f, //Esquina inferior izquierda
		-x, y, 0.0f,   0.0f, 1.0f   //Esquina superior izquierda
    };

    //Guardamos los datos del quad
    data = vertices;
}

//Funcion que inicializa los buffers de OpenGL y pasa los datos  de los vertices y coordenadas de textura
void elementoHud::setBuffersOpenGL(){
    //Inicalizar cada uno de los buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Enlazar el VAO en el que se guardan los distintos datos
    glBindVertexArray(VAO);

    //Enlazar el VBO al que se le pasan los datos del quad
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

    //Enlazar el EBO, al que se le pasan los indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Pasamos los datos al buffer VBO
    //Primero, las posiciones de cada vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Segundo, las coordenadas de textura para cada vertice
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Desactivamos el VAO hasta el dibujado del quad
    glBindVertexArray(0); 
}

//Funcion para dibujar el elemento hud
void elementoHud::draw(Shader *shader){

    //Activamos la textura 0 y enlazamos la imagen del elemento hud
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TGestorImagenes::getInstacia()->cargarImagen(path)->getID());

    //Calculamos la matriz model del elemento
    glm::mat4 model = tras * rot * esc;

    //Pasamos los datos necesarios al shader HUD
    shader->setInt("ourTexture", 0);
    shader->setMat4("model", model);
    shader->setVec4("ourColor", color);
    shader->setBool("transparency", transparency);
    shader->setFloat("transparencyLevel", transLevel);

    //Enlazamos el VAO que antes hemos rellenado
    glBindVertexArray(VAO);
    //Dibujamos el quad con OpenGL
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //Vaciamos los datos utilizamos
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//Funcion para rotar el elemento sobre si mismo
void elementoHud::rotacion(float angle){
    glm::vec3 myRotationAxis(0.0f, 0.0f, 1.0f);
	rot =  glm::rotate(glm::radians(angle), myRotationAxis);
}

//Funcion para trasladar el elemento por la pantalla
void elementoHud::traslacion(float x, float y){
    tras = glm::translate(glm::mat4(), glm::vec3(x, y, 0.0f));
}

//Funcion para escalar el elemento hud
void elementoHud::escalado(float x, float y){
    esc = glm::scale(glm::mat4(), glm::vec3(x,y,1.0f));
}

//Funcion que devuelve el nombre 
const char* elementoHud::getName(){ 
    return name; 
}

//Funcion para cambiar la textura del elemento hud
void elementoHud::cambiarTextura(const char* p){
    //Asignar la nueva ruta
    path = p;
}

//Funcion para cambiar el color del elemento hud
void elementoHud::cambiarColor(glm::vec4 c){
    //Asignar el nuevo color
    color = c;
}

//Funcion para cambiar la transparencia del elemento
//0 == totalmente trasparente , 1 == opaco
void elementoHud::cambiarTransparencia(bool t, float l){
    //Asignamos si el elemento sera transparente o no
    transparency = t;
    //Asignamos el nivel de transparencia en caso de que si lo sea
    transLevel = l;
}