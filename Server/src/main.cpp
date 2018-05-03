#include <ctime>

#include "obj3D.hpp"
#include "MotorFisicas.hpp"
#include "DebugFisicas.hpp"
#include "Camara3persona.hpp"

#include "Pista.hpp"
#include "GestorJugadores.hpp"
#include "GestorCarrera.hpp"
#include "GestorColisiones.hpp"
#include <vector>
#include "Server.hpp"
#include "Timer.hpp"


const int W_WIDTH = 1024;
const int H_WIDTH = 600;


void UpdatePhysics(unsigned int TDeltaTime);
void UpdateRender(btRigidBody *TObject);

int main()
{
    int x = 0;
    
    int debug;
    
    cameraThird *camara;
    int tipoCamara;
    bool cambioCamara;
    GestorColisiones *colisiones;
    
    std::uint32_t DeltaTime;
    std::uint32_t TimeStamp;
    
    Server *server;
    btVector3 pos;
    
    clock_t tiempoRefresco;
    clock_t tiempoRefrescoServer;
    GestorCarrera *gc;
    bool fin_carrera;
    std::string mensaje;
    //RED
    
    server = new Server(8);
    server->CreateServerInterface();
    server->ServerStartup();
    
    
    debug = 0;
    fin_carrera=false;
    
    
    Timer *t = Timer::getInstancia(); 
    
    GestorIDs::instancia().restartID(); 
    
    // Gravedad
    MotorFisicas::getInstancia()->getMundo()->setGravity(btVector3(0.0, -45.f, 0.0));
    
    //-----------------------------
    //	ESCENARIO MAPA
    //-----------------------------
    Pista *pistaca = Pista::getInstancia();
    Pista::getInstancia()->setMapa("pirata");
            
    
    //-----------------------------
    //	JUGADORES
    //-----------------------------
    //Posicion del nodo y el bloque de colisiones centralizado:
    
    GestorJugadores *jugadores = GestorJugadores::getInstancia();
    
    gc = new GestorCarrera();
    
    //-----------------------------
    //	CAMARA
    //-----------------------------
	camara = new cameraThird("camara_jugador3apersona", "escena_raiz");
    
    //-----------------------------
    //	GESTOR COLISIONES
    //-----------------------------
    colisiones = new GestorColisiones();
    
    //-----------------------------
    //	TIME
    //-----------------------------
    TimeStamp = glfwGetTime();
    DeltaTime = 0;
    tiempoRefresco = glfwGetTime();
    tiempoRefrescoServer = glfwGetTime();

	// -----------------------
	//	IMGUI
	// -----------------------
	bool debug_Jugador = false;
	bool muestraDebug = true;
	bool show_another_window=false;
	bool muestraDebugIA=false;
	TMotor::instancia().initDebugWindow();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf",30.0f);

    // -----------------------------
    //  GAME LOOP
    // -----------------------------
    RakSleep(30);
    bool checkInit=false;
    bool iniciar=false;
    while (!glfwWindowShouldClose(TMotor::instancia().getVentana()))
    {
        
        server->ReceivePackets();
        //========================================================
        //EMPIEZA INIT
        //========================================================
        if (iniciar && !server->getStarted()){
            debug = 0;
            fin_carrera=false;
            
            
            t = Timer::getInstancia(); 
            
            GestorIDs::instancia().restartID(); 
            
            // Gravedad
            MotorFisicas::getInstancia()->getMundo()->setGravity(btVector3(0.0, -50.f, 0.0));
            
            //-----------------------------
            //	ESCENARIO MAPA
            //-----------------------------
	        Pista::getInstancia()->setMapa("pirata");
            
            //-----------------------------
            //	JUGADORES
            //-----------------------------
            //Posicion del nodo y el bloque de colisiones centralizado:
            
            jugadores = GestorJugadores::getInstancia();
            
            gc = new GestorCarrera();
            
            //-----------------------------
            //	CAMARA
            //-----------------------------
	        camara = new cameraThird("camara_jugador3apersona", "escena_raiz");
            
            //-----------------------------
            //	GESTOR COLISIONES
            //-----------------------------
            colisiones = new GestorColisiones();
            
            //-----------------------------
            //	TIME
            //-----------------------------
            TimeStamp = glfwGetTime();
            DeltaTime = 0;
            tiempoRefresco = clock();
            tiempoRefrescoServer = clock();           
            checkInit=false;
            jugadores = GestorJugadores::getInstancia();
            std::vector<Corredor*> pj = jugadores->getJugadores();
            for (int i = 0; i < jugadores->getNumJugadores(); i++) { 
                pj.at(i)->getEstados()->setEstadoCarrera(0); 
                pj.at(i)->getEstados()->setEstadoCarrera(0); 
            } 
            iniciar=false;

            // -----------------------
            //	IMGUI
            // -----------------------
            debug_Jugador = false;
            muestraDebug = true;
            show_another_window=false;
            muestraDebugIA=false;
            TMotor::instancia().initDebugWindow();
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontDefault();
            io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf",30.0f);
        }
        //========================================================
        //EMPIEZA UPDATE
        //========================================================
        
        Pista *pistaca = Pista::getInstancia();
        std::vector<Item *> items = pistaca->getItems();
        jugadores = GestorJugadores::getInstancia();
        std::vector<Corredor*> pj = jugadores->getJugadores();
        if(pj.size()==0){
            server->setStarted(false);
        }
        //Comprobamos si el servidor ha iniciado la partida (para iniciar el contador)
        if (!server->getStarted()){
            fin_carrera=false;
            pj = jugadores->getJugadores();
            gc = new GestorCarrera();	
        }else{
            iniciar=true;
            if (checkInit==false){
                t->restartTimer(); 
                checkInit=true;
            }
            if (t->getTimer()<=3 && t->getTimer()>=1){ 
                int desc=4-t->getTimer(); 
            } 
            if (t->getTimer()==4){ 
                for (int i = 0; i < jugadores->getNumJugadores(); i++) { 
                    pj.at(i)->getEstados()->setEstadoCarrera(1); 
                    pj.at(i)->getEstados()->setEstadoCarrera(1); 
                } 
            } 
            //cout<<"jugadores->getnum: "<<jugadores->getNumJugadores();
            //cout<<"size: "<<pj.size()<<endl;
            //cout << irrTimer->getTime() << endl;
            
            DeltaTime = glfwGetTime() * 1000 - TimeStamp;
            TimeStamp = glfwGetTime() * 1000;
            UpdatePhysics(DeltaTime);
            
            for (int i = 0; i < pistaca->getTamCajas(); i++) {
                pistaca->getArrayCaja()[i]->comprobarRespawn(); // TODO: MOVER AL UPDATE DE PISTACA
            }
            
            //std::cout << "Items: " << items.size() << endl;
            for (int i=0;i<items.size();i++)
            {
               if(items.at(i)->getLanzado()){
                    if(items.at(i)->update()){
                        if(strcmp("Escudo", items.at(i)->getNombre()) == 0){
                            pj.at(items.at(i)->getIDPadre())->setProteccion(false);
                        }	

                        if(strcmp("HabilidadPirata", items.at(i)->getNombre()) == 0
                        || strcmp("HabilidadVikingo", items.at(i)->getNombre()) == 0
                        || strcmp("HabilidadGladiador", items.at(i)->getNombre()) == 0
                        || strcmp("HabilidadChino", items.at(i)->getNombre()) == 0){

                            pj.at(items.at(i)->getIDPadre())->setHabilidad(false);
                            
                        }
                        
                        items.at(i)->Delete();	
                        items.erase(items.begin()+i);
                        pistaca->setItems(items);
                        break;
                    }else{
                        items.at(i)->updateHijos();	

                    }
                }
            }
            pistaca->setItems(items);
            
            if (fin_carrera){
            }
            pj = jugadores->getJugadores();
            
            //camara->moveCameraControlServer();
            
            colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
            if (jugadores->getNumJugadores() != 0){
                server->aumentarTimestamp();
                for (int i = 0; i < jugadores->getNumJugadores(); i++) {
                    pj.at(i)->update();
                }
            }

            clock_t tiempoActual = clock();
            clock_t timediff = tiempoActual - tiempoRefrescoServer;
            float timediff_sec = ((float)timediff) / 100000;
            if (timediff_sec >= 0.01) {
                //cout<<"REFRESCO\n";
                server->refreshServer();
                tiempoRefrescoServer = clock();
            }
            
            
            
            if (jugadores->getNumJugadores() != 0){
                if (gc->update())
                    fin_carrera=true;
            }
            
            
            jugadores->setJugadores(pj);
        }
        if(server->getCommands()==1) return 0;
        

        //========================================================
        //ACABA UPDATE
        //========================================================
        //========================================================
        //EMPIEZA DRAW
        //========================================================
        
        GestorJugadores *jugadores = GestorJugadores::getInstancia();
        std::vector<Corredor*> pj = jugadores->getJugadores();

        //------- RENDER ----------
        if (debug) {
            for (int i = 0; i < pj.size(); i++) {
                if (strcmp("JugadorIA", pj.at(i)->getNodo()->getName()) == 0) {
                    CorredorIA *AUXILIAR = static_cast<CorredorIA *> (pj.at(i));
                    AUXILIAR->setDebugFisicas(true);
                    AUXILIAR->ActualizarRaytest();
                }
            }
            MotorFisicas::getInstancia()->getMundo()->debugDrawWorld();
        }
        else {
            for (int i = 0; i < pj.size(); i++) {
                if (strcmp("JugadorIA", pj.at(i)->getNodo()->getName()) == 0) {
                    CorredorIA *AUXILIAR = static_cast<CorredorIA *> (pj.at(i));
                    if (!AUXILIAR->getDebugFisicas()) {
                        i = pj.size();
                    }
                    else {
                        AUXILIAR->setDebugFisicas(false);
                        AUXILIAR->ActualizarRaytest();
                    }
                }
            }
	    }   

        	// ------------------------------
	// -------- IMGUI ---------------
	// ------------------------------
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	Corredor *jugador = GestorJugadores::getInstancia()->getJugadores().at(controlPlayer);
	
	// Mostrar ventanas

	ImGui_ImplGlfwGL3_NewFrame();
	if (show_another_window){
		ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		ImGui::StyleColorsLight(); 
		ImGuiIO& io = ImGui::GetIO();

		if (io.Fonts->Fonts.Size>1){
			ImGui::PushFont(io.Fonts->Fonts[1]);
		}else{
			ImGui::PushFont(io.Fonts->Fonts[0]);

		}
	}else{

		ImGui::StyleColorsClassic();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	}
	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		if (muestraDebug){		
			
			ImGui::Text("Renderizado: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			debugPlot(controlPlayer,ImGui::GetIO().Framerate,"Frames");
		
			ImGui::Text("Debug del Juego!");
			ImGui::Text("Pulsa 9 para activar - 0 desactivar");
			ImGui::Text("Jugadores: %i", GestorJugadores::getInstancia()->getNumJugadores());
			ImGui::Text("Elementos de fisicas: %i", MotorFisicas::getInstancia()->getMundo()->getNumCollisionObjects());

			if (ImGui::SliderFloat("Gravedad", &gravedad, -100.0f, 0.0f, "%.2f", 1.0f))
				MotorFisicas::getInstancia()->getMundo()->setGravity(btVector3(0.0, gravedad, 0.0));

			if (ImGui::Checkbox("Debug Fisicas", &debug))
				TMotor::instancia().setDebugBullet(debug);

			ImGui::Checkbox("Debug Jugador", &debug_Jugador);

			ImGui::Checkbox("Debug IA", &muestraDebugIA);
			if (debug_Jugador) {
				
				ImGui::Begin("Datos del Corredor Jugador", &debug_Jugador);
				ImGui::Text(jugador->toString().c_str());
				debugPlot(controlPlayer,jugador->getVehiculo()->getCurrentSpeedKmHour(),"Velocidad");
				static float fuerza, velocidadMedia, velocidadMaximaTurbo, velocidadMaxima, masa, indiceGiroAlto, indiceGiroBajo, velocidadLimiteGiro;
				jugador->getParametrosDebug(&fuerza, &velocidadMedia, &velocidadMaximaTurbo, &velocidadMaxima, &masa, &indiceGiroAlto, &indiceGiroBajo, &velocidadLimiteGiro);

				ImGui::SliderFloat("fuerza", &fuerza, 1000.0f, 10000.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("velocidadMedia", &velocidadMedia, 100.0f, 800.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("velocidadMaximaTurbo", &velocidadMaximaTurbo, 100.0f, 800.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("velocidadMaxima", &velocidadMaxima, 100.0f, 800.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("Masa", &masa, 0.0f, 8000.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("VelocidadLimiteGiro", &velocidadLimiteGiro, 0.0f, 8000.0f, "%.1f", 100.0f);
				ImGui::SliderFloat("indiceGiroAlto", &indiceGiroAlto, 0.0f, 1.0f, "%.4f", 0.01f);
				ImGui::SliderFloat("indiceGiroBajo", &indiceGiroBajo, 0.0f, 1.0f, "%.4f", 0.01f);
				jugador->setParametrosDebug(fuerza, velocidadMedia, velocidadMaximaTurbo, velocidadMaxima, masa, indiceGiroAlto, indiceGiroBajo, velocidadLimiteGiro);

				static float *posicion = new float[3];
				float *resetOri = new float[3];
				resetOri[0] = jugador->getNodo()->getRotation().z;
				resetOri[1] = jugador->getNodo()->getRotation().y;
				resetOri[2] = jugador->getNodo()->getRotation().x;


				ImGui::SliderFloat3("Posicion", posicion, -100, 100, "%.2f", 1.0f);
				ImGui::SameLine();
				if (ImGui::Button("Set position"))
					jugador->setPosicion(posicion, resetOri);// Hay que pasarle solo la posicion al jugador, no al nodo

				if (ImGui::CollapsingHeader("Ruedas")) {
					static float suspensionStiffness, DampingCompression, DampingRelaxation, frictionSlip, rollInfluence, suspForce, suspTravelCm;
					jugador->getParametrosRuedasDebug(&suspensionStiffness, &DampingCompression, &DampingRelaxation, &frictionSlip, &rollInfluence, &suspForce, &suspTravelCm);
					ImGui::SliderFloat("suspensionStiffness", &suspensionStiffness, 0.0f, 50.0f);
					ImGui::SliderFloat("frictionSlip", &frictionSlip, 1000.0f, 50000.0f, "%.1f", 100.0f);
					ImGui::SliderFloat("rollInfluence", &rollInfluence, 0.0f, 0.1f, "%.3f", 0.001f);
					ImGui::SliderFloat("suspForce", &suspForce, 1000.0f, 50000.0f, "%.1f", 100.0f);
					ImGui::SliderFloat("suspTravelCm", &suspTravelCm, 1000.0f, 50000.0f, "%.1f", 100.0f);
					jugador->setParametrosRuedasDebug(suspensionStiffness, DampingCompression, DampingRelaxation, frictionSlip, rollInfluence, suspForce, suspTravelCm);
					ImGui::Text("DampingCompression: %.3f", DampingCompression);
					ImGui::SameLine();
					ImGui::Text("DampingRelaxation: %.3f", DampingRelaxation);
				}
				
				if (ImGui::Button("Cerrar"))
					debug_Jugador = false;
				ImGui::End();
			}

			if (muestraDebugIA){
				std::vector<Corredor*> pj = GestorJugadores::getInstancia()->getJugadores();
				ImGui::Begin("Datos del Corredor IA", &muestraDebugIA);
				for (int i = 0; i < pj.size(); i++) {
					if (strcmp("JugadorIA", pj.at(i)->getNodo()->getName()) == 0) {
						
						CorredorIA *AUXILIAR = static_cast<CorredorIA *> (pj.at(i));
						ImGui::Text("--------------");
						ImGui::Text("IA %i:",i);
						sr=AUXILIAR->getDebugIA();
						debugRageIA(i);
						
						debugPlot(i,AUXILIAR->getVehiculo()->getCurrentSpeedKmHour(),"Velocidad");
						sr+=AUXILIAR->toString();
						ImGui::Text(sr.c_str());
						
						
					}
				}
		
				ImGui::End();
				sr="";
			}
		}
	}
	
	if (vueltas_aux!=vueltas){
		show_another_window=true;
		muestra_tiempo=t->getTimer();
	}
	if (show_another_window){
			int display_w,display_h;
			glfwGetFramebufferSize( TMotor::instancia().getVentana() , &display_w , &display_h );
			ImGui::SetNextWindowPos(ImVec2((display_w-300)/2, (display_h-500)/2));
			if (vueltas<=3){
				ImGui::SetNextWindowSize( ImVec2( (float)302 , (float)80 ) );
			}else{
				ImGui::SetNextWindowSize( ImVec2( (float)290 , (float)40 ) );
			}
			ImGui::SetNextWindowBgAlpha(0.6f); 
            ImGui::Begin("Another Window", &show_another_window,  ImGuiWindowFlags_NoResize 
			| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard 
			| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal);
			if (vueltas<=3){
				ImGui::Text("Tiempo vuelta: ");
				ImGui::Text(to_string(jugador->getTiempoVuelta()).c_str());
			}else{
				ImGui::Text("Has quedado: ");
				TMotor::instancia().getActiveHud()->traslateElement("puesto", 0.0f, 0.3f);
				muestra_tiempo=t->getTimer();
				//ImGui::Text(to_string(jugador->getPosicionCarrera()).c_str());
			}
			vueltas_aux=vueltas;
			if (t->getTimer()-muestra_tiempo>=4){
				show_another_window = false;
			}
           
            ImGui::End();
			
        }
		ImGui::PopFont();
        //========================================================
        //ACABA DRAW
        //========================================================
        
        if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS){
            server->ShutDownServer();
            Motor3d::instancia().getDevice()->drop();
            return 0;
        }
    }
    server->ShutDownServer();
    
    return 0;
}


void UpdatePhysics(unsigned int TDeltaTime) {
    //TDeltaTime= TDeltaTime*2;
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();
	mundo->stepSimulation(TDeltaTime * 0.001f, 30);
	int c = 0;
	for (int i = 0; i < objetos.size(); i++) {
		c++;
		UpdateRender(objetos.at(i));
	}
}

// Passes bullet's orientation to irrlicht
void UpdateRender(btRigidBody *TObject) {
    obj3D *Node = static_cast<obj3D *>(TObject->getUserPointer());
	// Set position
	btVector3 Point = TObject->getCenterOfMassPosition();

	Pista *mapa = Pista::getInstancia();

	//btTransform t;
	//TObject->getMotionState()->getWorldTransform(t);	
	//Node->setPosition(vector3df(t.getOrigin().getX(),t.getOrigin().getY(),t.getOrigin().getZ()));
	if (strcmp(Node->getName(), "Jugador") == 0 || strcmp(Node->getName(), "JugadorIA") == 0 || strcmp(Node->getName(), "JugadorRed") == 0) {
		//cout << "POSICION: " <<Point[0]<<","<< Point[1]<<","<< Point[2]<<endl; 
		Node->setPosition((float)Point[0], (float)Point[1] + 2, (float)Point[2]);
	}else{

		if(strcmp(Node->getName(), "rueda1") == 0 && strcmp(Node->getName(), "rueda2") == 0 && strcmp(Node->getName(), "rueda3") == 0 &&
		strcmp(Node->getName(), "rueda4") == 0){

		}else{
		
		Node->setPosition((float)Point[0], (float)Point[1], (float)Point[2]);

		}

		}

	// Set rotation
	if(strcmp(Node->getName(), "rueda1") != 0 && strcmp(Node->getName(), "rueda2") != 0 && strcmp(Node->getName(), "rueda3") != 0 &&
		strcmp(Node->getName(), "rueda4") != 0){

	const btQuaternion& TQuat = TObject->getOrientation();
	glm::vec3 axis(TQuat.getAxis().getX(), TQuat.getAxis().getY(), TQuat.getAxis().getZ());
	float angle = TQuat.getAngle() * RADTODEG;
	Node->setRotation(axis, angle);
		
		}
}