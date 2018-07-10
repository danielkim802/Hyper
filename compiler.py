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
MAKE_STRUCT = 0x18
MAKE_ARR = 0x19
PUSH_ENV = 0x1A
POP_ENV = 0x1B
MAKE_FUN = 0x17
JMP = 0x2A
ASSIGN_NAME = 0x1F
STORE_ARR = 0x1D
STORE_ATTR = 0x2D
STORE_NAME = 0x1E
RETURN = 0x2E
PUSH_PC = 0x2F
PRINT = 0x29
BTRUE = 0x27
BFALSE = 0x28
HALT = 0x2C

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
	MAKE_ARR   : "make_arr",
	MAKE_STRUCT: "make_struct",
	PUSH_ENV   : "push_env",
	POP_ENV    : "pop_env",
	MAKE_FUN   : "make_fun",
	JMP        : "jmp",
	ASSIGN_NAME: "assign_name",
	STORE_ARR  : "store_arr",
	STORE_ATTR : "store_attr",
	STORE_NAME : "store_name",
	RETURN     : "return",
	PUSH_PC    : "push_pc",
	PRINT      : "print",
	BTRUE      : "btrue",
	BFALSE     : "bfalse",
	HALT       : "halt",
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

	def save(self, type):
		pos = len(self.buffer)
		if type == parser.FLOAT:
			self.buffer += 8 * chr(0)
		elif type == parser.INT:
			self.buffer += 8 * chr(0)
		elif type == parser.BOOL:
			self.buffer += chr(0)
		return pos

	def write_saved(self, position, type, obj):
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
		bytearr = struct.pack(typ, obj)
		for i in range(len(bytearr)):
			self.buffer[position + i] = bytearr[i]

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
		self.write_cmd(PUSH_PC)
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

	def visit_ArrayDef(self, node):
		for i in reversed(node.arr):
			self.visit(i)
		self.write_cmd(MAKE_ARR, node.token)
		self.write_value(parser.INT, len(node.arr))

	def visit_StructDef(self, node):
		self.write_cmd(PUSH_ENV)
		self.visit(node.content)
		self.write_cmd(MAKE_STRUCT, node.token)
		self.write_cmd(POP_ENV)

	def visit_FunDef(self, node):
		self.write_cmd(MAKE_FUN, node.token)
		addr = self.save(parser.INT)
		self.write_value(parser.INT, len(node.args))
		for i in node.args:
			self.write_value(parser.STRING, i.name)
		self.write_cmd(JMP)
		jmp_addr = self.save(parser.INT)
		self.write_saved(addr, parser.INT, len(self.buffer))
		self.write_cmd(PUSH_ENV)
		self.visit(node.content)
		self.write_saved(jmp_addr, parser.INT, len(self.buffer))

	def visit_Assign(self, node):
		self.visit(node.right)
		if type(node.var) == parser.AST_Name:
			self.write_cmd(ASSIGN_NAME, node.token)
			self.write_value(parser.STRING, node.var.name)
		if type(node.var) == parser.AST_ArrayIndex:
			self.visit(node.var.idx)
			self.visit(node.var.arr)
			self.write_cmd(STORE_ARR, node.token)
		if type(node.var) == parser.AST_AttrRef:
			self.visit(node.var.obj)
			self.write_cmd(STORE_ATTR, node.token)
			self.write_value(parser.STRING, node.var.ref.name)

	def visit_Declaration(self, node):
		if node.right is not None:
			self.visit(node.right)

		self.write_cmd(STORE_NAME, node.token)
		self.write_value(parser.STRING, node.var.name)

		if node.right is not None:
			self.write_cmd(ASSIGN_NAME)
			self.write_value(parser.STRING, node.var.name)

	def visit_Return(self, node):
		if node.expr is not None:
			self.visit(node.expr)
		else:
			self.write_cmd(LOAD_NULL, node.token)
		self.write_cmd(RETURN, node.token)

	def visit_Print(self, node):
		if node.expr is not None:
			self.visit(node.expr)
		else:
			self.write_cmd(LOAD_NULL, node.token)
		self.write_cmd(PRINT, node.token)

	def visit_If(self, node):
		iftoken, ifcondition, ifcompound = node.ifcontent
		self.visit(ifcondition)
		self.write_cmd(BFALSE, iftoken)
		ifaddr = self.save(parser.INT)
		self.write_cmd(PUSH_ENV, iftoken)
		self.visit(ifcompound)
		self.write_cmd(POP_ENV)
		self.write_cmd(JMP)
		ifjmp = self.save(parser.INT)
		self.write_saved(ifaddr, parser.INT, len(self.buffer))

		elifjmps = []
		for (eliftoken, elifcondition, elifcompound) in node.elifcontents:
			self.visit(elifcondition)
			self.write_cmd(BFALSE, eliftoken)
			elifaddr = self.save(parser.INT)
			self.write_cmd(PUSH_ENV, eliftoken)
			self.visit(elifcompound)
			self.write_cmd(POP_ENV)
			self.write_cmd(JMP)
			elifjmp = self.save(parser.INT)
			elifjmps += [elifjmp]
			self.write_saved(elifaddr, parser.INT, len(self.buffer))

		if node.elsecontent is not None:
			elsetoken, elsecompound = node.elsecontent
			self.write_cmd(PUSH_ENV, elsetoken)
			self.visit(elsecompound)
			self.write_cmd(POP_ENV)

		self.write_saved(ifjmp, parser.INT, len(self.buffer))
		for elifjmp in elifjmps:
			self.write_saved(elifjmp, parser.INT, len(self.buffer))

	def visit_EOF(self, node):
		self.write_cmd(HALT, node.token)

	def visit_Program(self, node):
		self.write_cmd(PUSH_ENV)
		self.visit(node.content)
		self.write_cmd(POP_ENV)
		self.visit(node.eof)

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
		pos = self.ptr
		byte = ord(self.get())
		cmd = opcodes[byte]
		line = self.get_int()
		func = getattr(self, "read_" + cmd, self.read_error)
		self.text += "[" + str(pos) + "] " + str(line) + " " + cmd + " "
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

	def read_make_arr(self, opcode):
		length = self.get_int()
		self.text += str(length)

	def read_make_struct(self, opcode):
		pass

	def read_push_env(self, opcode):
		pass

	def read_pop_env(self, opcode):
		pass

	def read_make_fun(self, opcode):
		ptr = self.get_int()
		args = self.get_int()
		names = ""
		for i in range(args):
			names += self.get_string() + " "
		self.text += str(ptr) + " " + str(args) + " " + names

	def read_jmp(self, opcode):
		self.text += str(self.get_int())

	def read_assign_name(self, opcode):
		self.text += self.get_string()

	def read_store_arr(self, opcode):
		pass

	def read_store_attr(self, opcode):
		self.text += self.get_string()

	def read_store_name(self, opcode):
		self.text += self.get_string()

	def read_push_pc(self, opcode):
		pass

	def read_return(self, opcode):
		pass

	def read_print(self, opcode):
		pass

	def read_btrue(self, opcode):
		self.text += str(self.get_int())

	def read_bfalse(self, opcode):
		self.text += str(self.get_int())

	def read_halt(self, opcode):
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