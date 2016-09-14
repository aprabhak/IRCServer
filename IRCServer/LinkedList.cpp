#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LinkedList.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void llist_init(linkedusers * list)
{
	list->head = NULL;
}

void llist_adduser(linkedusers * list, const char * username, const char * password) {
	// Create new node
	users * n = (users *) malloc(sizeof(users));
	char * tmp = strdup(username);
	char * tmp2 = strdup(password);
	n->username = tmp;
	n->password = tmp2;

	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;
}

void llist2_addroom(linkedrooms * list2,const char * args) {
	rooms * e = (rooms *)malloc(sizeof(rooms));
	char * tmp = strdup(args);
	e->roomname = tmp;
	e->next2 = list2->head2;
	list2->head2 = e;
}

void llist2_printrooms(int fd,linkedrooms * list2) {
	rooms * e;
	char * msg;
	if (list2->head2 == NULL) {
		msg = strdup("EMPTY\r\n");
		printf("room = %s\n", msg);
		write(fd,msg,strlen(msg));
		return;
	}
	e = list2->head2;
	while (e != NULL) {
		msg = strdup(e->roomname);
		printf("room = %s\n",msg);
		strcat(msg,"\r\n");
		write(fd,msg,strlen(msg));
		e = e->next2;
	}
}

void llist2_enterroom(linkedrooms * list2,const char * username,const char * password,const char * args) {
	rooms * e;
	e = list2->head2;
	while (e != NULL) {
		if (strcmp(e->roomname,args) == 0) {
			llist_adduser(&(e->usersinroom),username,password);
		}
		e = e->next2;
	}
}

void llist2_printusersinroom(int fd,linkedrooms * list2,const char * args) {
	rooms * e;
	e = list2->head2;
	while (e != NULL) {
		if (strcmp(e->roomname,args) == 0) {
			llist_printusers(fd,&(e->usersinroom));
		}
		e = e->next2;
	}
}

int llist2_deleteuserinroom(linkedrooms * list2,const char * username,const char * args) {
	rooms * e;
	e = list2->head2;
	while (e != NULL) {
		if (strcmp(e->roomname,args) == 0) {
		    return llist_deleteuser(&(e->usersinroom),username);
		}
		e = e->next2;
	}
	return 0;
}


void llist_printusers(int fd,linkedusers * list) {

	users * e;

	if (list->head == NULL) {
		write(fd,"\r\n",2);
		return;
	}

	//printf("{");

	e = list->head;
	while (e != NULL) {
		//const char * msg = "DENIED\r\n";
	    //write(fd,msg,strlen(msg));
	    char * msg;
	    msg = strdup(e->username);
	    strcat(msg,"\r\n");
	    write(fd,msg,strlen(msg));
		//printf("%s", e->username);
		e = e->next;
		if (e!=NULL) {
			//printf(", ");
		}
	}
	write(fd,"\r\n",2);
	//printf("}\n");
}

void llist2_sendmessage(linkedrooms * list2,const char * username,const char * message,const char * args) {
	rooms * e;
	e = list2->head2;
	while (e != NULL) {
		if (strcmp(e->roomname,args) == 0) {
			e->usermessages[e->messageindex].message = strdup(message);
			e->usermessages[e->messageindex].username = strdup(username);
			(e->messageindex)++;
		}
		e = e->next2;
	}
}

void llist2_getmessages(int fd,linkedrooms * list2,const char * message,const char * args) {
	rooms * e;
	//int i = 0;
	char * msg;
	char * msg2;
	char buffer[50];
	char * msgn = strdup(message);
	int msgno = atoi(msgn);
	int i = msgno;
	e = list2->head2;
	while (e != NULL) {
		if (strcmp(e->roomname,args) == 0) {
			if (i >= e->messageindex) {
				const char * msg3 = "NO-NEW-MESSAGES";
				write(fd,msg3,strlen(msg3));
			}
			while (i < e->messageindex) {
                 
				msg = strdup(e->usermessages[i].username);
				msg2 = strdup(e->usermessages[i].message);
				sprintf(buffer,"%d %s %s\r\n",i,msg,msg2);
				write(fd,(void*)buffer,strlen(buffer));
				i = i + 1;
				/*
				strcat(msg,"\r\n");
				write(fd,msg,strlen(msg));
				msg = strdup(e->usermessages[i].message);
				strcat(msg,"\r\n");
				write(fd,msg,strlen(msg));
				i = i + 1; */

			}
		}
		e = e->next2;
	}
	write(fd,"\r\n",2);

}

