V := @
ifeq ($(VERBOSE),1)
	V :=
endif

all:
	$(V) if ! [ -d ../../bin ]; then mkdir ../../bin; fi
	$(V) gcc vmerror.c value.c valuestack.c envstack.c env.c chunk.c garbagecollector.c virtualmachine.c hypercompiler.c main.c -lpython2.7 -o ../../bin/hyper