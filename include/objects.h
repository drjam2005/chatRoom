#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <raylib.h>

// forward dec
struct _MESSAGE_PACKET;

struct Client;
struct Client {
	int client_fd = 0;
	char username[256] = {0};
};

class messageBox {
	private:
		char message[1024] = {0};
		int index = 0;
	public:
		void ParseKey(int);
		void AddChar(char chr);
		char* getMsg();
};


struct messageData {
	Client user;
	char message[1024] = {0};
};

class messageField {
private:
	int MAX_LINES = 10;
	std::vector<messageData> messages;
public:
	std::vector<messageData> getMessages();
	messageField(int lines=10){
		MAX_LINES = lines;
		messages.reserve(MAX_LINES);
	}
	
	void AddMessage(_MESSAGE_PACKET msg);
};

class ClientUI {
	private:
		Client user;
		int socket_fd;
		messageBox userMsg;
		messageField messages;
	public:
		ClientUI(Client, int);
		void Render();
		void parseChar();
		void parseKey();
		void AddMSG(_MESSAGE_PACKET);

		void Update();


		void sendMessage(_MESSAGE_PACKET);
};
