#ifndef ESCENA_H
#define ESCENA_H



class Escena {
public:
	typedef enum { MENU, CARRERA, CARGA, LOBBY, OPCIONES, CREDITOS } tipo_escena;

	Escena(tipo_escena tipo);
	~Escena();

	// Metodos virtuales a implementar por los hijos
	virtual void init() = 0;	// Inicializar la escena
	virtual void dibujar() = 0;	// Dibujar los elementos de la escena
	virtual void limpiar() = 0;	// Borrar los elementos de la escena
	virtual void update() = 0;	// Actualizar la escena y sus elementos

	// METODOS GET
	tipo_escena getTipoEscena(); // Devuelve el tipo de escena

	// METODOS SET
	
protected:

	tipo_escena tipoEscena;

	virtual void comprobarInputs() = 0; // Comprobar los inputs en la escena

};

#endif /* ESCENA_H */