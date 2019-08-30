#ifndef ESCENA_H
#define ESCENA_H

class Escena {
public:
	enum tipo_escena { MENU, CARRERA, ONLINE, CARGA, LOBBY, OPCIONES, CREDITOS, SALIR, PODIO }; // Definicion de los diferentes tipos de escenas
	static const int nTipos = 6; // Numero de tipos de escenas que pueden haber

	enum estado_escena { CAMBIO, CORRIENDO, CARGANDO }; // Definicion del estado actual de la escena

	Escena(tipo_escena tipo);
	~Escena();

	// Metodos virtuales a implementar por los hijos

	// Inicializar la escena
	virtual void init() = 0;

	// Dibujar los elementos de la escena
	virtual void dibujar() = 0;

	// Borrar los elementos de la escena
	virtual void limpiar() = 0;

	// Actualizar la escena y sus elementos
	virtual void update() = 0;

	// Comprobar los inputs en la escena
	//	devuelve true si tiene que cambiar de escena
	virtual tipo_escena comprobarInputs() = 0;

	// METODOS GET

	// Devuelve el tipo de escena
	tipo_escena getTipoEscena();

	// METODOS SET

protected:

	tipo_escena tipoEscena; // Tipo de escena a la que pertenece

};

#endif /* ESCENA_H */