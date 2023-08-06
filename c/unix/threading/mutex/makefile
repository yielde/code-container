objects=computethread.o doneflag.o randsafe.o showresults.o computethreadmain.o globalerror.o sharedsum.o

comput: $(objects)
	cc -o comput $(objects) -lm

$(objects): globalerror.h

clean:
	rm comput $(objects)