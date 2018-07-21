V := @
ifeq ($(VERBOSE),1)
	V :=
endif

all:
	$(V) $(MAKE) -C src/virtualmachine all
	$(V) cp bin/hyper hyper

clean:
	$(V) rm -rf bin
	$(V) rm -f hyper
	$(V) rm -f src/compiler/*.pyc