/*
#include "Skybox.hpp"


void Skybox::createSkyBox(){
    createCubeMap();
    createShader();
    setCubeVertices();

    glEnableVertexAttribArray(vertex);
    glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);*/

/*
    //---------MODEL VIEW MATRIX---------
    glm::mat4 Projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); 
    glm::mat4 View       = glm::mat4(1.0f);
    glm::mat4 Model      = glm::scale(glm::mat4(1.0f),glm::vec3(50,50,50));

    //---------LOOP UPDATE-----------
    if (kb.getKeyState(KEY_UP))    alpha += 180.0f*elapsed0;
    if (kb.getKeyState(KEY_DOWN))  alpha -= 180.0f*elapsed0;
    if (kb.getKeyState(KEY_LEFT))  beta  -= 180.0f*elapsed0;
    if (kb.getKeyState(KEY_RIGHT)) beta  += 180.0f*elapsed0;
    jp[0] = js.joystickPosition(0);
    alpha += jp[0].y*elapsed0*180.0f;
    beta  += jp[0].x*elapsed0*180.0f;
    
    glm::mat4 RotateX = glm::rotate(glm::mat4(1.0f), alpha, glm::vec3(-1.0f, 0.0f, 0.0f));
    glm::mat4 RotateY = glm::rotate(RotateX, beta, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 M = Projection * View * Model * RotateY;
    glUniformMatrix4fv(PVM, 1, GL_FALSE, glm::value_ptr(M));


    //----------RENDER-----------
    //glDrawElements(GL_QUADS, sizeof(cube_indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}*/
/*
void Skybox::createCubeMap(){
    xpos = IMG_Load("assets/skybox/SkyBox_Front.bmp"); xneg = IMG_Load("assets/skybox/SkyBox_Back.bmp");
    ypos = IMG_Load("assets/skybox/SkyBox_Top.bmp"); yneg = IMG_Load("assets/skybox/SkyBox_Bottom.bmp");
    zpos = IMG_Load("assets/skybox/SkyBox_Left.bmp"); zneg = IMG_Load("assets/skybox/SkyBox_Right.bmp");
    GLuint cubemap_texture;
    setupCubeMap(cubemap_texture, xpos, xneg, ypos, yneg, zpos, zneg);
    SDL_FreeSurface(xneg);  SDL_FreeSurface(xpos);
    SDL_FreeSurface(yneg);  SDL_FreeSurface(ypos);
    SDL_FreeSurface(zneg);  SDL_FreeSurface(zpos);
    
    // rendering..
    
    //deleteCubeMap(cubemap_texture);
}

void Skybox::createShader(){
     // initialize the extension wrangler
    glewInit();
    
    // load our shaders and compile them.. create a program and link it
    GLuint glShaderV = glCreateShader(GL_VERTEX_SHADER);
    GLuint glShaderF = glCreateShader(GL_FRAGMENT_SHADER);
    //Shader sh=TMotor::instancia().getShaderSkybox();
    const GLchar* vShaderSource = loadFile("assets/shaders/shaderSkybox/vertexShader.txt");
    const GLchar* fShaderSource = loadFile("assets/shaders/shaderSkybox/fragmentShader.txt");
    glShaderSource(glShaderV, 1, &vShaderSource, NULL);
    glShaderSource(glShaderF, 1, &fShaderSource, NULL);
    delete [] vShaderSource;
    delete [] fShaderSource;
    glCompileShader(glShaderV);
    glCompileShader(glShaderF);
    GLuint glProgram = glCreateProgram();
    glAttachShader(glProgram, glShaderV);
    glAttachShader(glProgram, glShaderF);
    glLinkProgram(glProgram);
    glUseProgram(glProgram);
    
    // shader logs
    int  vlength,    flength;
    char vlog[2048], flog[2048];
    glGetShaderInfoLog(glShaderV, 2048, &vlength, vlog);
    glGetShaderInfoLog(glShaderF, 2048, &flength, flog);
    std::cout << vlog << std::endl << std::endl << flog << std::endl << std::endl;
    
    // grab the pvm matrix and vertex location from our shader program
    PVM    = glGetUniformLocation(glProgram, "PVM");
    vertex = glGetAttribLocation(glProgram, "vertex");
}

void Skybox::setCubeVertices(){
    // cube vertices for vertex buffer object
    GLfloat cube_vertices[] = {
    -1.0,  1.0,  1.0,
    -1.0, -1.0,  1.0,
    1.0, -1.0,  1.0,
    1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,
    -1.0, -1.0, -1.0,
    1.0, -1.0, -1.0,
    1.0,  1.0, -1.0,
    };
    GLuint vbo_cube_vertices;
    glGenBuffers(1, &vbo_cube_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // cube indices for index buffer object
    GLushort cube_indices[] = {
    0, 1, 2, 3,
    3, 2, 6, 7,
    7, 6, 5, 4,
    4, 5, 1, 0,
    0, 3, 7, 4,
    1, 2, 6, 5,
    };
    GLuint ibo_cube_indices;
    glGenBuffers(1, &ibo_cube_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Skybox::setupCubeMap(GLuint& texture) {
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
 
void Skybox::setupCubeMap(GLuint& texture, SDL_Surface *xpos, SDL_Surface *xneg, SDL_Surface *ypos, SDL_Surface *yneg, SDL_Surface *zpos, SDL_Surface *zneg) {
    setupCubeMap(texture);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->w, xpos->h, 0, xpos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, xpos->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->w, xneg->h, 0, xneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, xneg->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->w, ypos->h, 0, ypos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, ypos->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->w, yneg->h, 0, yneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, yneg->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->w, zpos->h, 0, zpos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, zpos->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->w, zneg->h, 0, zneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, zneg->pixels);
}
 
void Skybox::deleteCubeMap(GLuint& texture) {
    glDeleteTextures(1, &texture);
}




*/