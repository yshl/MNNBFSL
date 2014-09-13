#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* readsource(char* filename, size_t* srcsize)
{
    char *source=NULL;
    FILE *fp;

    *srcsize=0;
    if((fp=fopen(filename,"r"))==NULL){
	perror(filename);
	exit(1);
    }
    while(1){
	char buffer[4096];
	size_t readlen;
	char* newsrc;
	if((readlen=fread(buffer,1,sizeof(buffer),fp))==0){
	    if(ferror(fp)){
		perror(filename);
	    }
	    break;
	}
	if((newsrc=realloc(source, *srcsize+readlen))==NULL){
	    perror(filename);
	    break;
	}
	source=newsrc;
	memcpy(source+*srcsize, buffer, readlen);
	*srcsize+=readlen;
    }
    fclose(fp);
    return source;
}

typedef struct {
    const char *name;
    size_t sp;
    size_t length;
    size_t *data;
} Stack;

void Stack_expand(Stack* stack, size_t newlen)
{
    if(newlen>stack->length){
	size_t *tmp=realloc(stack->data, sizeof(*(stack->data))*newlen);
	if(tmp==NULL){
	    perror(stack->name);
	    exit(1);
	}
	stack->data = tmp;
	stack->length = newlen;
    }
}

void Stack_init(Stack* stack, const char* stackname)
{
    stack->name=stackname;
    stack->sp=0;
    stack->length=0;
    stack->data=NULL;
    Stack_expand(stack, 1024);
}

void Stack_delete(Stack* stack)
{
    stack->sp=0;
    stack->length=0;
    free(stack->data);
    stack->data=NULL;
}

void Stack_push(Stack* stack, size_t value)
{
    if(stack->sp >= stack->length){
	Stack_expand(stack, stack->sp+1024);
    }
    stack->data[stack->sp++]=value;
}

size_t Stack_pop(Stack* stack)
{
    if(stack->sp<1){
	fprintf(stderr, "%s underflow\n", stack->name);
	exit(1);
    }
    return stack->data[--(stack->sp)];
}

void Stack_dup(Stack* stack)
{
    Stack_push(stack, stack->data[stack->sp-1]);
}

void Stack_inc(Stack* stack)
{
    if(stack->sp<1){
	fprintf(stderr, "%s underflow\n", stack->name);
	exit(1);
    }
    stack->data[stack->sp-1]++;
}

void Stack_dec(Stack* stack)
{
    if(stack->sp<1){
	fprintf(stderr, "%s underflow\n", stack->name);
	exit(1);
    }
    stack->data[stack->sp-1]--;
}

void printstack(Stack* stack, Stack* rstack)
{
    size_t i;
    for(i=0; i<stack->sp; i++){
	fprintf(stderr, "%zd ", stack->data[i]);
    }
    fprintf(stderr, "/");
    for(i=rstack->sp; i-->0; ){
	fprintf(stderr, " %zd", rstack->data[i]);
    }
    fprintf(stderr, "\n");
}

void interpret(char* source, size_t srcsize)
{
    size_t pc=0;
    Stack stack, rstack;

    Stack_init(&stack, "stack");
    Stack_init(&rstack, "rstack");

    for(pc=0; pc<srcsize; pc++){
	size_t rval;
	int ch;
	switch(source[pc]){
	    case '"':
		Stack_dup(&stack);
		break;
	    case '+':
		Stack_inc(&stack);
		break;
	    case '-':
		Stack_dec(&stack);
		break;
	    case '<':
		Stack_push(&stack, Stack_pop(&rstack));
		break;
	    case '>':
		Stack_push(&rstack, Stack_pop(&stack));
		break;
	    case '[':
		Stack_push(&rstack, pc);
		break;
	    case ']':
		rval=Stack_pop(&rstack);
		if(Stack_pop(&stack)){
		    pc=rval-1;
		}
		break;
	    case '.':
		putchar(Stack_pop(&stack));
		break;
	    case ',':
		ch=getchar();
		if(ch!=EOF){
		    Stack_push(&stack, ch);
		}
		Stack_push(&stack, ch!=EOF);
		break;
#ifdef DEBUG
	    case 's':
		printstack(&stack, &rstack);
		break;
#endif
	    default:
		break;
	}
    }
    Stack_delete(&stack);
    Stack_delete(&rstack);
}

int main(int argc, char **filenames)
{
    char *source;
    size_t srcsize;

    if(argc<=1){
	fprintf(stderr,"Usage: mnnbfsl sourcefile\n");
	exit(1);
    }
    source=readsource(filenames[1],&srcsize);
    interpret(source, srcsize);
    free(source);
    return 0;
}
