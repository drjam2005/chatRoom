#pragma once

#include <string>
#include <raylib.h>
#include <string.h>

struct Client;
struct Message;

struct Client {
	int client_fd = 0;
	char username[256] = {0};
};

struct Message {
	char message[1024] = {0};
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

class ClassUI {
	private:
		messageBox msg;
		std::string nickname;
	public:
		ClassUI(char*);
		void Render();
};
