#ifndef __TABLE_H__
#define __TABLE_H__

typedef union Value {
	int i;   	//interger
	float f;	//float
	void* p;	//pointer
} Value;

//Tagged value, value with type
#define TValuefields Value value_; int tt_

typedef struct TValue {
	TValuefields;
} TValue;

typedef struct TKey {
	TValuefields;
	struct TKey* next;
} TKey;

#define TypeNil 0
#define TypeInteget 1
#define TypeFloat 2
#define TypeString 4
#define TypePointer 8

#define setvalnil(o) {TValue* v = (o); v->tt_ = TypeNil;}
#define isvalnil(o) ((o)->tt_ == TypeNil)

#define setvali(o,u) {TValue* v = (o); v->tt_ = TypeInteget; v->value_.i = (u);}
#define isvali(o) ((o)->tt_ == TypeInteget)
#define getvali(o) ((o)->value_.i)

#define setvalf(o,u) {TValue* v = (o); v->tt_ = TypeFloat; v->value_.f = (u);}
#define isvalf(o) ((o)->tt_ == TypeFloat)
#define getvalf(o) ((o)->value_.f)

#define setvalstr(o,u) {TValue* v = (o); v->tt_ = TypeString; v->value_.str = (u);}
#define isvalstr(o) ((o)->tt_ == TypeString)
#define getvalstr(o) ((o)->value_.p)

#define setvalp(o,u) {TValue* v = (o); v->tt_ = TypeString; v->value_.p = (u);}
#define isvalp(o) ((o)->tt_ == TypeString)
#define getvalp(o) ((o)->value_.p)

typedef struct Node {
  TValue val;
  TKey key;
} Node;

typedef struct Table {
	TValue *array;
	int arrsize;
	int arruse;
	Node* nodes;
	int nsize;
	int nuse;
	Node* lastfree;
	TValue nil;
} Table;

#endif