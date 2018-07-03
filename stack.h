struct stack{
	int top;
	int *arr;
	int capacity;
};

struct stack *createStack(int capacity){

	struct stack *s = (struct stack *)malloc(sizeof(struct stack *));
	s->capacity = capacity;
	s->arr = (int *)malloc(sizeof(int)*capacity);
	s->top = -1;
	return s;
}

void push(struct stack *s, int v){
	if (s->top > s->capacity){
		s->arr = (int *)realloc(s->arr, s->capacity * 2);
		s->capacity = s->capacity * 2;
	}
	s->arr[++(s->top)] = v;
	return;
}

int pop(struct stack *s){
	if (s->top == -1){
		printf("Stack is Empty\n");
		return -1;
	}
	return s->arr[s->top--];
}