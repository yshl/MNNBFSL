import std.stdio;
import std.file;

struct Stack{
    private:
	string name;
	size_t[] data;
	size_t sp;

    public:
	this(string stackname){
	    name=stackname;
	    data.length=1024;
	    sp=0;
	}
	void push(size_t value){
	    if(sp>=data.length){
		data.length+=1024;
	    }
	    data[sp++]=value;
	}
	auto pop(){
	    if(sp<1) throw new Exception(name~" underflow");
	    return data[--sp];
	}
	void dup(){
	    if(sp<1) throw new Exception(name~" underflow");
	    push(data[sp-1]);
	}
	void inc(){
	    if(sp<1) throw new Exception(name~" underflow");
	    data[sp-1]++;
	}
	void dec(){
	    if(sp<1) throw new Exception(name~" underflow");
	    data[sp-1]--;
	}
	void print(){
	    foreach(d; data[0..sp]){
		stderr.writef("%s ", d);
	    }
	}
	void rprint(){
	    foreach_reverse(d; data[0..sp]){
		stderr.writef(" %s", d);
	    }
	}
}

void printstack(Stack stack, Stack rstack)
{
    stack.print;
    stderr.write("/");
    rstack.rprint;
    stderr.writeln;
}

void interpret(const ubyte[] source)
{
    auto stack=Stack("stack");
    auto rstack=Stack("rstack");
    size_t pc;
    try{
	for(pc=0; pc<source.length; pc++){
	    size_t s;
	    int ch;
	    switch(source[pc]){
		case '"':
		    stack.dup;
		    break;
		case '+':
		    stack.inc;
		    break;
		case '-':
		    stack.dec;
		    break;
		case '<':
		    stack.push(rstack.pop);
		    break;
		case '>':
		    rstack.push(stack.pop);
		    break;
		case '[':
		    rstack.push(pc);
		    break;
		case ']':
		    s=rstack.pop;
		    if(stack.pop){
			pc=s-1;
		    }
		    break;
		case '.':
		    putchar(cast(int)stack.pop);
		    break;
		case ',':
		    ch=getchar;
		    if(ch!=EOF){
			stack.push(ch);
		    }
		    stack.push(ch!=EOF);
		    break;
		case 's':
		    printstack(stack, rstack);
		    break;
		default:
		    break;
	    }
	}
    }catch(Exception ex){
	stderr.writefln("%s at %s", ex.msg, pc);
    }
}

void main(string[] filenames)
{
    if(filenames.length<=1){
	stderr.writeln("Usage: mnnbfsl sourcefile");
	return;
    }
    auto source=cast(ubyte[]) read(filenames[1]);
    interpret(source);
}