int llist_sameuser(linkedusers * list,const char * username) {
	users * e;
	e = list->head;
	while (e != NULL) {
		if (strcmp(e->username,username) == 0) {
			return 1;
		}
		e = e -> next;
	}
	return 0;
}

int llist2_sameuserinroom(linkedrooms * list2,const char * username,const char * args) {
	rooms * e;
  	e = list2->head2;
  	while (e != NULL) {
  		if (strcmp(e->roomname,args) == 0) {
  			return llist_sameuser(&(e->usersinroom),username);
  		}
  		e = e->next2;
  	}
}

int llist2_sameroom(linkedrooms * list2,const char * args) {
	rooms * e;
	e = list2->head2;
	if (e == NULL) {
		return 0;
	}
	while (e != NULL) {
		if (strcmp(e->roomname,args) == 0) {
			return 1;
		}
		e = e -> next2;
	}
	return 0;
} 

int llist_userexist(linkedusers * list, const char * username,const char * password) {
	users * e;
	e = list->head;
	while (e != NULL) {
		if ((strcmp(e->username,username) == 0) && (strcmp(e->password,password) == 0)) {
			return 1;
		}
		e = e -> next;
	}
	return 0;
}

int llist_deleteuser(linkedusers * list,const char * username) {
	users * e = list -> head;
	users *prev = NULL;
	while (e != NULL) {
		if (strcmp(e -> username,username) == 0) {
			break;
		}
		prev = e;
		e = e -> next;
	}
	if (e == NULL) {
		return 0;
	}
	if (prev == NULL) {
		list -> head = e -> next;
	}
	else {
		prev -> next = e -> next;
	}
//	free(e);
	return 1;
}

int llist_number_elementserver(linkedusers * list) {
	users * e;
	int i = 0;
	e = list -> head;
	while (e != NULL) {
		e = e -> next;
		i = i + 1;
	}

	return i;
}

void llist_sortserver(linkedusers * list) {
	int size = llist_number_elementserver(list);
	printf("size = %d",size);
	int i = 0;
	while (i < size) {
		i = i + 1;
		users * current = list->head;
		while (current-> next != NULL) {
			if (strcmp(current->username,current->next->username) > 0) {
				printf("current->username = %s\n",current->username);
				printf("current->next->username = %s\n",current->next->username);
				char * tmp = strdup(current->username);
				current->username = strdup(current->next->username);
				current->next->username = strdup(tmp);
				char * t1 = strdup(current->password);
				current->password = strdup(current->next->password);
				current->next->password = strdup(t1);
			}
			current = current -> next;
		}
	}

}

  void llist2_sortroom(linkedrooms * list2,const char * args) {
  	rooms * e;
  	e = list2->head2;
  	while (e != NULL) {
  		if (strcmp(e->roomname,args) == 0) {
  			llist_sortserver(&(e->usersinroom));
  		}
  		e = e->next2;
  	}
  }

  int list2_roomexist(linkedrooms * list2,const char * args) {
  	rooms * e;
  	e = list2->head2;
  	while (e != NULL) {
  		if (strcmp(e->roomname,args) == 0) {
  			return 1;
  		}
  		e = e->next2;
  	}
  	return 0;	
  }

  int llist2_userexistinroom(linkedrooms * list2,const char * username,const char * password,const char * args) {
  	printf("username = %s\n password = %s\nargs = %s\n",username,password,args);
  	int i = 0;
  	rooms * e;
  	e = list2->head2;
  	while (e != NULL) {
  		printf("e-roomname = %s\n",e->roomname);
  		if (strcmp(e->roomname,args) == 0) {
  			i = llist_userexist(&(e->usersinroom),username,password);
  			printf("i = %d\n",i);
  			if (i == 1) {
  				printf("user exists in room\n");
  			return i;	
  			}
  		}
  		e = e->next2;
  	}
  	printf("user not found,returning %d\n",i);
  	return i;
  }






