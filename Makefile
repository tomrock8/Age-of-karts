FUENTE := $(wildcard src/*.cpp)
OBJETOS := $(subst src/,obj/,$(subst .cpp,.o,$(FUENTE)))
LIBRERIAS := -lIrrlicht -lXxf86vm -lGL -lX11
RUTAS := -I. -I/usr/local/include/bullet/
CFLAGS := -ggdb

.PHONY: objdir info all 

all: objdir exec

exec: $(OBJETOS)
	g++ -o $@ $^ $(RUTAS) $(LIBRERIAS) $(CFLAGS)

obj/%.o : src/%.cpp
	g++ -o $@ -c $^ $(RUTAS) $(CFLAGS)

objdir:
	mkdir -p obj/

info:
	$(info $(FUENTE))
	$(info $(OBJETOS))


#TODO:
#  Comandos para compilar en release
#  Comandos para compilar para Windows 
#  Buscar en subcarpetas en src como pueda ser src/fachada
#  Comando para compilar y ejecutar de una
