DEP = ../mutex

computethreadsig: computethreadsig.c signalthread.c $(DEP)/computethread.c $(DEP)/sharedsum.c $(DEP)/globalerror.c $(DEP)/doneflag.c $(DEP)/randsafe.c $(DEP)/showresults.c
	cc -o computethreadsig computethreadsig.c signalthread.c $(DEP)/computethread.c $(DEP)/sharedsum.c $(DEP)/globalerror.c $(DEP)/doneflag.c $(DEP)/randsafe.c $(DEP)/showresults.c -lm

clean:
	rm computethreadsig