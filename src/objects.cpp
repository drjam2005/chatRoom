#include <objects.h>
#include <iostream>
#include <algorithm>
#include <packets.h>
#include <sys/socket.h>
#include <sstream>

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

ClientUI::ClientUI(Client client, int socket_fd) : b1()
{
    user = client;
    users.push_back(user);
    this->socket_fd = socket_fd;

    b1 = Button(
        Rectangle{ 800 - 140.0f, 600 - 65.0f, 75, 30 },
        (char*)"SEND", 5.0f
    );

	b1.SetClick([&] {
		_MESSAGE_PACKET msgPacket;
		msgPacket.client.client_fd = user.client_fd;
		strncpy(msgPacket.client.username, user.username, sizeof(msgPacket.client.username));
		strncpy(msgPacket.message, userMsg.getMsg(), 1024);

		messages.AddMessage(msgPacket);
		sendMessage(msgPacket);
		userMsg.ParseKey(KEY_ENTER);
	});
}

void ClientUI::Render(){
	// render messages	
	DrawRectangle(50, 50, GetScreenWidth()-200, GetScreenHeight()-125, WHITE);
	int index = 0;
	int yPos = 50;
	for(_MESSAGE_PACKET msg : messages.getMessages()){
		bool isUser = false;
		if(msg.client.client_fd == user.client_fd)
			isUser = true;

		messageGUI msgthing(msg.client.username, msg.message);
		msgthing.Render(yPos, 20, isUser);

		//DrawText(TextFormat("(%d) %s: %s", msg.client.client_fd, msg.client.username, msg.message), 60, 60+20*index, 20, color);
		index++;
	}
	index = 0;

	// render user list
	for(Client& usr : users){
		Color clr = BLACK;
		if(usr.client_fd == user.client_fd) clr = Color{0,0,100,255};
		DrawText(TextFormat("(%d) %s",usr.client_fd, usr.username), GetScreenWidth()-140, 60+(20*index), 20, clr);
		index++;
	}
	// render user input field
	DrawRectangle(50, GetScreenHeight()-65, GetScreenWidth()-200, 30, WHITE);
	DrawText(userMsg.getMsg(), 60, GetScreenHeight()-60, 20, BLACK);
	b1.Render();
	b1.Update();
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

void ClientUI::UpdateUserList(Client clnt, PACKET_TYPE pt){
	if(pt == USER_EXISTING || pt == USER_JOIN){
		users.push_back(clnt);
	}if(pt == USER_LEAVE){
		users.erase(std::remove(users.begin(), users.end(), clnt), users.end());
	}
}
void ClientUI::AddMSG(_MESSAGE_PACKET msg){
	messages.AddMessage(msg);
}
void messageField::AddMessage(_MESSAGE_PACKET msg){
	if(messages.size() < 10){
		messages.push_back(msg);
	}else{
		messages.erase(messages.begin());
		messages.push_back(msg);
	}
}

std::vector<_MESSAGE_PACKET> messageField::getMessages(){
	return messages;
}


messageGUI::messageGUI(char* user, char* message){
	strncpy(this->user, user, sizeof(this->user));
	strncpy(this->message, message, sizeof(this->message));
}

void messageGUI::Render(int& yPos, int fontSize, bool isUser){
	Vector2 dimensions = MeasureTextEx(GetFontDefault(), this->message, fontSize, 0);
	std::stringstream ss(this->message);
	std::string str;
	std::string mesg;
	int maxSize = 200;
	int currLen = 0;
	while(ss >> str){
		std::string tempstr = str + " ";
		currLen += MeasureTextEx(GetFontDefault(), tempstr.data(), fontSize, 0).x;
		if(currLen > maxSize){
			mesg += "\n"; currLen = 0;
			mesg += str;
		}else{
			mesg += str;
			mesg += " ";
		}
	}
	dimensions = MeasureTextEx(GetFontDefault(), mesg.data(), fontSize, 0);
	if(isUser){
		DrawRectangle(GetScreenWidth()-200-dimensions.x, yPos+fontSize, dimensions.x+50, dimensions.y, BLUE);
		DrawText(TextFormat("%s", mesg.data()), GetScreenWidth()-200-dimensions.x, yPos+fontSize, fontSize, BLACK);
	}else{
		DrawRectangle(50, yPos+fontSize, dimensions.x+50, dimensions.y, GRAY);
		DrawText(TextFormat("%s:", this->user), 50, yPos, fontSize, BLACK);
		DrawText(TextFormat("%s", mesg.data()), 50, yPos+fontSize, fontSize, BLACK);
		yPos += fontSize;
	}


	yPos += dimensions.y;
}
