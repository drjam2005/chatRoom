#include <objects.h>

void messageBox::ParseKey(int key) {
	if(key == KEY_BACKSPACE){
		if(index){
			message[index--] = 0;
		}
	}
	if(key == KEY_ENTER){
		memset(message, 0, sizeof(message));
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

