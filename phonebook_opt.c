#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "phonebook_opt.h"

entry hash_table[NUM_OF_DICT];


uint32_t murmur3_32(const char *key, uint32_t len, uint32_t seed)
{
	static const uint32_t c1 = 0xcc9e2d51;
	static const uint32_t c2 = 0x1b873593;
	static const uint32_t r1 = 15;
	static const uint32_t r2 = 13;
	static const uint32_t m = 5;
	static const uint32_t n = 0xe6546b64;

	uint32_t hash = seed;

	const int nblocks = len / 4;
	const uint32_t *blocks = (const uint32_t *) key;
	int i;
	for (i = 0; i < nblocks; i++) {
		uint32_t k = blocks[i];
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;

		hash ^= k;
		hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
	}

	const uint8_t *tail = (const uint8_t *)(key + nblocks * 4);
	uint32_t k1 = 0;

	switch (len & 3) {
	case 3:
		k1 ^= tail[2] << 16;
	case 2:
		k1 ^= tail[1] << 8;
	case 1:
		k1 ^= tail[0];

		k1 *= c1;
		k1 = (k1 << r1) | (k1 >> (32 - r1));
		k1 *= c2;
		hash ^= k1;
	}

	hash ^= len;
	hash ^= (hash >> 16);
	hash *= 0x85ebca6b;
	hash ^= (hash >> 13);
	hash *= 0xc2b2ae35;
	hash ^= (hash >> 16);

	return (hash % NUM_OF_DICT);
}


int findName(char lastname[])
{
	uint32_t key = murmur3_32(lastname, strlen(lastname), 2);
	if (strcasecmp(lastname, hash_table[key].lastName) == 0) {
		return 1;
	} else {
		entry *temp = hash_table[key].pNext;

		if (temp == NULL) {
			return 0;
		}

		while (strcasecmp(lastname, temp->lastName) != 0) {
			if (temp->pNext != NULL) {
				temp = temp->pNext;
			} else {
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

void append(char lastname[]) ///////////////////////////////////////ERROR
{
	uint32_t key = murmur3_32(lastname, strlen(lastname), 2);

	if (strcasecmp(hash_table[key].lastName, "") == 0) {
		strncpy(hash_table[key].lastName , lastname, strlen(lastname));
		hash_table[key].pNext = NULL;
	} else {  //if collision,using single linked-list
		entry *temp = hash_table[key].pNext;
		while (temp != NULL) {
			temp = temp->pNext;
		}
		temp = (entry *) malloc(sizeof(entry));
		strncpy(temp->lastName, lastname, strlen(lastname));
		temp->pNext = NULL;
	}

}

void free_all()
{
	for (int i = 0; i < NUM_OF_DICT; i++) {
		if (!(strcasecmp(hash_table[i].lastName, "") == 0)) { //if the hash_entry is used
			entry* temp = hash_table[i].pNext;
			if (temp != NULL) {
				while (temp->pNext != NULL) {
					entry *temp1 = temp;
					temp = temp->pNext;
					free(temp1);
				}
				free(temp);
			}

		}
	}
}

