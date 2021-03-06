import hyperparser
import struct
import sys

# opcodes
# expressions
AND         = 0x00
OR          = 0x01
NOT         = 0x02
LT          = 0x03
LTE         = 0x04
GT          = 0x05
GTE         = 0x06
NE          = 0x07
EQ          = 0x08
ADD         = 0x09
SUB         = 0x0A
MUL         = 0x0B
DIV         = 0x0C
PLUS        = 0x0D
MINUS       = 0x0E
FUN_CALL    = 0x0F
GET_ATTR    = 0x10
ARR_IDX     = 0x11

# literals
LOAD_INT    = 0x12
LOAD_FLOAT  = 0x13
LOAD_NAME   = 0x14
LOAD_BOOL   = 0x15
LOAD_NULL   = 0x16
LOAD_STRING = 0x17
MAKE_FUN    = 0x18
MAKE_STRUCT = 0x19
MAKE_ARR    = 0x1A

# vm functions
PUSH_ENV    = 0x1B
POP_ENV     = 0x1C

# statements
ASSIGN_NAME = 0x1D
STORE_ARR   = 0x1E
STORE_ATTR  = 0x1F
STORE_NAME  = 0x20
RETURN      = 0x21
PRINT       = 0x22
USE_FILE    = 0x23

# control
FOR_SETUP_TO= 0x24
FOR_SETUP_IN= 0x25
FOR_LOOP    = 0x26
FOR_UPDATE  = 0x27
BTRUE       = 0x28
BFALSE      = 0x29
JMP         = 0x2A
HALT        = 0x2B

# library functions
LEN_ARR     = 0x2C
INPUT       = 0x2D
EXIT        = 0x2E

