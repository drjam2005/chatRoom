#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <raylib.h>

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


class messageData {
	Client user;
	char message[1024] = {0};
};

class messageField {
	std::vector<messageData> messages;
};

class ClientUI {
	private:
		std::string nickname;
		int socket_fd;
		messageBox userMsg;
		messageField messages;
	public:
		ClientUI(char*, int);
		void Render();
		void parseChar();
		void parseKey();

		void sendMessage(char*);
};
