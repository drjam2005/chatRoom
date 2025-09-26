#include <objects.h>

enum PACKET_TYPE {
	// user joining/leaving
	USER_JOIN,
	USER_EXISTING,
	USER_LEAVE,

	// message handling
	MESSAGE,
};

struct _MESSAGE_PACKET {
	Client client;
	char message[1024] = {0};
};
