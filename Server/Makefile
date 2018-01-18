FUENTE := $(wildcard src/*.cpp)
OBJETOS := $(subst src/,obj/,$(subst .cpp,.o,$(FUENTE)))
LIBRERIAS := -lXxf86vm -lGL -lX11 -lSDL -lSDL_image -lGLU -lRakNetLibStatic 
LIBRERIAS2 := -lXxf86vm -lGL -lX11 -lRakNetLibStatic -lpthread
#ARCHIVOS_SO := libBulletDynamics.so libBulletCollision.so libBulletSoftBody.so libLinearMath.so libBulletWorldTransform.so libBulletFileLoader.so libIrrlicht.so.1.8.4 libBulletDynamics.so.2.88 libBulletCollision.so.2.88 libBulletSoftBody.so.2.88 libLinearMath.so.2.88 libBulletWorldTransform.so.2.88 libBulletFileLoader.so.2.88

RUTAS := -I./../include/raknet -L./../lib -I.
RUTAS2 := -I.
CFLAGS := -ggdb -std=c++11 
.PHONY: objdir info all 

all: objdir exec

alt: objdir exec2


exec2: $(OBJETOS)
	g++ -o $@ $^ $(RUTAS2) $(LIBRERIAS2) -std=c++11

exec: $(OBJETOS)
	#export RUTA_LIB=${HOME}:${RUTA_LIB}
	#export RUTA_LIB=$(HOME):$(RUTA_LIB)
	#export PATH=$PATH:/usr/local/bin
	#echo "$PATH"
	#sudo cp lib/*.so.2.88 /usr/local/lib
	export RUTA_LIB=/usr/local/lib

	
	g++ -fPIC -g -o $@ $^  $(RUTAS) $(LIBRERIAS) $(CFLAGS)	
	LD_LIBRARY_PATH=./lib
	export LD_LIBRARY_PATH
	@echo "Generado ejecutable."

obj/%.o : src/%.cpp
	@g++ -fPIC -g -o $@ -c $^ $(RUTAS) $(CFLAGS)
	@echo "Compilado."

objdir:
	@mkdir -p obj/
	@echo "Creando carpeta de compilados."

info:
	$(info $(FUENTE))
	$(info $(OBJETOS))

clean:
	rm -f -r obj/
	rm -f exec
#	echo "Limpiando resultado de compilacion."

#TODO:
#  Comandos para compilar en release
#  Comandos para compilar para Windows 
#  Buscar en subcarpetas en src como pueda ser src/fachada
#  Comando para compilar y ejecutar de una
# libtarget.so: main.o
#   gcc -shared  -o libtarget.so main.o
