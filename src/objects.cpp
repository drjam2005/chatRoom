#include <objects.h>
#include <iostream>
#include <packets.h>
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

ClientUI::ClientUI(Client client, int socket_fd){
	user = client;
	this->socket_fd = socket_fd;
}

void ClientUI::Render(){
	// render messages	
	DrawRectangle(50, 50, GetScreenWidth()-200, GetScreenHeight()-125, WHITE);
	int index = 0;
	for(messageData msg : messages.getMessages()){
		DrawText(TextFormat("(%d) %s: %s", msg.user.client_fd, msg.user.username, msg.message), 60, 60+20*index, 20, BLACK);
		index++;
	}

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
		_MESSAGE_PACKET msgPacket;
		msgPacket.client.client_fd = user.client_fd;
		strncpy(msgPacket.client.username, user.username, sizeof(msgPacket.client.username));
		strncpy(msgPacket.message, userMsg.getMsg(), 1024);

		messages.AddMessage(msgPacket);
		sendMessage(msgPacket);
	}
	userMsg.ParseKey(chr);
}

void ClientUI::sendMessage(_MESSAGE_PACKET msgPacket){
	PACKET_TYPE pt = MESSAGE;
	send(socket_fd, &pt, sizeof(PACKET_TYPE), 0);
	send(socket_fd, &msgPacket, sizeof(_MESSAGE_PACKET), 0);
}

void ClientUI::Update(){
	parseChar();
	parseKey();
}

void ClientUI::AddMSG(_MESSAGE_PACKET msg){
	messages.AddMessage(msg);
}

void messageField::AddMessage(_MESSAGE_PACKET msg){
	messageData mesg;
	mesg.user = msg.client;
	strncpy(mesg.message, msg.message, sizeof(mesg.message));
	if(messages.size() < 10){
		messages.push_back(mesg);
	}else{
		messages.erase(messages.begin());
		messages.push_back(mesg);
	}
}

std::vector<messageData> messageField::getMessages(){
	return messages;
}
