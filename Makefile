FUENTE := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OBJETOS := $(subst src/,obj/,$(subst .cpp,.o,$(FUENTE)))

LIBRERIAS :=   -lGL -lSDL -lSDL_image -lGLU -lRakNetLibStatic  -lpthread -lglfw -lGLEW -lassimp -lopenal -lalut -lsndio
LIBRERIAS_BULLET := -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody -lBulletWorldImporter -lBulletFileLoader 

RUTAS2 := -I. -I/usr/local/include/bullet/ 
RUTAS :=  -I./include/raknet -I./include/bullet -I./include -I./include/assimp -I./include/GL -I./include/GLFW -I./include/glm -I./include/sdl -I./include/stb -I./include/openal -L./lib
RUTAS_INTERNAS := -I./src -I./src/Carrera -I./src/Corredor -I./src/Escena -I./src/Item -I./src/Motor -I./src/imgui -I./src/OpenGL/Motor -I./src/OpenGL/ArbolEscena -I./src/OpenGL/Fachada -I./src/OpenGL/Shader -I./src/OpenGL/GestorRecursos -I./src/OpenGL

CFLAGS := -ggdb -std=c++11 
DEBUG := -O
.PHONY: objdir info all 

all: objdir exec
release: changeRelease objdir exec

exec: $(OBJETOS)
	@g++ $(DEBUG) -o $@ $^  $(RUTAS) $(RUTAS_INTERNAS) $(LIBRERIAS) $(LIBRERIAS_BULLET) $(CFLAGS)	
	@echo "Generado ejecutable."

obj/%.o : src/%.cpp
	@g++ $(DEBUG) -o $@ -c $^ $(RUTAS) $(RUTAS_INTERNAS)  $(CFLAGS)
	@echo "Compilado $@."

changeRelease:
	$(eval DEBUG := -O2)

objdir:
	@mkdir -p obj/
	@mkdir -p obj/Carrera
	@mkdir -p obj/Corredor
	@mkdir -p obj/Escena
	@mkdir -p obj/Item
	@mkdir -p obj/Motor
	@mkdir -p obj/OpenGL
	@mkdir -p obj/OpenGL/ArbolEscena
	@mkdir -p obj/OpenGL/Fachada
	@mkdir -p obj/OpenGL/Shader
	@mkdir -p obj/OpenGL/GestorRecursos
	@mkdir -p obj/OpenGL/Motor
	@mkdir -p obj/imgui
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
