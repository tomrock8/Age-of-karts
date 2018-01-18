FUENTE := $(wildcard src/*.cpp)
OBJETOS := $(subst src/,obj/,$(subst .cpp,.o,$(FUENTE)))
LIBRERIAS := -lIrrlicht -lXxf86vm -lGL -lX11 -lSDL -lSDL_image -lGLU -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody -lBulletWorldImporter -lBulletFileLoader -lRakNetLibStatic 
LIBRERIAS2 := -lIrrlicht -lXxf86vm -lGL -lX11 -lSDL -lGL -lSDL_image -lGLU -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody -lBulletWorldImporter -lBulletFileLoader -lRakNetLibStatic -lpthread 
RUTAS2 := -I. -I/usr/local/include/bullet/
#ARCHIVOS_SO := libBulletDynamics.so libBulletCollision.so libBulletSoftBody.so libLinearMath.so libBulletWorldTransform.so libBulletFileLoader.so libIrrlicht.so.1.8.4 libBulletDynamics.so.2.88 libBulletCollision.so.2.88 libBulletSoftBody.so.2.88 libLinearMath.so.2.88 libBulletWorldTransform.so.2.88 libBulletFileLoader.so.2.88

RUTAS := -I./include/irrlicht -I./include/raknet -I./include/irrlicht/include -I./include/bullet -L./lib 
CFLAGS := -ggdb -std=c++11 
.PHONY: objdir info all 

all: objdir exec

run: objdir exec run

alt: objdir exec2 

runalt: objdir exec2 altRun

exec2: $(OBJETOS)
	@g++ -o $@ $^ $(RUTAS2) $(LIBRERIAS2) $(CFLAGS) -Wl,-rpath=/usr/local/lib
	@echo "Generado ejecutable. Ejecutar ./$@"

exportAlt: 
	export LD_LIBRARY_PATH="/usr/local/lib"


altRun:
	@./exec2
	@echo "Ejecutando."

run:
	@./exec
	@echo "Ejecutando."

export: 
	LD_LIBRARY_PATH=./lib
	export LD_LIBRARY_PATH

exec: $(OBJETOS)
	#export RUTA_LIB=${HOME}:${RUTA_LIB}
	#export RUTA_LIB=$(HOME):$(RUTA_LIB)
	#export PATH=$PATH:/usr/local/bin
	#echo "$PATH"
	#sudo cp lib/*.so.2.88 /usr/local/lib
	export RUTA_LIB=/usr/local/lib

	
	g++ -fPIC -g -o $@ $^  $(RUTAS) $(LIBRERIAS) $(CFLAGS) -Wl,-rpath=./lib	
	@echo "Generado ejecutable."

obj/%.o : src/%.cpp
	@g++ -fPIC -g -o $@ -c $^ $(RUTAS) $(CFLAGS)
	@echo "Compilado $@."

objdir:
	@mkdir -p obj/
	@echo "Creando carpeta de compilados."

info:
	$(info $(FUENTE))
	$(info $(OBJETOS))

clean:
	@rm -f -r obj/
	@rm -f exec
	@rm -f exec2
	@echo "Limpiando resultado de compilacion."

help: 
	@echo "Comandos utiles en Age Of Karts"
	@echo "- make: para compilar con las librerias del proyecto"
	@echo "- make run: para compilar con make y ejecutar"
	@echo "- make alt: para compilar con las librerias instaladas en el sistema"
	@echo "- make runalt: para compilar con make alt y ejecutar"
	@echo "- make clean: para eliminar la carpeta de objetos y los ejecutables"
	@echo "- make info: para ver la informacion de los archivos de src"

#TODO:
#  Comandos para compilar en release
#  Comandos para compilar para Windows 
#  Buscar en subcarpetas en src como pueda ser src/fachada
#  Comando para compilar y ejecutar de una
# libtarget.so: main.o
#   gcc -shared  -o libtarget.so main.o
