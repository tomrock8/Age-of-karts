FUENTE := $(wildcard src/*.cpp)
OBJETOS := $(subst src/,obj/,$(subst .cpp,.o,$(FUENTE)))
LIBRERIAS := -lIrrlicht -lXxf86vm -lGL -lX11 -lSDL -lGL -lSDL_image -lGLU -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody -lBulletWorldImporter -lBulletFileLoader -lRakNetLibStatic -lpthread 
RUTAS := -I. -I/usr/local/include/bullet/
CFLAGS := -ggdb -std=c++11
.PHONY: objdir info all 

all: objdir exec

exec: $(OBJETOS)
	@g++ -o $@ $^ $(RUTAS) $(LIBRERIAS) $(CFLAGS)
	@echo "Generado ejecutable. Ejecutar ./$@"

obj/%.o : src/%.cpp
	@echo "Compilando $@..."
	@g++ -o $@ -c $^ $(RUTAS) $(CFLAGS)

objdir:
	@mkdir -p obj/
	@echo "Creando carpeta de compilados."

info:
	$(info $(FUENTE))
	$(info $(OBJETOS))

clean:
	@rm -f -r obj/
	@rm -f exec
	@echo "Limpieza completada."
#	echo "Limpiando resultado de compilacion."

#TODO:
#  Comandos para compilar en release
#  Comandos para compilar para Windows 
#  Buscar en subcarpetas en src como pueda ser src/fachada
#  Comando para compilar y ejecutar de una
