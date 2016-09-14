
#ifndef IRC_SERVER
#define IRC_SERVER
#include "HashTableVoid.h"
#include "LinkedList.h"
#define PASSWORD_FILE "password.txt"

/*typedef struct users {
	char * names[100];
	int size;
	int maxsize;
} users;
*/
class IRCServer {
	// Add any variables you need
	//HashTableVoid users;
	//Room* rooms[100];
	//HashTableVoid rooms;


private:
	int open_server_socket(int port);

public:
	void initialize();
	bool checkPassword(int fd, const char * user, const char * password);
	void processRequest( int socket );
	void addUser(int fd, const char * user, const char * password, const char * args);
	void enterRoom(int fd, const char * user, const char * password, const char * args);
	void createRoom(int fd,const char * user, const char * password, const char * args);
	void listRooms(int fd, const char * user, const char * password, const char * args);
	void leaveRoom(int fd, const char * user, const char * password, const char * args);
	void sendMessage(int fd, const char * user, const char * password,const char * message,const char * args);
	void getMessages(int fd, const char * user, const char * password,const char * message, const char * args);
	void getUsersInRoom(int fd, const char * user, const char * password, const char * args);
	void getAllUsers(int fd, const char * user, const char * password, const char * args);
	void runServer(int port);
};

#endif