opcodes = {
	# expressions
	AND         : "and",         OR          : "or",          NOT         : "not", 
	LT          : "lt",          LTE         : "lte",         GT          : "gt",  
	GTE         : "gte",         NE          : "ne",          EQ          : "eq",       
	ADD         : "add",         SUB         : "sub",         MUL         : "mul",      
	DIV         : "div",         PLUS        : "plus",        MINUS       : "minus", 
	FUN_CALL    : "fun_call",    GET_ATTR    : "get_attr",    ARR_IDX     : "arr_idx",

	# literals
	LOAD_INT    : "load_int",    LOAD_FLOAT  : "load_float",  LOAD_NAME   : "load_name", 
	LOAD_BOOL   : "load_bool",   LOAD_NULL   : "load_null",   LOAD_STRING : "load_string",
	MAKE_FUN    : "make_fun",    MAKE_STRUCT : "make_struct", MAKE_ARR    : "make_arr",

	# vm functions
	PUSH_ENV    : "push_env",    POP_ENV     : "pop_env",

	# statements
	ASSIGN_NAME : "assign_name", STORE_ARR   : "store_arr",   STORE_ATTR  : "store_attr",
	STORE_NAME  : "store_name",  RETURN      : "return",      PRINT       : "print",
	USE_FILE    : "use_file",

	# control
	FOR_SETUP_TO: "for_setup_to",FOR_SETUP_IN: "for_setup_in",FOR_LOOP    : "for_loop",
	FOR_UPDATE  : "for_update",  BTRUE       : "btrue",       BFALSE      : "bfalse",      
	JMP         : "jmp",         HALT        : "halt",

	# library functions
	LEN_ARR     : "len_arr",     INPUT       : "input",       EXIT        : "exit"
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
		try:
			text = open(filename, 'r').read()
		except IOError:
			print "[IOError]: '%s' No such file" % filename
			sys.exit(0)
		self.file = filename
		self.parser = hyperparser.Parser(text)
		self.buffer = bytearray()
		self.line = 1
		self.warnings = ""

	def error(self, token, msg):
		print "line %i [SyntaxError]: \"%s\" %s" % (token.line, str(token.value), msg)
		sys.exit(0)

	def warning(self, msg):
		self.warnings += "line %i [Warning]: " % self.line + msg + '\n'

	def save(self, type):
		pos = len(self.buffer)
		if type == hyperparser.FLOAT:
			self.buffer += 8 * chr(0)
		elif type == hyperparser.INT:
			self.buffer += 8 * chr(0)
		elif type == hyperparser.BOOL:
			self.buffer += chr(0)
		return pos

	def write_saved(self, position, type, obj):
		if type == hyperparser.FLOAT:
			typ = 'd'
		elif type == hyperparser.INT:
			typ = 'Q'

			if obj > 0xFFFFFFFFFFFFFFFF:
				self.warning("Integer overflow")
			elif obj >= (1 << 63):
				self.warning("Integer overflow")

			obj &= 0xFFFFFFFFFFFFFFFF
		elif type == hyperparser.BOOL:
			typ = 'c'
		elif type == hyperparser.STRING:
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
		self.write_value(hyperparser.INT, self.line)

	def write_value(self, type, obj):
		if type == hyperparser.FLOAT:
			typ = 'd'
		elif type == hyperparser.INT:
			typ = 'Q'

			sign = (obj >> 63) & 1
			newobj = obj & 0xFFFFFFFFFFFFFFFF
			newobj = newobj - 2 ** 64 if sign else newobj
			if obj > 0xFFFFFFFFFFFFFFFF:
				self.warning("Integer overflow (%i -> %i)" % (obj, newobj))
			elif obj >= (1 << 63):
				self.warning("Integer overflow (%i -> %i)" % (obj, newobj))

			obj &= 0xFFFFFFFFFFFFFFFF
		elif type == hyperparser.BOOL:
			typ = 'c'
		elif type == hyperparser.STRING:
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

		if node.op.type == hyperparser.ADD:
			self.write_cmd(ADD, node.token)
		if node.op.type == hyperparser.SUB:
			self.write_cmd(SUB, node.token)
		if node.op.type == hyperparser.MUL:
			self.write_cmd(MUL, node.token)
		if node.op.type == hyperparser.DIV:
			self.write_cmd(DIV, node.token)

	def visit_UnaryOp(self, node):
		self.visit(node.right)

		if node.op.type == hyperparser.ADD:
			self.write_cmd(PLUS, node.token)
		if node.op.type == hyperparser.SUB:
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
		self.write_value(hyperparser.STRING, node.name)

	def visit_CompBinOp(self, node):
		self.visit(node.left)
		self.visit(node.right)

		if node.op.type == hyperparser.LT:
			self.write_cmd(LT, node.token)
		if node.op.type == hyperparser.LTE:
			self.write_cmd(LTE, node.token)
		if node.op.type == hyperparser.GT:
			self.write_cmd(GT, node.token)
		if node.op.type == hyperparser.GTE:
			self.write_cmd(GTE, node.token)
		if node.op.type == hyperparser.NE:
			self.write_cmd(NE, node.token)
		if node.op.type == hyperparser.EQ:
			self.write_cmd(EQ, node.token)

	def visit_BoolBinOp(self, node):
		self.visit(node.left)
		self.visit(node.right)

		if node.op.type == hyperparser.AND:
			self.write_cmd(AND, node.token)
		if node.op.type == hyperparser.OR:
			self.write_cmd(OR, node.token)

	def visit_Pipe(self, node):
		self.visit(node.left)
		self.visit(node.right)

		self.write_cmd(FUN_CALL, node.token)
		self.write_value(hyperparser.INT, 1)
		self.write_value(hyperparser.INT, len(self.buffer) + 8)

	def visit_BoolUnaryOp(self, node):
		self.visit(node.right)

		if node.op.type == hyperparser.NOT:
			self.write_cmd(NOT, node.token)

	def visit_FunCall(self, node):
		for i in reversed(node.args):
			self.visit(i)
		self.visit(node.fun)
		self.write_cmd(FUN_CALL)
		self.write_value(hyperparser.INT, len(node.args))
		self.write_value(hyperparser.INT, len(self.buffer) + 8)

	def visit_AttrRef(self, node):
		self.visit(node.obj)
		self.write_cmd(GET_ATTR, node.token)
		self.write_value(hyperparser.STRING, node.ref.name)

	def visit_ArrayIndex(self, node):
		self.visit(node.arr)
		self.visit(node.idx)
		self.write_cmd(ARR_IDX, node.token)

	def visit_ArrayDef(self, node):
		for i in reversed(node.arr):
			self.visit(i)
		self.write_cmd(MAKE_ARR, node.token)
		self.write_value(hyperparser.INT, len(node.arr))

	def visit_StructDef(self, node):
		self.write_cmd(PUSH_ENV)
		self.visit(node.content)
		self.write_cmd(MAKE_STRUCT, node.token)

	def visit_FunDef(self, node):
		self.write_cmd(MAKE_FUN, node.token)
		addr = self.save(hyperparser.INT)
		self.write_value(hyperparser.INT, len(node.args))
		for i in node.args:
			self.write_value(hyperparser.STRING, i.name)
		self.write_cmd(JMP)
		jmp_addr = self.save(hyperparser.INT)
		self.write_saved(addr, hyperparser.INT, len(self.buffer))
		self.visit(node.content)
		self.write_saved(jmp_addr, hyperparser.INT, len(self.buffer))

	def visit_Assign(self, node):
		self.visit(node.right)
		if type(node.var) == hyperparser.AST_Name:
			self.write_cmd(ASSIGN_NAME, node.token)
			self.write_value(hyperparser.STRING, node.var.name)
		if type(node.var) == hyperparser.AST_ArrayIndex:
			self.visit(node.var.idx)
			self.visit(node.var.arr)
			self.write_cmd(STORE_ARR, node.token)
		if type(node.var) == hyperparser.AST_AttrRef:
			self.visit(node.var.obj)
			self.write_cmd(STORE_ATTR, node.token)
			self.write_value(hyperparser.STRING, node.var.ref.name)

	def visit_Declaration(self, node):
		if node.right is not None:
			self.visit(node.right)

		self.write_cmd(STORE_NAME, node.token)
		self.write_value(hyperparser.STRING, node.var.name)

		if node.right is not None:
			self.write_cmd(ASSIGN_NAME)
			self.write_value(hyperparser.STRING, node.var.name)

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
			self.write_cmd(LOAD_STRING, node.token)
			self.write_value(hyperparser.STRING, '')
		self.write_cmd(PRINT, node.token)

	def visit_If(self, node):
		iftoken, ifcondition, ifcompound = node.ifcontent
		self.visit(ifcondition)
		self.write_cmd(BFALSE, iftoken)
		ifaddr = self.save(hyperparser.INT)
		self.write_cmd(PUSH_ENV, iftoken)
		self.visit(ifcompound)
		self.write_cmd(POP_ENV)
		self.write_cmd(JMP)
		ifjmp = self.save(hyperparser.INT)
		self.write_saved(ifaddr, hyperparser.INT, len(self.buffer))

		elifjmps = []
		for (eliftoken, elifcondition, elifcompound) in node.elifcontents:
			self.visit(elifcondition)
			self.write_cmd(BFALSE, eliftoken)
			elifaddr = self.save(hyperparser.INT)
			self.write_cmd(PUSH_ENV, eliftoken)
			self.visit(elifcompound)
			self.write_cmd(POP_ENV)
			self.write_cmd(JMP)
			elifjmp = self.save(hyperparser.INT)
			elifjmps += [elifjmp]
			self.write_saved(elifaddr, hyperparser.INT, len(self.buffer))

		if node.elsecontent is not None:
			elsetoken, elsecompound = node.elsecontent
			self.write_cmd(PUSH_ENV, elsetoken)
			self.visit(elsecompound)
			self.write_cmd(POP_ENV)

		self.write_saved(ifjmp, hyperparser.INT, len(self.buffer))
		for elifjmp in elifjmps:
			self.write_saved(elifjmp, hyperparser.INT, len(self.buffer))

	def visit_For(self, node):
		# loop setup
		self.write_cmd(PUSH_ENV, node.token)
		if node.token2.type == hyperparser.TO:
			self.visit(node.expr)
			self.visit(node.expr2)
			self.write_cmd(FOR_SETUP_TO)
		if node.token2.type == hyperparser.IN:
			self.write_cmd(LOAD_INT)
			self.write_value(hyperparser.INT, 0)
			self.visit(node.expr)
			self.write_cmd(FOR_SETUP_IN)
		self.write_value(hyperparser.STRING, node.var.name)

		# condition check
		loop_addr = len(self.buffer)
		self.write_cmd(FOR_LOOP)
		self.write_value(hyperparser.STRING, node.var.name)
		end_addr = self.save(hyperparser.INT)

		# body
		self.write_cmd(PUSH_ENV)
		self.visit(node.content)
		self.write_cmd(POP_ENV)

		# increment counter
		self.write_cmd(FOR_UPDATE)
		self.write_value(hyperparser.INT, loop_addr)

		# end of loop
		self.write_saved(end_addr, hyperparser.INT, len(self.buffer))
		self.write_cmd(POP_ENV)

	def visit_While(self, node):
		# condition check
		jmp_addr = len(self.buffer)
		self.visit(node.condition)
		self.write_cmd(BFALSE)
		end_addr = self.save(hyperparser.INT)
		
		# body
		self.write_cmd(PUSH_ENV, node.token)
		self.visit(node.content)
		self.write_cmd(POP_ENV)
		self.write_cmd(JMP)
		self.write_value(hyperparser.INT, jmp_addr)
		
		# end of loop
		self.write_saved(end_addr, hyperparser.INT, len(self.buffer))

	def visit_Use(self, node):
		self.write_cmd(USE_FILE)
		self.write_value(hyperparser.STRING, node.file)
		self.write_value(hyperparser.STRING, node.name)

	def visit_ASM(self, node):
		args = []
		pos = 0
		curr = ''
		instring = False
		while pos < len(node.string):
			while pos < len(node.string) and node.string[pos].isspace():
				pos += 1
			while pos < len(node.string) and (not node.string[pos].isspace() or instring):
				if node.string[pos] == "'":
					instring = not instring
				curr += node.string[pos]
				pos += 1
			if curr:
				args += [curr]
				curr = ''
		if instring:
			self.error(node.token, "Invalid asm string argument")

		if not args:
			self.error(node.token, "Invalid asm statement")

		cmd = args[0].lower()
		args = args[1:]
		types = []
		cond = lambda args: True

		def name(string):
			for i in string:
				if i != '_' and not i.isalnum():
					raise ValueError
			return string

		def string(s):
			if len(s) < 2:
				raise ValueError
			if not (s[0] == s[-1] == "'"):
				raise ValueError
			return s[1:-1]

		def boolean(b):
			if b not in ['1', '0']:
				raise ValueError
			return chr(0) if b == '1' else chr(1)

		if cmd == "and": self.write_cmd(AND, node.token)
		elif cmd == "or": self.write_cmd(OR, node.token)
		elif cmd == "not": self.write_cmd(NOT, node.token)
		elif cmd == "lt": self.write_cmd(LT, node.token)
		elif cmd == "lte": self.write_cmd(LTE, node.token)
		elif cmd == "gt": self.write_cmd(GT, node.token)
		elif cmd == "gte": self.write_cmd(GTE, node.token)
		elif cmd == "ne": self.write_cmd(NE, node.token)
		elif cmd == "eq": self.write_cmd(EQ, node.token)
		elif cmd == "add": self.write_cmd(ADD, node.token)
		elif cmd == "sub": self.write_cmd(SUB, node.token)
		elif cmd == "mul": self.write_cmd(MUL, node.token)
		elif cmd == "div": self.write_cmd(DIV, node.token)
		elif cmd == "plus": self.write_cmd(PLUS, node.token)
		elif cmd == "minus": self.write_cmd(MINUS, node.token)
		elif cmd == "fun_call": self.write_cmd(FUN_CALL, node.token); types = [int, int]
		elif cmd == "get_attr": self.write_cmd(GET_ATTR, node.token); types = [name]
		elif cmd == "arr_idx": self.write_cmd(ARR_IDX, node.token)
		elif cmd == "load_int": self.write_cmd(LOAD_INT, node.token); types = [int]
		elif cmd == "load_float": self.write_cmd(LOAD_FLOAT, node.token); types = [float]
		elif cmd == "load_name": self.write_cmd(LOAD_NAME, node.token); types = [name]
		elif cmd == "load_bool": self.write_cmd(LOAD_BOOL, node.token); types = [boolean]
		elif cmd == "load_null": self.write_cmd(LOAD_NULL, node.token)
		elif cmd == "load_string": self.write_cmd(LOAD_STRING, node.token); types = [string]
		elif cmd == "make_fun": self.write_cmd(MAKE_FUN, node.token); types = [int, int] + [name] * (len(args) - 2); \
			cond = lambda args: args[1] == len(args) - 2
		elif cmd == "make_struct": self.write_cmd(MAKE_STRUCT, node.token)
		elif cmd == "make_arr": self.write_cmd(MAKE_ARR, node.token); types = [int]
		elif cmd == "push_env": self.write_cmd(PUSH_ENV, node.token)
		elif cmd == "pop_env": self.write_cmd(POP_ENV, node.token)
		elif cmd == "assign_name": self.write_cmd(ASSIGN_NAME, node.token); types = [name]
		elif cmd == "store_arr": self.write_cmd(STORE_ARR, node.token)
		elif cmd == "store_attr": self.write_cmd(STORE_ATTR, node.token); types = [name]
		elif cmd == "store_name": self.write_cmd(STORE_NAME, node.token); types = [name]
		elif cmd == "return": self.write_cmd(RETURN, node.token)
		elif cmd == "print": self.write_cmd(PRINT, node.token)
		elif cmd == "use_file": self.write_cmd(USE_FILE, node.token); types = [string, name]
		elif cmd == "for_setup_to": self.write_cmd(FOR_SETUP_TO, node.token); types = [name]
		elif cmd == "for_setup_in": self.write_cmd(FOR_SETUP_IN, node.token); types = [name]
		elif cmd == "for_loop": self.write_cmd(FOR_LOOP, node.token); types = [name, int]
		elif cmd == "for_update": self.write_cmd(FOR_UPDATE, node.token); types = [int]
		elif cmd == "btrue": self.write_cmd(BTRUE, node.token); types = [int]
		elif cmd == "bfalse": self.write_cmd(BFALSE, node.token); types = [int]
		elif cmd == "jmp": self.write_cmd(JMP, node.token); types = [int]
		elif cmd == "halt": self.write_cmd(HALT, node.token)
		elif cmd == "len_arr": self.write_cmd(LEN_ARR, node.token)
		elif cmd == "input": self.write_cmd(INPUT, node.token)
		elif cmd == "exit": self.write_cmd(EXIT, node.token)
		else: self.error(node.token, "Invalid command")

		if len(args) != len(types):
			self.error(node.token, "Invalid number of arguments")

		for i in range(len(args)):
			try:
				args[i] = types[i](args[i])
			except ValueError:
				self.error(node.token, "Invalid argument types")

		if not cond(args):
			self.error(node.token, "Invalid arguments")

		for i in range(len(args)):
			if types[i] == int:
				self.write_value(hyperparser.INT, args[i])
			if types[i] == float:
				self.write_value(hyperparser.FLOAT, args[i])
			if types[i] in [string, name]:
				self.write_value(hyperparser.STRING, args[i])
			if types[i] == boolean:
				self.write_value(hyperparser.BOOL, args[i])

	def visit_EOF(self, node):
		self.write_cmd(HALT, node.token)

	def visit_Program(self, node):
		self.visit(node.content)
		self.visit(node.eof)

	def compile(self):
		ast = self.parser.parse()
		self.visit(ast)
		if self.warnings:
			print self.warnings
		file = open(self.file + 'c', 'w')
		file.write(self.buffer)
		return 0

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

	def get_byte(self):
		return ord(self.get())

	def get_int(self):
		res = bytearray()
		for i in range(8):
			if self.eof():
				self.error()
			res += self.get()

		return struct.unpack('q', res)[0]

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
		cmd = opcodes[self.get_byte()]
		line = self.get_int()
		self.text += "[" + str(pos) + "] " + str(line) + " " + cmd + " "
		getattr(self, "read_" + cmd, self.read_error)()
		self.text += '\n'

	# expressions
	def read_and(self):
		pass

	def read_or(self):
		pass

	def read_not(self):
		pass

	def read_lt(self):
		pass

	def read_lte(self):
		pass

	def read_gt(self):
		pass

	def read_gte(self):
		pass

	def read_ne(self):
		pass

	def read_eq(self):
		pass

	def read_add(self):
		pass

	def read_sub(self):
		pass

	def read_mul(self):
		pass

	def read_div(self):
		pass

	def read_plus(self):
		pass

	def read_minus(self):
		pass

	def read_fun_call(self):
		self.text += str(self.get_int()) + " " + str(self.get_int())

	def read_get_attr(self):
		name = self.get_string()
		self.text += name

	def read_arr_idx(self):
		pass

	# literals
	def read_load_int(self):
		i = self.get_int()
		self.text += str(i)

	def read_load_float(self):
		f = self.get_float()
		self.text += str(f)

	def read_load_name(self):
		self.text += self.get_string()

	def read_load_bool(self):
		b = ord(self.get())
		val = 'true' if b == 1 else 'false'
		self.text += val

	def read_load_null(self):
		pass

	def read_load_string(self):
		self.text += "'" + self.get_string() + "'"

	def read_make_fun(self):
		ptr = self.get_int()
		args = self.get_int()
		names = ""
		for i in range(args):
			names += self.get_string() + " "
		self.text += str(ptr) + " " + str(args) + " " + names

	def read_make_struct(self):
		pass

	def read_make_arr(self):
		length = self.get_int()
		self.text += str(length)

	# vm functions
	def read_push_env(self):
		pass

	def read_pop_env(self):
		pass

	# statements
	def read_assign_name(self):
		self.text += self.get_string()

	def read_store_arr(self):
		pass

	def read_store_attr(self):
		self.text += self.get_string()

	def read_store_name(self):
		self.text += self.get_string()

	def read_return(self):
		pass

	def read_print(self):
		pass

	def read_use_file(self):
		self.text += self.get_string() + " " + self.get_string()

	# control
	def read_btrue(self):
		self.text += str(self.get_int())

	def read_bfalse(self):
		self.text += str(self.get_int())

	def read_jmp(self):
		self.text += str(self.get_int())

	def read_halt(self):
		pass

	# library functions
	def read_len_arr(self):
		pass

	def read_input(self):
		pass

	def disassemble(self):
		while not self.eof():
			self.read_cmd()
		return self.text
