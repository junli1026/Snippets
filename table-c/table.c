#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "table.h"

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
	if(t->nodes) {
		int i = 0;
		for(i = 0; i < t->nsize; i++) {
			Node* c = t->nodes[i];
			Node* n = curr;
			while(n) {
				c = n;
				n = n->next;
				free(c);
			}
		}
	}
	free(t);
}

//{ hash
static unsigned long hashpointer(void* p) {
	return 0;
}

static unsigned long hashstr(char* str) {
	unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash; 
}

static unsigned long hashf(double f) {
	struct t {
		unsigned long a;
		unsigned long b;
	} tt;
	memcpy(&tt, p, sizeof(double));
	return (unsigned long)(tt.a + tt.b);
}

static unsigned long hashi(int i) {
	return (unsigned long)i;
}
//}

static TValue* tgetslot(Table* t, int hash, TValue* k) {
	if(t->nodes == NULL) {
		t->nodes = calloc(2, sizeof(Node*));
		t->nsize = 2;
		t->nuse = 0;
	}
	int index = hash % t->nsize;
	Node* curr = t->nodes[index];
	Node* np = curr;
	while(np) {
		if (np->key == *key) return &(np->val);
		np = np->next;
	}
	Node* n = (Node*)calloc(1, sizeof(Node));
	if(n == NULL) return NULL;
	n->next = curr;
	n->key = *k;
	t->nodes[index] = n;
	return &(n->val);
}

//{ get value slot
static TValue* tgetbyi(Table* t, int i) {
	if (t->arrsize == 0) {
		t->array = calloc(2, sizeof(Node)); //initial size is 2
		if (t->array == NULL) return tgetslot(t, hashi(i));
		t->arrsize = 2;
		t->arruse = 0;
	}
	if (i < t->arrsize * 2) { //array
		if (i >= t->arrsize) {
			TValue* r = (TValue*)realloc(t->array, sizeof(TValue) * t->arrsize * 2);
			if (r == NULL) goto fail;
			t->array = r;
			t->arrsize = t->arrsize * 2;
		}
		return t->array + i;
	} else { //hash
		return tgetslot(t, hashi(i));
	}
}

//}

TValue* tget(Table* t, TValue* key) {
	unsigned long hash;
	if(isvali(key)) {
		return tgetbyi(Table* t, key->value_.i); 
	} else if (isvalf(key)) {
		hash = hashf(key->value_.f);
		return tgetslot(t, hash, key);
	} else if (isvalstr(key)) {
		hash = hashstr(key->value_.str);
		return tgetslot(t, hash, key);
	} else if(isvalp(key)) {
		hash = hashstr(key->value_.p);
		return tgetslot(t, hash, key);
	} else {
		return NULL;
	}
}

int tset(Table* t, TValue* key, TValue* value) {
	TValue * tv = tget(t, key);
	if(tv == NULL) return -1;
	tv = *value;
	return 0;
}

void test() {
	Table* t = tnew();

	TValue key,value;
	setvali(&key, 2);
	setvali(&value, 2);
	tset(t, &key, &value);

	TValue* a = tget(t, &key);
	assert(2 == getvali(a));
	tdelete(t);
}

int main() {
	test();
}