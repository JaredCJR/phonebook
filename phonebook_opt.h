#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H
#define MAX_hash 4294940563  // seed = 2
#define NUM_OF_DICT 349903 //the least prime number biggber than the number of PhoneBook
#define MAX_LAST_NAME_SIZE 16

/* original version */
typedef struct __PHONE_BOOK_ENTRY {
	char lastName[MAX_LAST_NAME_SIZE];
	char firstName[16];
	char email[16];
	char phone[10];
	char cell[10];
	char addr1[16];
	char addr2[16];
	char city[16];
	char state[2];
	char zip[5];
	struct __PHONE_BOOK_ENTRY *pNext;
} entry;

int findName(char lastname[]);
void append(char lastName[]);
void free_all();

#endif

