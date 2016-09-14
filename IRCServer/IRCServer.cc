
const char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                          \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
"                                                               \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "IRCServer.h"

int QueueLength = 5;
	linkedusers list;
	linkedrooms list2;
    //LinkedList roomlists[20];
    //int i = 0;
    //int j = 0;


int
IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);
  
	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1; 
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
			     (char *) &optval, sizeof( int ) );
	
	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			  (struct sockaddr *)&serverIPAddress,
			  sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}
	
	// Put socket in listening mode and set the 
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

void
IRCServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);

	initialize();
	
	while ( 1 ) {
		
		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
					  (struct sockaddr *)&clientIPAddress,
					  (socklen_t*)&alen);
		
		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}
		
		// Process request.
		processRequest( slaveSocket );		
	}
}

int
main( int argc, char ** argv )
{
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}
	
	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;

	// It will never return
	ircServer.runServer(port);
	
}

//
// Commands:
//   Commands are started y the client.
//
//   Request: ADD-USER <USER> <PASSWD>\r\n
//   Answer: OK\r\n or DENIED\r\n
//
//   REQUEST: GET-ALL-USERS <USER> <PASSWD>\r\n
//   Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//
//   REQUEST: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LIST-ROOMS <USER> <PASSWD>\r\n
//   Answer: room1\r\n
//           room2\r\n
//           ...
//           \r\n
//
//   Request: ENTER-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LEAVE-ROOM <USER> <PASSWD>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
//   Answer: OK\n or DENIED\n
//
//   Request: GET-MESSAGES <USER> <PASSWD> <LAST-MESSAGE-NUM> <ROOM>\r\n
//   Answer: MSGNUM1 USER1 MESSAGE1\r\n
//           MSGNUM2 USER2 MESSAGE2\r\n
//           MSGNUM3 USER2 MESSAGE2\r\n
//           ...\r\n
//           \r\n
//
//    REQUEST: GET-USERS-IN-ROOM <USER> <PASSWD> <ROOM>\r\n
//    Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//

