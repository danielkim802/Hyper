V := @
ifeq ($(VERBOSE),1)
	V :=
endif

all:
	$(V) $(MAKE) -C src/virtualmachine
	$(V) cp bin/hyper hyper

debug:
	$(V) $(MAKE) -C src/virtualmachine debug
	$(V) cp bin/hyper hyper
	$(V) cp -r bin/hyper.dSYM hyper.dSYM

clean:
	$(V) rm -rf bin
	$(V) rm -f hyper
	$(V) rm -rf hyper.dSYM
	$(V) rm -f src/compiler/*.pyc