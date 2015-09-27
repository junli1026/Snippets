#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <assert.h>
#include "table.h"

static void newnodes(Table* t);
static int expendnodes(Table *t);
static void deletenodes(Table* t);

Table* tnew () {
	Table* t = (Table*) calloc(1, sizeof(Table));
	if (t == NULL) {
		return NULL;
	}
	t->nodes = NULL;
	t->nsize = 0;
	t->nuse = 0;
	t->array = NULL;
	t->arrsize = 0;
	t->arruse = 0;
	t->lastfree = NULL;
	return t;
}

void tdelete(Table *t) {
	if(t->array) {
		free(t->array);
	}
	deletenodes(t);
	free(t);
}

//{ nodes
static void newnodes(Table *t) {
	t->nodes = calloc(2, sizeof(Node*));
	t->nsize = 2;
	t->nuse = 0;
	t->lastfree = t->nodes + t->nsize;
}

static void deletenodes(Table* t) {
	if(t->nodes) {
		int i = 0;
		for(i = 0; i < t->nsize; i++) {
			Node *c = t->nodes[i];
			Node *n = c;
			while(n) {
				c = n;
				n = n->next;
				free(c);
			}
		}
	}
}

static int expendnodes(Table* t) {
	t->nodes = realloc(t->nodes, sizeof(Node*) * t->nsize * 2);
	if(t->nodes == NULL) return -1;
	t->nsize *= 2;
	t->lastfree = t->nodes + t->nsize;
	return 0;
}

static void set2freenode(Table *t, Node *node) {
	while(t->lastfree > t->nodes) {
		if(*(t->lastfree) == NULL) {
			*(t->lastfree) = node;
			return;
		}
		t->lastfree --;
	}
	assert(t->lastfree == t->nodes);
	expendnodes(t);
	assert(*(t->lastfree) == NULL);
	*(t->lastfree) = node;
	t->lastfree --;
}
//}

//{ hash
static unsigned long hashtv(TValue *tv) {
	if(isvali(tv)) {
		int i = getvali(tv);
		return (unsigned long)i;
	} else if(isvalf(tv)) {
		double f = getvalf(tv);
		struct t {unsigned long a[2]; } tt;
		memcpy(&tt, &f, sizeof(double));
		return (unsigned long)(tt.a[0] + tt.a[1]);
	} else if(isvalstr(tv)) {
		char *str = getvalstr(tv);
		unsigned long hash = 5381;
    	while (*str) {
        	hash = ((hash << 5) + hash) + *str++; /* hash * 33 + c */
    	}
    	return hash;
	} else if(isvalp(tv)) {
		return 0;
	} else {
		return 0;
	}
}
//}

static TValue* tgetslot(Table* t, TValue* k) {
	if(t->nodes == NULL) {
		newnodes(t);
	}
	int index = hashtv(k) % t->nsize;
	Node *head = t->nodes[index];
	if(head == NULL || hashtv(&(head->key)) % t->nsize == index) {
		//position is not occupied or occupied by node with correct key
		Node *np = head;
		while(np) {
			if (memcmp(&(np->key), k, sizeof(*k)) == 0) return &(np->val);
			np = np->next;
		}
		Node* n = (Node*)calloc(1, sizeof(Node));
		if(n == NULL) return NULL;
		n->next = head;
		n->key = *k;
		t->nodes[index] = n;
		t->nuse++;
		return &(n->val);
	} else { //head is occupied by wrong node
		assert(head->next == NULL);
		Node* n = (Node*)calloc(1, sizeof(Node));
		if(n == NULL) return NULL;
		n->next = NULL;
		n->key = *k;
		t->nodes[index] = n;
		set2freenode(t, head);
		return &(n->val);
	}
}

//{ get value slot
static TValue* tgetbyi(Table* t, int i, TValue* key) {
	if (t->arrsize == 0) {
		t->array = calloc(2, sizeof(Node)); //initial size is 2
		if (t->array == NULL) return tgetslot(t, key);
		t->arrsize = 2;
		t->arruse = 0;
	}
	if (i < t->arrsize * 2) { //array
		if (i >= t->arrsize) {
			TValue* r = (TValue*)realloc(t->array, sizeof(TValue) * t->arrsize * 2);
			if (r == NULL) return NULL;
			t->array = r;
			t->arrsize = t->arrsize * 2;
		}
		return t->array + i;
	} else { //hash
		return tgetslot(t, key);
	}
}
//}

TValue* tget(Table* t, TValue* key) {
	unsigned long hash;
	if(isvali(key)) {
		return tgetbyi(t, key->value_.i, key); 
	} else if (isvalf(key)) {
		return tgetslot(t, key);
	} else if (isvalstr(key)) {
		return tgetslot(t, key);
	} else if(isvalp(key)) {
		return tgetslot(t, key);
	} else {
		return NULL;
	}
}

int tset(Table* t, TValue* key, TValue* value) {
	TValue * tv = tget(t, key);
	if(tv == NULL) return -1;
	*tv = *value;
	return 0;
}

void test() {
	Table* t = tnew();
	TValue key,value, *a;
	
	setvali(&key, 2);
	setvali(&value, 2);
	tset(t, &key, &value);
	a = tget(t, &key);
	assert(2 == getvali(a));
	
	setvalf(&key, 2.3);
	setvalf(&value, 3.3);
	tset(t, &key, &value);
	a = tget(t, &key);
	assert(3.3 == getvalf(a));

	const char* str = "helloworld";
	setvalstr(&key, "helloworld");
	setvalf(&value, 3.12);
	tset(t, &key, &value);
	a = tget(t, &key);
	assert(3.12 == getvalf(a));


	tdelete(t);
	printf("test passed\n");
}

int main() {
	test();
}