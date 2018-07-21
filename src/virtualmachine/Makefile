V := @
ifeq ($(VERBOSE),1)
	V :=
endif

all:
	$(V) if ! [ -d ../../bin ]; then \
		mkdir ../../bin; \
	fi
	$(V) gcc vmerror.c value.c valuestack.c envstack.c env.c chunk.c \
	garbagecollector.c virtualmachine.c hypercompiler.c main.c \
	-I /System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7 \
	-lpython2.7 -o ../../bin/hyper