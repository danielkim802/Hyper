import parser
import struct
import time, sys

# opcodes
AND    = 0x00
OR     = 0x01
NOT    = 0x02
LT     = 0x03
LTE    = 0x04
GT     = 0x05
GTE    = 0x06
NE     = 0x07
EQ     = 0x08
ADD    = 0x09
SUB    = 0x0A
MUL    = 0x0B
DIV    = 0x0C
PLUS   = 0x0D
MINUS  = 0x0E
LOAD_INT = 0x12
LOAD_FLOAT = 0x13
LOAD_BOOL = 0x15
LOAD_NULL = 0x16
LOAD_STRING = 0x2B
LOAD_NAME = 0x14
FUN_CALL = 0x0F
GET_ATTR = 0x10
ARR_IDX = 0x11

opcodes = {
	AND        : "and",
	OR         : "or",
	NOT        : "not",
	LT         : "lt",
	LTE        : "lte",
	GT         : "gt",
	GTE        : "gte",
	NE         : "ne",
	EQ         : "eq",
	ADD        : "add",
	SUB        : "sub",
	MUL        : "mul",
	DIV        : "div",
	PLUS       : "plus",
	MINUS      : "minus",
	LOAD_INT   : "load_int",
	LOAD_FLOAT : "load_float",
	LOAD_BOOL  : "load_bool",
	LOAD_NULL  : "load_null",
	LOAD_STRING: "load_string",
	LOAD_NAME  : "load_name",
	FUN_CALL   : "fun_call",
	GET_ATTR   : "get_attr",
	ARR_IDX    : "arr_idx",
}

class ASTTraverser(object):
	def visit(self, node):
		func_name = "visit_" + type(node).__name__[4:]
		func = getattr(self, func_name, self.visit_error)
		return func(node)

	def visit_error(self, node):
		raise Exception("No visit method for " + type(node).__name__)

class Compiler(ASTTraverser):
	def __init__(self, filename):
		text = open(filename, 'r').read()
		self.file = filename
		self.parser = parser.Parser(text)
		self.buffer = bytearray()
		self.line = 1

	def write_cmd(self, cmd, token=None):
		if token is not None:
			self.line = token.line
		self.buffer += chr(cmd)
		self.write_value(parser.INT, self.line)

	def write_value(self, type, obj):
		if type == parser.FLOAT:
			typ = 'd'
		elif type == parser.INT:
			typ = 'd'
		elif type == parser.BOOL:
			typ = 'c'
		elif type == parser.STRING:
			obj += chr(0)
			typ = str(len(obj)) + 's'
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

	def visit_UnaryOp(self, node):
		self.visit(node.right)

		if node.op.type == parser.ADD:
			self.write_cmd(PLUS, node.token)
		if node.op.type == parser.SUB:
			self.write_cmd(MINUS, node.token)

	def visit_Int(self, node):
		self.write_cmd(LOAD_INT, node.token)
		self.write_value(node.token.type, node.value)

	def visit_Float(self, node):
		self.write_cmd(LOAD_FLOAT, node.token)
		self.write_value(node.token.type, node.value)

	def visit_Bool(self, node):
		self.write_cmd(LOAD_BOOL, node.token)
		self.write_value(node.token.type, chr(1) if node.value == 'true' else chr(0))

	def visit_Null(self, node):
		self.write_cmd(LOAD_NULL, node.token)

	def visit_String(self, node):
		self.write_cmd(LOAD_STRING, node.token)
		self.write_value(node.token.type, node.value)

	def visit_Name(self, node):
		self.write_cmd(LOAD_NAME, node.token)
		self.write_value(parser.STRING, node.name)

	def visit_CompBinOp(self, node):
		self.visit(node.left)
		self.visit(node.right)

		if node.op.type == parser.LT:
			self.write_cmd(LT, node.token)
		if node.op.type == parser.LTE:
			self.write_cmd(LTE, node.token)
		if node.op.type == parser.GT:
			self.write_cmd(GT, node.token)
		if node.op.type == parser.GTE:
			self.write_cmd(GTE, node.token)
		if node.op.type == parser.NE:
			self.write_cmd(NE, node.token)
		if node.op.type == parser.EQ:
			self.write_cmd(EQ, node.token)

	def visit_BoolBinOp(self, node):
		self.visit(node.left)
		self.visit(node.right)

		if node.op.type == parser.AND:
			self.write_cmd(AND, node.token)
		if node.op.type == parser.OR:
			self.write_cmd(OR, node.token)

	def visit_BoolUnaryOp(self, node):
		self.visit(node.right)

		if node.op.type == parser.NOT:
			self.write_cmd(NOT, node.token)

	def visit_FunCall(self, node):
		for i in reversed(node.args):
			self.visit(i)
		self.visit(node.fun)
		self.write_cmd(FUN_CALL)
		self.write_value(parser.INT, len(node.args))

	def visit_AttrRef(self, node):
		self.visit(node.obj)
		self.write_cmd(GET_ATTR, node.token)
		self.write_value(parser.STRING, node.ref.name)

	def visit_ArrayIndex(self, node):
		self.visit(node.arr)
		self.visit(node.idx)
		self.write_cmd(ARR_IDX, node.token)

	def compile(self):
		ast = self.parser.parse()
		self.visit(ast)
		file = open(self.file + '.o', 'w')
		file.write(self.buffer)

class Disassembler(object):
	def __init__(self, file):
		file = open(file, 'r').read()
		self.file = bytearray(file)
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

	def get_string(self):
		s = ""
		while not self.eof() and self.byte != chr(0):
			s += self.get()
		if self.eof():
			self.error()
		self.get()
		return s

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

	def read_load_name(self, opcode):
		self.text += self.get_string()

	def read_load_null(self, opcode):
		pass

	def read_load_string(self, opcode):
		self.text += self.get_string()

	def read_lt(self, opcode):
		pass

	def read_lte(self, opcode):
		pass

	def read_gt(self, opcode):
		pass

	def read_gte(self, opcode):
		pass

	def read_ne(self, opcode):
		pass

	def read_eq(self, opcode):
		pass

	def read_plus(self, opcode):
		pass

	def read_minus(self, opcode):
		pass

	def read_not(self, opcode):
		pass

	def read_and(self, opcode):
		pass

	def read_or(self, opcode):
		pass

	def read_fun_call(self, opcode):
		args = self.get_int()
		self.text += str(args)

	def read_get_attr(self, opcode):
		name = self.get_string()
		self.text += name

	def read_arr_idx(self, opcode):
		pass

	def disassemble(self):
		while not self.eof():
			self.read_cmd()
		return self.text

start = time.time()
filename = "script2.txt"
Compiler(filename).compile()
print '[', time.time() - start, ']'
print Disassembler(filename + ".o").disassemble()