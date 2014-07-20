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

void push_stack(const char* stackname, size_t **stack, size_t *stacklen,
	size_t *sp, size_t value)
{
    if(*sp>=*stacklen){
	size_t newlen=*sp+1024;
	size_t *tmp=realloc(*stack, sizeof(size_t)*newlen);
	if(tmp==NULL){
	    perror(stackname);
	    exit(1);
	}
	*stack=tmp;
	*stacklen=newlen;
    }
    (*stack)[(*sp)++]=value;
}

void printstack(size_t *stack, size_t sp, size_t *rstack, size_t rsp)
{
    size_t i;
    for(i=0; i<sp; i++){
	fprintf(stderr, "%zd ", stack[i]);
    }
    fprintf(stderr, "/");
    for(i=rsp; i-->0;){
	fprintf(stderr, " %zd", rstack[i]);
    }
    fprintf(stderr, "\n");
}

void interpret(char* source, size_t srcsize)
{
    size_t pc=0, sp=0, rsp=0;
    size_t *stack=NULL, *rstack=NULL;
    size_t stacklen=1024, rstacklen=1024;
    int ch;

    stack=malloc(sizeof(*stack)*stacklen);
    if(stack==NULL){
	perror("Can't allocate stack memory");
	exit(1);
	return;
    }
    rstack=malloc(sizeof(*rstack)*stacklen);
    if(rstack==NULL){
	perror("Can't allocate rstack memory");
	exit(1);
	return;
    }
    for(pc=0; pc<srcsize; pc++){
	size_t rval;
	switch(source[pc]){
	    case '"':
		if(sp<1){
		    fprintf(stderr, "stack underflow\n");
		    exit(1);
		}
		push_stack("stack", &stack, &stacklen, &sp, stack[sp-1]);
		break;
	    case '+':
		if(sp<1){
		    fprintf(stderr, "stack underflow\n");
		    exit(1);
		}
		stack[sp-1]++;
		break;
	    case '-':
		if(sp<1){
		    fprintf(stderr, "stack underflow\n");
		    exit(1);
		}
		stack[sp-1]--;
		break;
	    case '<':
		if(rsp<1){
		    fprintf(stderr, "rstack underflow\n");
		    exit(1);
		}
		push_stack("stack", &stack, &stacklen, &sp, rstack[--rsp]);
		break;
	    case '>':
		if(sp<1){
		    fprintf(stderr, "stack underflow\n");
		    exit(1);
		}
		push_stack("rstack", &rstack, &rstacklen, &rsp, stack[--sp]);
		break;
	    case '[':
		push_stack("rstack", &rstack, &rstacklen, &rsp, pc);
		break;
	    case ']':
		if(sp<1){
		    fprintf(stderr, "stack underflow\n");
		    exit(1);
		}
		if(rsp<1){
		    fprintf(stderr, "rstack underflow\n");
		    exit(1);
		}
		rval=rstack[--rsp];
		if(stack[--sp]){
		    pc=rval-1;
		}
		break;
	    case '.':
		if(sp<1){
		    fprintf(stderr, "stack underflow\n");
		    exit(1);
		}
		putchar(stack[--sp]);
		break;
	    case ',':
		ch=getchar();
		if(ch==EOF){
		    push_stack("stack", &stack, &stacklen, &sp, 0);
		}else{
		    push_stack("stack", &stack, &stacklen, &sp, ch);
		    push_stack("stack", &stack, &stacklen, &sp, 1);
		}
		break;
#ifdef DEBUG
	    case 's':
		printstack(stack, sp, rstack, rsp);
		break;
#endif
	    default:
		break;
	}
    }
    free(stack);
    free(rstack);
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
