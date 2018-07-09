import parser
import struct
import time, sys

# opcodes
AND    = 0x00
OR     = 0x01
ADD    = 0x09
SUB    = 0x0A
MUL    = 0x0B
DIV    = 0x0C
PLUS   = 0x0D
MINUS  = 0x0E
LOAD_INT = 0x12
LOAD_FLOAT = 0x13
LOAD_BOOL = 0x15

opcodes = {
	AND        : "and",
	OR         : "or",
	ADD        : "add",
	SUB        : "sub",
	MUL        : "mul",
	DIV        : "div",
	PLUS       : "plus",
	MINUS      : "minus",
	LOAD_INT   : "load_int",
	LOAD_FLOAT : "load_float",
	LOAD_BOOL  : "load_bool"
}

class ASTTraverser(object):
	def visit(self, node):
		func_name = "visit_" + type(node).__name__[4:]
		func = getattr(self, func_name, self.visit_error)
		return func(node)

	def visit_error(self, node):
		raise Exception("No visit method for " + type(node).__name__)

class Compiler(ASTTraverser):
	def __init__(self, text):
		self.parser = parser.Parser(text)
		self.buffer = bytearray()

	def write_cmd(self, cmd, token):
		self.buffer += chr(cmd)
		self.write_value(token.line)

	def write_value(self, obj):
		if type(obj) == float:
			typ = 'd'
		elif type(obj) == int:
			typ = 'd'
		elif type(obj) == str:
			typ = 'c'
		else:
			typ = None
		self.buffer += struct.pack(typ, obj)

	def visit_CompoundStmt(self, node):
		for stmt in node.stmts:
			self.visit(stmt)

	def visit_BinOp(self, node):
		self.visit(node.left)
		self.visit(node.right)

		if node.op.type == parser.ADD:
			self.write_cmd(ADD, node.token)
		if node.op.type == parser.SUB:
			self.write_cmd(SUB, node.token)
		if node.op.type == parser.MUL:
			self.write_cmd(MUL, node.token)
		if node.op.type == parser.DIV:
			self.write_cmd(DIV, node.token)

	def visit_Int(self, node):
		self.write_cmd(LOAD_INT, node.token)
		self.write_value(node.value)

	def visit_Float(self, node):
		self.write_cmd(LOAD_FLOAT, node.token)
		self.write_value(node.value)

	def visit_Bool(self, node):
		self.write_cmd(LOAD_BOOL, node.token)
		self.write_value(chr(1) if node.value == 'true' else chr(0))

	def compile(self):
		ast = self.parser.parse()
		self.visit(ast)
		return self.buffer

class Disassembler(object):
	def __init__(self, file):
		self.file = file
		self.ptr = 0
		self.byte = chr(self.file[self.ptr])
		self.text = ""

	def error(self):
		print "error disassembling"
		sys.exit(0)

	def read_error(self, opcode):
		raise Exception("No read method for opcode " + hex(opcode))

	def eof(self):
		return self.byte is None

	def get_int(self):
		return int(self.get_float())

	def get_float(self):
		res = bytearray()
		for i in range(8):
			if self.eof():
				self.error()
			res += self.get()

		return struct.unpack('d', res)[0]

	def get(self):
		res = self.byte

		try:
			self.ptr += 1
			self.byte = chr(self.file[self.ptr])
		except IndexError:
			self.byte = None

		return res

	def read_cmd(self):
		global opcodes
		byte = ord(self.get())
		cmd = opcodes[byte]
		line = self.get_int()
		func = getattr(self, "read_" + cmd, self.read_error)
		self.text += str(line) + " " + cmd + " "
		func(byte)
		self.text += '\n'

	def read_add(self, opcode):
		pass

	def read_sub(self, opcode):
		pass

	def read_mul(self, opcode):
		pass

	def read_div(self, opcode):
		pass

	def read_load_int(self, opcode):
		i = self.get_int()
		self.text += str(i)

	def read_load_float(self, opcode):
		f = self.get_float()
		self.text += str(f)

	def read_load_bool(self, opcode):
		b = ord(self.get())
		val = 'true' if b == 1 else 'false'
		self.text += val

	def disassemble(self):
		while not self.eof():
			self.read_cmd()
		return self.text

text = open("script2.txt", "r").read()
start = time.time()
compiled = Compiler(text).compile()
print '[', time.time() - start, ']'
print Disassembler(compiled).disassemble()