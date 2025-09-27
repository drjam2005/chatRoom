#include <objects.h>
#include <packets.h>
#include <iostream>
#include <sys/socket.h>

void messageBox::ParseKey(int key) {
	if(key == KEY_BACKSPACE){
		if(index){
			message[--index] = 0;
		}
	}
	if(key == KEY_ENTER){
		memset(message, 0, sizeof(message));
		index = 0;
	}
}

void messageBox::AddChar(char chr){
	if(index < 1024)
		message[index++] = chr;
}

char* messageBox::getMsg(){
	return message;
}

ClientUI::ClientUI(char* name, int socket_fd){
	nickname = name;
	this->socket_fd = socket_fd;
}

void ClientUI::Render(){
	// render messages	
	DrawRectangle(50, 50, GetScreenWidth()-200, GetScreenHeight()-125, WHITE);


	// render user field
	DrawRectangle(50, GetScreenHeight()-65, GetScreenWidth()-200, 30, WHITE);
	DrawText(userMsg.getMsg(), 60, GetScreenHeight()-60, 20, BLACK);
}

void ClientUI::parseChar(){
	int chr = GetCharPressed();
	if(chr)
		userMsg.AddChar(chr);
}
void ClientUI::parseKey(){
	int chr = GetKeyPressed();
	if(chr == KEY_ENTER){
		sendMessage(userMsg.getMsg());
	}
	userMsg.ParseKey(chr);
}

void ClientUI::sendMessage(char* message){
	PACKET_TYPE pt = MESSAGE;
	_MESSAGE_PACKET msgPacket;
	strncpy(msgPacket.message, message, 1024);

	send(socket_fd, &pt, sizeof(PACKET_TYPE), 0);
	send(socket_fd, &msgPacket, sizeof(_MESSAGE_PACKET), 0);
}
