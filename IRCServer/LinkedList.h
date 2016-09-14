
// Data structs for a list of ints

struct users {
	const char * username;
	const char * password;
	//const char * message[100];
    //users * names;
	struct users * next;
	//int messageindex;
	//const char * roomname;
};

typedef struct users users;

struct linkedusers {
	users * head;
};

typedef struct linkedusers linkedusers;

struct messages {
	const char * message;
	const char * username;
};

typedef struct messages messages;

struct rooms {
	int messageindex = 0;
	const char * roomname;
	struct rooms * next2;
	linkedusers usersinroom;
	messages usermessages[100];
};

typedef struct rooms rooms;

struct linkedrooms {
	rooms * head2;
};

typedef struct linkedrooms linkedrooms;

//LinkedList roomlists[20];
//int i = 0;

/*struct users {
 char * username;
 char * message;
}; */

//typedef struct users users;

void llist_init(linkedusers * list);
//void llist_print(LinkedList * list);
void llist_adduser(linkedusers * list, const char * username, const char * password);
int llist_sameuser(linkedusers * list,const char * username);
void llist_printusers(int fd,linkedusers * list);
//void llist_adduser(LinkedList * list, char * roomname,);
int llist_userexist(linkedusers * list,const char * username,const char * password);
int llist2_sameroom(linkedrooms * list2,const char * args);
void llist2_addroom(linkedrooms * list2,const char * args);
void llist2_printrooms(int fd,linkedrooms * list2);
//void llist_addroomname(LinkedList * list,const char * roomname);
void llist2_enterroom(linkedrooms * list2,const char * username,const char * password,const char * args);
void llist2_printusersinroom(int fd,linkedrooms * list2,const char * args);
int llist_deleteuser(linkedusers * list,const char * username);
int llist2_deleteuserinroom(linkedrooms * list2,const char * username,const char * args);
//int llist_checkroom(LinkedList * list,const char * roomname);
void llist2_sendmessage(linkedrooms * list2,const char * username,const char * message,const char * args);
void llist2_getmessages(int fd,linkedrooms * list2,const char * args,const char * messages);
int llist_number_elementserver(linkedusers * list);
void llist_sortserver(linkedusers * list);
void llist2_sortroom(linkedrooms * list2,const char * args);
int llist2_userexistinroom(linkedrooms * list2,const char * username,const char * password,const char * args);
int llist2_sameuserinroom(linkedrooms * list2,const char * username,const char * args);
int list2_roomexist(linkedrooms * list2,const char * args);
//void llist_printrooms(LinkedList ** roomlists,int i);
//int llist_deleteuser(LinkedList * list,const char * username);
//void llist_addmessage(LinkedList * list,const char * username,const char * message);
//void llist_printmessages(LinkedList * list,const char * username);
//int llist_removeuser(LinkedList * list, char * roomname); // Can this be done in non-object oriented way
//int llist_get_ith(LinkedList * list, int ith, int *value);
//int llist_remove_ith(LinkedList * list, int ith);
//int llist_number_elements(LinkedList * list);
//int llist_save(LinkedList * list, char * file_name);
//int llist_read(LinkedList * list, char * file_name);
//void llist_sort(LinkedList * list, int ascending);
//void llist_clear(LinkedList *list);

//int llist_remove_first(LinkedList * list, int * value);
//int llist_remove_last(LinkedList * list, int * value);
//void llist_insert_first(LinkedList * list, int value);
//void llist_insert_last(LinkedList * list, int value);
