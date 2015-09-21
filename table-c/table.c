#include <stdlib.h>
#include <assert.h>
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
		//nodesdelte(t->nodes);
	}
	free(t);
}

static TValue* tgetarray(Table* t, int index) {
	assert(index >= 0);
	if (t->arrsize == 0) {
		t->array = calloc(2, sizeof(Node)); //initial size is 2
		t->arrsize = 2;
		if (t->array == NULL) goto fail;
		return tgetarray(t, index);
	} else if (index >= t->arrsize) {
		TValue* r = (TValue*)realloc(t->array, sizeof(TValue) * t->arrsize * 2);
		if (r == NULL) goto fail;
		t->array = r;
		t->arrsize = t->arrsize * 2;
		return t->array + index;
	} else {
		return t->array + index;
	}
fail:
	return NULL;
}

TValue* tget(Table* t, TValue* key) {
	if(isvali(key) && key->value_.i < t->arrsize) {
		return tgetarray(t, key->value_.i);
	} else if (isvalf(key)) {
		return &(t->nil);
	} else if (isvalstr(key)) {
		return &(t->nil);
	} else if(isvalp(key)) {
		return &(t->nil);
	} else {
		return &(t->nil);
	}
}

void tset(Table* t, TValue* key, TValue* value) {
	if(isvali(key)) {
		TValue* tv = tgetarray(t, getvali(key));
		setvali(tv, getvali(value));
	} else if (isvalf(key)) {
		
	} else if (isvalstr(key)) {
		
	} else if(isvalp(key)) {
		
	} else {
		
	}
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