void
IRCServer::processRequest( int fd )
{
	// Buffer used to store the comand received from the client
	const int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;
	
	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;
	
	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&
		read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}
		
		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}
	
	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;

	printf("RECEIVED: %s\n", commandLine);

      //printf("The commandLine has the following format:\n");
	//printf("COMMAND <user> <password> <arguments>. See below.\n");
	//printf("You need to separate the commandLine into those components\n");
	//printf("For now, command, user, and password are hardwired.\n");
    /*char * tok = strtok(commandLine," ");
	const char * command = tok;
	tok = strtok(NULL," ");
	const char * user = tok;
    tok = strtok(NULL," ");
	const char * password  = tok;
	tok = strtok(NULL, ".");
	const char * message = tok;
	tok = strtok(NULL, " ");
	const char * args = tok;*/
	char c;
	int i = 0;
	char * command = (char*)malloc(50*sizeof(char));
	char * user = (char*)malloc(50*sizeof(char));
	char * password = (char*)malloc(50*sizeof(char));
	char * args = (char*)malloc(100*sizeof(char));
	char * message = (char*)malloc(100*sizeof(char));
	char * number = (char*)malloc(100*sizeof(char));
	int part = 1;
	int j = 0;
	int x = 0;
	for (i = 0; i < 100; i++) {
		if(commandLine[i] == ' ' && part != 5 ) {
			part++;
			j = 0;
		}
		else if(commandLine[i] == '\r') {
			break;
		}
		else if(part == 1) {
			command[j++] = commandLine[i];
		}
		else if(part == 2) {
			user[j++] = commandLine[i];
		}
		else if(part == 3) {
			password[j++] = commandLine[i];
		}
		else if(part == 4) {
			args[j++] = commandLine[i];
		}
		else if(part == 5) {
			message[j++] = commandLine[i];
		}
		else {
			break;
		}
	}
	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("message=%s\n",message);
	printf("args=%s\n", args);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "CREATE-ROOM")) {
		createRoom(fd,user,password,args);
	}
	else if(!strcmp(command, "LIST-ROOMS")) {
	       listRooms(fd,user,password,args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password,message, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password,message, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else {
		const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	memset(command,0,100);
	memset(user,0,100);
	memset(password,0,100);
	memset(args,0,100);
	memset(message,0,100);

	// Send OK answer
	//const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);	
}

void
IRCServer::initialize()
{
	// Open password file

	// Initialize users in room
	llist_init(&list);

	// Initalize message list

}

bool
IRCServer::checkPassword(int fd, const char * user, const char * password) {
	
    if (llist_userexist(&list,user,password) == 1) {
    	return true;
    } 
    	return false; 
}

void
IRCServer::addUser(int fd, const char * user, const char * password, const char * args)
{
	// Here add a new user. For now always return OK.
	/*char * passcopy;
	passcopy = strdup(password);
	bool e = users.insertItem(user,(void*)passcopy);
	const char * msg =  "OK\r\n";
	write(fd, msg, strlen(msg));
	return;*/
	if (llist_sameuser(&list,user) == 1) {
		const char * msg = "DENIED\r\n";
		write(fd,msg,strlen(msg));
		return;
	}
	llist_adduser(&list,user,password);
	const char * msg = "OK\r\n";
	write(fd,msg,strlen(msg));
	return;		
}

void
IRCServer::enterRoom(int fd, const char * user, const char * password, const char * args)
{   if (llist_userexist(&list,user,password) == 1) {
		if (llist2_sameuserinroom(&list2,user,args) == 1) {
			const char * msg = "OK\r\n";
			write(fd,msg,strlen(msg));
			return;
		}
		if (list2_roomexist(&list2,args) == 0) {
			const char * msg = "ERROR (No room)\r\n";
			write(fd,msg,strlen(msg));
			return;
		}
	llist2_enterroom(&list2,user,password,args);
	const char * msg = "OK\r\n";
	write(fd,msg,strlen(msg));
	} else {
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd,msg,strlen(msg));
		return;
	}
		
}

void
IRCServer::createRoom(int fd, const char * user, const char * password, const char * args) {
	if (llist_userexist(&list,user,password) == 1 && llist2_sameroom(&list2,args) == 0) {
	llist2_addroom(&list2,args);
	const char * msg = "OK\r\n";
	write(fd,msg,strlen(msg));
	} else {
		const char * msg = "DENIED\r\n";
		write(fd,msg,strlen(msg));
		return;
	}
}

void
IRCServer::listRooms(int fd, const char * user, const char * password, const char * args) {
	if (llist_userexist(&list,user,password) == 1) {
	llist2_printrooms(fd,&list2);
	} else {
		const char * msg = "DENIED\r\n";
		write(fd,msg,strlen(msg));
		return;
	}
}

void
IRCServer::leaveRoom(int fd, const char * user, const char * password, const char * args)
{
	if (llist_userexist(&list,user,password) == 1) {
	if(llist2_deleteuserinroom(&list2,user,args) == 1) {
		const char * msg = "OK\r\n";
		write(fd,msg,strlen(msg));
	} else {
		const char * msg = "ERROR (No user in room)\r\n";
		write(fd,msg,strlen(msg));
	}
} else {
	const char * msg = "ERROR (Wrong password)\r\n";
	write(fd,msg,strlen(msg));
}
}

void
IRCServer::sendMessage(int fd, const char * user, const char * password,const char * message, const char * args)
{
	if (llist_userexist(&list,user,password) == 1) {
		if (llist2_userexistinroom(&list2,user,password,args) == 1) {
	llist2_sendmessage(&list2,user,message,args);
	const char * msg = "OK\r\n";
	write(fd,msg,strlen(msg));
  } else {
  	const char * msg = "ERROR (user not in room)\r\n";
  	write(fd,msg,strlen(msg));
  }
   } else {
   	const char * msg = "ERROR (Wrong password)\r\n";
   	write(fd,msg,strlen(msg));
   }

	/*for (j = 0; j<=i;j++) {
		if (llist_checkroom(&roomlists[j],args) == 1) {
			llist_addmessage(&roomlists[j],user,message);
		}
	}
	j = 0; */
}

void
IRCServer::getMessages(int fd, const char * user, const char * password,const char * message, const char * args)
{   
	if (llist_userexist(&list,user,password) == 1) {
		if (llist2_userexistinroom(&list2,user,password,message) == 1) {
			llist2_getmessages(fd,&list2,args,message);
		} else {
			const char * msg = "ERROR (User not in room)\r\n";
			write(fd,msg,strlen(msg));
		}
	//const char * msg = "OK\r\n";
	//write(fd,msg,strlen(msg));
    } else {
    	const char * msg = "ERROR (Wrong password)\r\n";
    	write(fd,msg,strlen(msg));
    }
	
}

void
IRCServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args)
{   if (llist_userexist(&list,user,password) == 1) {
	llist2_sortroom(&list2,args);
	llist2_printusersinroom(fd,&list2,args);
    } else {
    	const char * msg = "ERROR (Wrong password)\r\n";
    	write(fd,msg,strlen(msg));
    }
}

void
IRCServer::getAllUsers(int fd, const char * user, const char * password,const  char * args)
{	      if (checkPassword(fd,user,password) == true) {
		  llist_sortserver(&list);
	      llist_printusers(fd,&list);
	  } else {
	  	const char * msg = "ERROR (Wrong password)\r\n";
	    write(fd,msg,strlen(msg));
	  }
}

