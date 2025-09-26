#include <objects.h>
#include <iostream>

void messageBox::ParseKey(int key) {
	if(key == KEY_BACKSPACE){
		if(index){
			message[--index] = 0;
		}
	}
	if(key == KEY_ENTER){
		std::cout << message << std::endl;
		memset(message, 0, sizeof(message)); index = 0;
	}
}

void messageBox::AddChar(char chr){
	if(index < 1024)
		message[index++] = chr;
}

char* messageBox::getMsg(){
	return message;
}

ClassUI::ClassUI(char* name){
	nickname = name;
}

void ClassUI::Render(){
	DrawText(msg.getMsg(), 20, 20, 10, WHITE);
}

void ClassUI::parseChar(){
	int chr = GetCharPressed();
	if(chr)
		msg.AddChar(chr);
}
void ClassUI::parseKey(){
	int chr = GetKeyPressed();
	if(chr)
		msg.ParseKey(chr);
}
