#ifndef ESCENA_H
#define ESCENA_H



class Escena {
public:
	typedef enum { MENU, CARRERA, CARGA, LOBBY, OPCIONES, CREDITOS } tipo_escena; // Definicion de los diferentes tipos de escenas
	static const int nTipos = 6; // Numero de tipos de escenas que pueden haber

	typedef enum { CAMBIO, CORRIENDO, CARGANDO } estado_escena; // Definicion del estado actual de la escena

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
	virtual bool comprobarInputs() = 0;

	// METODOS GET

	// Devuelve el tipo de escena
	tipo_escena getTipoEscena();

	// METODOS SET

protected:

	tipo_escena tipoEscena; // Tipo de escena a la que pertenece



};

#endif /* ESCENA_H */