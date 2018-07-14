import compiler
import sys

if len(sys.argv) < 2:
	print "usage: hyper [filename]"
	sys.exit(0)

compiler.Compiler(sys.argv[1]).compile()
