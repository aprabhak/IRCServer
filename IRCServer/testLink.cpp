#include <stdio.h>
#include "LinkedList.h"

int main() {
	LinkedList list;

	llist_init(&list);

	llist_print(&list);

	llist_adduser(&list,"Sam");
	llist_adduser(&list,"Tim");

	llist_print(&list);
}
