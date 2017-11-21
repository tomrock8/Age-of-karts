class Teclado : public IEventReceiver {
	
private:
	bool KeyDown [KEY_KEY_CODES_COUNT];
public:
	Teclado(){
		for(int i = 0;i<KEY_KEY_CODES_COUNT;i++){
			KeyDown[i] = false;
		}
	}
	virtual bool OnEvent(const SEvent &event){
		switch(event.EventType){
				case irr::EET_KEY_INPUT_EVENT:
				{
					KeyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
				}
				default:
					 break;
		}
		return false;
	}
	virtual bool isKeyDown(EKEY_CODE keyCode) const {
		return KeyDown[keyCode];
	}
	virtual bool isKeyUp(EKEY_CODE keyCode) const {
		return !KeyDown[keyCode];
	}
	
	
	
	
};