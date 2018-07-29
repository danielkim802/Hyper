import sys

# primitive types
INT      = 'INT'
FLOAT    = 'FLOAT'
FUN      = 'FUN'
STRING   = 'STRING'
BOOL     = 'BOOL'

# file
NULL     = 'NULL'
EOF      = 'EOF'

# arithmetic ops
ADD      = 'ADD'
SUB      = 'SUB'
MUL      = 'MUL'
DIV      = 'DIV'

# comparison ops
LT       = 'LT'
LTE      = 'LTE'
GT       = 'GT'
GTE      = 'GTE'
NE       = 'NE'
EQ       = 'EQ'

# syntax
SPACE    = 'SPACE'
NEWLINE  = 'NEWLINE'
NAME     = 'NAME'
LPAREN   = 'LPAREN'
RPAREN   = 'RPAREN'
LBRACE   = 'LBRACE'
RBRACE   = 'RBRACE'
LBRACKET = 'LBRACKET'
RBRACKET = 'RBRACKET'
ASSIGN   = 'ASSIGN'
AASSIGN  = 'AASSIGN'
SASSIGN  = 'SASSIGN'
MASSIGN  = 'MASSIGN'
DASSIGN  = 'DASSIGN'
COMMA    = 'COMMA'
DOT      = 'DOT'
VAR      = 'VAR'
PIPE     = 'PIPE'
USE      = 'USE'
AS       = 'AS'

# control, keywords
IF       = 'IF'
ELIF     = 'ELIF'
ELSE     = 'ELSE'
FOR      = 'FOR'
TO       = 'TO'
WHILE    = 'WHILE'
FUNDEF   = 'FUNDEF'
STRUCTDEF= 'STRUCTDEF'
NOT      = 'NOT'
AND      = 'AND'
OR       = 'OR'
RETURN   = 'RETURN'
PRINT    = 'PRINT'

keywords = {
	"if"      : IF,
	"elif"    : ELIF,
	"else"    : ELSE,
	"for"     : FOR,
	"to"      : TO,
	"while"   : WHILE,
	"fun"     : FUNDEF,
	"struct"  : STRUCTDEF,
	"true"    : BOOL,
	"false"   : BOOL,
	"not"     : NOT,
	"and"     : AND,
	"or"      : OR,
	"return"  : RETURN,
	"print"   : PRINT,
	"null"    : NULL,
	"var"     : VAR,
	"use"     : USE,
	"as"      : AS
}

class Token(object):
	def __init__(self, type, value, line):
		self.type = type
		self.value = value
		self.line = line

	def __repr__(self):
		return 'T(%s, %s)' % (self.type, str(self.value))

class Lexer(object):
	def __init__(self, src):
		self.text = src
		self.pos = 0
		if len(self.text) == 0:
			self.char = None
		else:
			self.char = self.text[self.pos]
		self.line = 1

	def error(self, char, msg):
		print "line %i [SyntaxError]: '%s' %s" % (self.line, char, msg)
		sys.exit(0)

	def get(self):
		char = self.char

		try:
			self.pos += 1
			self.char = self.text[self.pos]
		except IndexError:
			self.char = None

		if char == '\n':
			self.line += 1
		return char

	def peek(self):
		pos = self.pos
		char = self.char
		line = self.line

		self.get()
		res = self.char

		self.pos = pos
		self.char = char
		self.line = line
		return res

	def eof(self):
		return self.char is None

	def is_space(self, char):
		return char != '\n' and char.isspace()

	def is_newline(self, char):
		return char == '\n'

	def get_space(self):
		while not self.eof() and self.is_space(self.char):
			self.get()
		return Token(SPACE, ' ', self.line)

	def get_newline(self):
		self.get()
		return Token(NEWLINE, '\\n', self.line)

	def get_num(self):
		i = ""
		dot = False
		while not self.eof() and (self.char.isdigit() or self.char == '.'):
			if self.char == '.':
				dot = True
			i += self.get()

		if dot:
			try:
				return Token(FLOAT, float(i), self.line)
			except ValueError:
				self.error(self.char, "unexpected token")
		return Token(INT, int(i), self.line)

	def get_name_or_keyword(self):
		global keywords
		res = ""
		while not self.eof() and (self.char.isalnum() or self.char == '_'):
			res += self.get()

		if res in keywords:
			return Token(keywords[res], res, self.line)
		return Token(NAME, res, self.line)

	def get_compop(self):
		if self.peek() == '=':
			return {
				'<=' : Token(LTE, '<=', self.line),
				'>=' : Token(GTE, '>=', self.line),
				'!=' : Token(NE, '!=', self.line),
				'==' : Token(EQ, '==', self.line)
			}[self.get() + self.get()]

		if self.char == '!':
			self.error(self.char, "Invalid syntax")

		return {
			'<' : Token(LT, '<', self.line),
			'>' : Token(GT, '>', self.line)
		}[self.get()]

	def get_binop(self):
		return {
			'+' : Token(ADD, '+', self.line),
			'-' : Token(SUB, '-', self.line),
			'*' : Token(MUL, '*', self.line),
			'/' : Token(DIV, '/', self.line)
		}[self.get()]

	def get_string(self):
		self.get()
		res = ""
		while not self.eof() and self.char != "'":
			res += self.get()

		if self.eof():
			self.error("'", "Incomplete string")

		self.get()
		return Token(STRING, res, self.line)

	def get_token(self):
		if self.char == ';':
			while not self.eof() and self.char != '\n':
				self.get()

		if self.eof():
			return Token(EOF, 'EOF', self.line)
		if self.char.isalpha() or self.char == '_':
			return self.get_name_or_keyword()
		if self.char.isdigit() or self.char == '.' and self.peek() is not None and self.peek().isdigit():
			return self.get_num()
		if self.char == '.':
			self.get()
			return Token(DOT, '.', self.line)
		if self.char == '\\':
			self.get()
			return Token(PIPE, '\\', self.line)
		if self.is_space(self.char):
			return self.get_space()
		if self.is_newline(self.char):
			return self.get_newline()
		if self.char == '=' and self.peek() != '=':
			self.get()
			return Token(ASSIGN, '=', self.line)
		if self.char in ['<', '>', '!', '=']:
			return self.get_compop()
		if self.char in ['+', '-', '*', '/']:
			if self.peek() == '=':
				return {
					'+=' : Token(AASSIGN, '+=', self.line),
					'-=' : Token(SASSIGN, '-=', self.line),
					'*=' : Token(MASSIGN, '*=', self.line),
					'/=' : Token(DASSIGN, '/=', self.line)
				}[self.get() + self.get()]
			return self.get_binop()
		if self.char == '(':
			self.get()
			return Token(LPAREN, '(', self.line)
		if self.char == ')':
			self.get()
			return Token(RPAREN, ')', self.line)
		if self.char == '{':
			self.get()
			return Token(LBRACE, '{', self.line)
		if self.char == '}':
			self.get()
			return Token(RBRACE, '}', self.line)
		if self.char == '[':
			self.get()
			return Token(LBRACKET, '[', self.line)
		if self.char == ']':
			self.get()
			return Token(RBRACKET, ']', self.line)
		if self.char == ',':
			self.get()
			return Token(COMMA, ',', self.line)
		if self.char == '\'':
			return self.get_string()

		self.error(self.char, "Invalid syntax")

	def peek_token(self):
		pos = self.pos
		char = self.char
		line = self.line

		res = self.get_token()

		self.pos = pos
		self.char = char
		self.line = line
		return res

	def peek_2tokens(self):
		pos = self.pos
		char = self.char
		line = self.line

		self.get_token()
		res = self.get_token()

		self.pos = pos
		self.char = char
		self.line = line
		return res

class AST(object):
	pass

class AST_BinOp(AST):
	def __init__(self, left, op, right):
		self.left = left
		self.op = self.token = op
		self.right = right

class AST_UnaryOp(AST):
	def __init__(self, op, right):
		self.op = self.token = op
		self.right = right

class AST_CompBinOp(AST):
	def __init__(self, left, op, right):
		self.left = left
		self.op = self.token = op
		self.right = right

class AST_BoolBinOp(AST):
	def __init__(self, left, op, right):
		self.left = left
		self.op = self.token = op
		self.right = right

class AST_BoolUnaryOp(AST):
	def __init__(self, op, right):
		self.op = self.token = op
		self.right = right

class AST_Pipe(AST):
	def __init__(self, left, op, right):
		self.left = left
		self.op = self.token = op
		self.right = right

class AST_Int(AST):
	def __init__(self, token):
		self.value = token.value
		self.token = token

class AST_Float(AST):
	def __init__(self, token):
		self.value = token.value
		self.token = token

class AST_Bool(AST):
	def __init__(self, token):
		self.value = token.value
		self.token = token

class AST_String(AST):
	def __init__(self, token):
		self.value = token.value
		self.token = token

class AST_Null(AST):
	def __init__(self, token):
		self.value = token.value
		self.token = token

class AST_Name(AST):
	def __init__(self, token):
		self.name = token.value
		self.token = token

	def __repr__(self):
		return "AST_Name(%s)" % self.name

class AST_Assign(AST):
	def __init__(self, var, token, right):
		self.var = var
		self.token = token
		self.right = right

class AST_Declaration(AST):
	def __init__(self, token, var, right):
		self.token = token
		self.var = var
		self.right = right

class AST_If(AST):
	def __init__(self, ifcontent, elifs, elsecontent):
		self.ifcontent = ifcontent     # (token, condition, compound_stmt)
		self.elifcontents = elifs      # array of tuples (token, condition, compound_stmt)
		self.elsecontent = elsecontent # (token, compound_stmt)

class AST_While(AST):
	def __init__(self, token, condition, content):
		self.token = token
		self.condition = condition
		self.content = content

class AST_For(AST):
	def __init__(self, token, var, expr, content):
		self.token = token
		self.var = var
		self.expr = expr
		self.content = content

class AST_FunDef(AST):
	def __init__(self, token, args, stmts):
		self.token = token
		self.args = args
		self.content = stmts

class AST_FunCall(AST):
	def __init__(self, fun, args):
		self.fun = fun
		self.args = args

class AST_StructDef(AST):
	def __init__(self, token, stmts):
		self.token = token
		self.content = stmts

class AST_ArrayDef(AST):
	def __init__(self, token, arr):
		self.token = token
		self.arr = arr

class AST_ArrayIndex(AST):
	def __init__(self, token, arr, idx):
		self.token = token
		self.arr = arr
		self.idx = idx

class AST_AttrRef(AST):
	def __init__(self, obj, token, ref):
		self.obj = obj
		self.token = token
		self.ref = ref

class AST_Return(AST):
	def __init__(self, token, expr):
		self.token = token
		self.expr = expr

class AST_Print(AST):
	def __init__(self, token, expr):
		self.token = token
		self.expr = expr

class AST_Use(AST):
	def __init__(self, token, file, name):
		self.token = token
		self.file = file
		self.name = name

class AST_Statement(AST):
	def __init__(self, stmt):
		self.stmt = stmt

class AST_CompoundStmt(AST):
	def __init__(self):
		self.stmts = []

class AST_EOF(AST):
	def __init__(self, token):
		self.token = token

class AST_Program(AST):
	def __init__(self, content, eof):
		self.content = content
		self.eof = eof

class Parser(object):
	def __init__(self, src, trace=False):
		self.lexer = Lexer(src)
		self.token = self.lexer.get_token()
		self.trace = trace
		self.in_fun = 0

	def error(self, token, msg):
		if token.type == NEWLINE:
			print "line %i [SyntaxError]: %s" % (token.line - 1, "Unexpected end of statement")
		elif token.type == EOF:
			print "line %i [SyntaxError]: %s" % (token.line, "Unexpected end of statement")
		else:
			print "line %i [SyntaxError]: '%s' %s" % (token.line, str(token.value), msg)
		if self.trace:
			raise Exception()
		sys.exit(0)

	def eat(self, type):
		if self.token.type != type:
			self.error(self.token, "Unexpected token")
		self.token = self.lexer.get_token()

	def peek(self):
		return self.lexer.peek_token()

	def peek2(self):
		return self.lexer.peek_2tokens()

	def eat_space(self):
		if self.token.type == SPACE:
			self.eat(SPACE)

	# variable : [SPACE] NAME [SPACE]
	def variable(self):
		self.eat_space()
		res = AST_Name(self.token)
		self.eat(NAME)
		self.eat_space()
		return res

	# fun_def : FUNDEF (SPACE NAME)* (NEWLINE | SPACE)* LBRACE compound_stmt RBRACE
	def fun_def(self):
		token = self.token
		self.eat(FUNDEF)
		args = []
		while self.token.type == SPACE:
			self.eat(SPACE)
			if self.token.type == NAME:
				args += [AST_Name(self.token)]
				self.eat(NAME)
			else:
				break
		self.eat_newline_space()
		self.eat(LBRACE)
		self.in_fun += 1
		stmts = self.compound_stmt()
		self.in_fun -= 1
		self.eat(RBRACE)

		for i in stmts.stmts:
			if type(i) == AST_Return:
				return AST_FunDef(token, args, stmts)
		stmts.stmts += [AST_Return(token, None)]
		return AST_FunDef(token, args, stmts)

	# struct_def : STRUCTDEF (NEWLINE | SPACE)* LBRACE compound_stmt RBRACE
	def struct_def(self):
		token = self.token
		self.eat(STRUCTDEF)
		self.eat_newline_space()
		self.eat(LBRACE)
		stmts = self.compound_stmt()
		self.eat(RBRACE)
		return AST_StructDef(token, stmts)

	# array_def : LBRACKET (NEWLINE | SPACE)* [expr (NEWLINE | SPACE)* (COMMA (NEWLINE | SPACE)* expr (NEWLINE | SPACE)*)*] RBRACKET
	def array_def(self):
		res = AST_ArrayDef(self.token, [])
		self.eat(LBRACKET)
		self.eat_newline_space()

		if self.token.type == RBRACKET:
			self.eat(RBRACKET)
			return res

		res.arr += [self.expr()]
		self.eat_newline_space()

		while self.token.type == COMMA:
			self.eat(COMMA)
			self.eat_newline_space()
			res.arr += [self.expr()]
			self.eat_newline_space()

		self.eat(RBRACKET)
		return res

	# factor : INT | FLOAT | NAME | BOOL | STRING | NULL
	#        | fun_def | struct_def | array_def
	#        | LPAREN expr RPAREN
	def factor(self):
		if self.token.type == BOOL:
			res = AST_Bool(self.token)
			self.eat(BOOL)
		elif self.token.type == NULL:
			res = AST_Null(self.token)
			self.eat(NULL)
		elif self.token.type == STRING:
			res = AST_String(self.token)
			self.eat(STRING)
		elif self.token.type == STRUCTDEF:
			res = self.struct_def()
		elif self.token.type == FUNDEF:
			res = self.fun_def()
		elif self.token.type == LBRACKET:
			res = self.array_def()
		elif self.token.type == NAME:
			res = AST_Name(self.token)
			self.eat(NAME)
		elif self.token.type == INT:
			res = AST_Int(self.token)
			self.eat(INT)
		elif self.token.type == FLOAT:
			res = AST_Float(self.token)
			self.eat(FLOAT)
		elif self.token.type == LPAREN:
			self.eat(LPAREN)
			res = self.expr()
			self.eat(RPAREN)
		else:
			self.error(self.token, "Unexpected token")

		return res

	# array_index : LBRACKET (NEWLINE | SPACE)* expr (NEWLINE | SPACE)* RBRACKET
	def array_index(self):
		self.eat(LBRACKET)
		self.eat_newline_space()
		res = self.expr()
		self.eat_newline_space()
		self.eat(RBRACKET)
		return res

	# get_attr : factor (array_index* DOT factor)* array_index*
	def get_attr(self):
		res = self.factor()
		while self.token.type in [DOT, LBRACKET]:
			while self.token.type == LBRACKET:
				res = AST_ArrayIndex(self.token, res, self.array_index())
			if self.token.type == DOT:
				token = self.token
				self.eat(DOT)
				name = self.factor()
				if type(name) != AST_Name:
					self.error(token, "Invalid attribute")
				res = AST_AttrRef(res, token, name)
			else:
				break
		return res

	# fun_call : [SPACE] get_attr (SPACE get_attr)* [SPACE]
	def fun_call(self):
		self.eat_space()

		res = [self.get_attr()]
		while self.token.type == SPACE:
			self.eat(SPACE)
			if self.token.type in {INT, FLOAT, NAME, FUNDEF, STRUCTDEF, 
			                       LPAREN, LBRACKET, BOOL, STRING, NULL}:
				res += [self.get_attr()]
			else:
				break

		self.eat_space()

		if len(res) == 1:
			return res[0]
		return AST_FunCall(res[0], res[1:])

	# unary_op : [SPACE] (ADD | SUB)* fun_call
	def unary_op(self):
		self.eat_space()
		res = curr = AST_UnaryOp(None, None)
		while self.token.type in {ADD, SUB}:
			token = self.token
			if self.token.type == ADD:
				self.eat(ADD)
			if self.token.type == SUB:
				self.eat(SUB)
			curr.right = AST_UnaryOp(token, None)
			curr = curr.right
		curr.right = self.fun_call()
		return res.right

	# term : unary_op ((MUL | DIV) unary_op)*
	def term(self):
		res = self.unary_op()
		while self.token.type in [MUL, DIV]:
			res = AST_BinOp(res, self.token, None)
			if self.token.type == MUL:
				self.eat(MUL)
			if self.token.type == DIV:
				self.eat(DIV)
			res.right = self.unary_op()
		return res

	# arith_expr : term ((ADD | SUB) term)*
	def arith_expr(self):
		res = self.term()
		while self.token.type in [ADD, SUB]:
			res = AST_BinOp(res, self.token, None)
			if self.token.type == ADD:
				self.eat(ADD)
			if self.token.type == SUB:
				self.eat(SUB)
			res.right = self.term()
		return res

	# comp_expr : arith_expr ((LT | LTE | GT | GTE | NE | EQ) arith_expr)*
	def comp_expr(self):
		res = self.arith_expr()
		ops = {LT, LTE, GT, GTE, NE, EQ}
		while self.token.type in ops:
			res = AST_CompBinOp(res, self.token, None)
			if self.token.type == LT:
				self.eat(LT)
			if self.token.type == LTE:
				self.eat(LTE)
			if self.token.type == GT:
				self.eat(GT)
			if self.token.type == GTE:
				self.eat(GTE)
			if self.token.type == NE:
				self.eat(NE)
			if self.token.type == EQ:
				self.eat(EQ)
			res.right = self.arith_expr()
		return res

	# bool_factor : [SPACE] [NOT] comp_expr
	def bool_factor(self):
		if self.token.type == SPACE:
			self.eat(SPACE)
		if self.token.type == NOT:
			token = self.token
			self.eat(NOT)
			return AST_BoolUnaryOp(token, self.comp_expr())
		return self.comp_expr()

	# bool_term : bool_factor (AND bool_factor)*
	def bool_term(self):
		res = self.bool_factor()
		while self.token.type == AND:
			res = AST_BoolBinOp(res, self.token, None)
			self.eat(AND)
			res.right = self.bool_factor()
		return res

	# bool_expr : bool_term (OR bool_term)* 
	def bool_expr(self):
		res = self.bool_term()
		while self.token.type == OR:
			res = AST_BoolBinOp(res, self.token, None)
			self.eat(OR)
			res.right = self.bool_term()
		return res

	# pipe_expr : bool_expr (PIPE bool_expr)*
	def pipe_expr(self):
		res = self.bool_expr()
		while self.token.type == PIPE:
			res = AST_Pipe(res, self.token, None)
			self.eat(PIPE)
			res.right = self.bool_expr()
		return res

	# expr : pipe_expr
	def expr(self):
		return self.pipe_expr()

	# assign_stmt : attr_ref (ASSIGN | AASSIGN | SASSIGN | MASSIGN | DASSIGN) expr
	def assign_stmt(self):
		expr = self.expr()

		if self.token.type not in [ASSIGN, AASSIGN, SASSIGN, MASSIGN, DASSIGN]:
			return expr

		if type(expr) not in [AST_Name, AST_ArrayIndex, AST_AttrRef]:
			self.error(self.token, "Invalid assignment target")

		token = self.token
		right = None
		if self.token.type == ASSIGN:
			self.eat(ASSIGN)
			right = self.expr()
		if self.token.type == AASSIGN:
			self.eat(AASSIGN)
			right = AST_BinOp(expr, Token(ADD, token.value, token.line), self.expr())
		if self.token.type == SASSIGN:
			self.eat(SASSIGN)
			right = AST_BinOp(expr, Token(SUB, token.value, token.line), self.expr())
		if self.token.type == MASSIGN:
			self.eat(MASSIGN)
			right = AST_BinOp(expr, Token(MUL, token.value, token.line), self.expr())
		if self.token.type == DASSIGN:
			self.eat(DASSIGN)
			right = AST_BinOp(expr, Token(DIV, token.value, token.line), self.expr())

		return AST_Assign(expr, token, right)

	# declaration_stmt : VAR SPACE variable [ASSIGN expr]
	def declaration_stmt(self):
		token = self.token
		self.eat(VAR)
		self.eat(SPACE)
		var = self.variable()

		if self.token.type == ASSIGN:
			self.eat(ASSIGN)
			right = self.expr()
		else:
			right = None

		return AST_Declaration(token, var, right)
	
	def eat_newline_space(self):
		while self.token.type in {NEWLINE, SPACE}:
			if self.token.type == NEWLINE:
				self.eat(NEWLINE)
			if self.token.type == SPACE:
				self.eat(SPACE)

	# eats all newlines and spaces but makes sure to leave a trailing newline
	# when terminating. After this function is called, the possible patterns
	# include:
	#           tokentype
	#           <some other token>
	#           NEWLINE [SPACE] <some other token>
	def eat_newline_space_with_lookahead(self, tokentype):
		while True:
			first = self.token.type
			second = self.peek().type
			third = self.peek2().type

			if first == tokentype:
				break
			if first == SPACE:
				self.eat(SPACE)
				continue
			if (first, second) == (NEWLINE, NEWLINE):
				self.eat(NEWLINE)
				continue
			if (first, second) == (NEWLINE, tokentype):
				self.eat(NEWLINE)
				continue
			if (first, second) == (NEWLINE, SPACE):
				if third in [NEWLINE, tokentype]:
					self.eat(NEWLINE)
					continue
			break

	# if_stmt : IF expr (NEWLINE | SPACE)* LBRACE compound_stmt RBRACE 
	#           ((NEWLINE | SPACE)* ELIF expr (NEWLINE | SPACE)* LBRACE compound_stmt RBRACE)* 
	#           [(NEWLINE | SPACE)* ELSE (NEWLINE | SPACE)* LBRACE compound_stmt RBRACE] [SPACE]
	def if_stmt(self):
		iftoken = self.token
		self.eat(IF)
		ifcond = self.expr()
		self.eat_newline_space()
		self.eat(LBRACE)
		ifcompound_stmt = self.compound_stmt()
		self.eat(RBRACE)

		elifs = []
		while self.token.type in [NEWLINE, SPACE, ELIF]:
			self.eat_newline_space_with_lookahead(ELIF)
			if self.token.type != ELIF:
				break

			eliftoken = self.token
			self.eat(ELIF)
			elifcond = self.expr()
			self.eat_newline_space()
			self.eat(LBRACE)
			elifcompound_stmt = self.compound_stmt()
			self.eat(RBRACE)
			elifs += [(eliftoken, elifcond, elifcompound_stmt)]

		elsestmt = None
		self.eat_newline_space_with_lookahead(ELSE)
		if self.token.type == ELSE:
			elsetoken = self.token
			self.eat(ELSE)
			self.eat_newline_space()
			self.eat(LBRACE)
			elsecompound_stmt = self.compound_stmt()
			self.eat(RBRACE)
			self.eat_space()
			elsestmt = elsetoken, elsecompound_stmt

		self.eat_space()
		return AST_If((iftoken, ifcond, ifcompound_stmt), elifs, elsestmt)

	# return_stmt : RETURN [expr]
	def return_stmt(self):
		if self.in_fun == 0:
			self.error(self.token, "outside of function");
		token = self.token
		self.eat(RETURN)
		self.eat_space()
		if self.token.type in [INT, FLOAT, NAME, BOOL, STRING, NULL, FUNDEF, STRUCTDEF, LBRACKET, LPAREN]:
			expr = self.expr()
			return AST_Return(token, expr)
		return AST_Return(token, None)

	# while_stmt : WHILE expr (NEWLINE | SPACE)* LBRACE compound_stmt RBRACE [SPACE]
	def while_stmt(self):
		token = self.token
		self.eat(WHILE)
		condition = self.expr()
		self.eat_newline_space()
		self.eat(LBRACE)
		content = self.compound_stmt()
		self.eat(RBRACE)
		self.eat_space()
		return AST_While(token, condition, content)

	# for_stmt : FOR SPACE NAME SPACE TO expr (NEWLINE | SPACE)* LBRACE compound_stmt RBRACE [SPACE]
	def for_stmt(self):
		token = self.token
		self.eat(FOR)
		self.eat(SPACE)
		var = AST_Name(self.token)
		self.eat(NAME)
		self.eat(SPACE)
		self.eat(TO)
		expr = self.expr()
		self.eat_newline_space()
		self.eat(LBRACE)
		content = self.compound_stmt()
		self.eat(RBRACE)
		self.eat_space()
		return AST_For(token, var, expr, content)

	# print_stmt : PRINT [expr]
	def print_stmt(self):
		token = self.token
		self.eat(PRINT)
		self.eat_space()
		if self.token.type in [INT, FLOAT, NAME, BOOL, STRING, NULL, FUNDEF, STRUCTDEF, LBRACKET, LPAREN]:
			expr = self.expr()
			return AST_Print(token, expr)
		return AST_Print(token, None)

	# use_stmt : USE SPACE STRING SPACE AS SPACE NAME
	def use_stmt(self):
		token = self.token
		self.eat(USE)
		self.eat(SPACE)
		file = self.token.value
		self.eat(STRING)
		self.eat(SPACE)
		self.eat(AS)
		self.eat(SPACE)
		name = self.token.value
		self.eat(NAME)
		return AST_Use(token, file, name)

	# statement : expr | assign_stmt | if_stmt | return_stmt | while_stmt | for_stmt | print_stmt | declaration_stmt | use_stmt
	def statement(self):
		if self.token.type == IF:
			return self.if_stmt()
		if self.token.type == RETURN:
			return self.return_stmt()
		if self.token.type == WHILE:
			return self.while_stmt()
		if self.token.type == FOR:
			return self.for_stmt()
		if self.token.type == PRINT:
			return self.print_stmt()
		if self.token.type == VAR:
			return self.declaration_stmt()
		if self.token.type == USE:
			return self.use_stmt()
		return self.assign_stmt()

	# compound_stmt : ((statement | [SPACE]) NEWLINE)* [statement | SPACE]
	def compound_stmt(self):
		res = AST_CompoundStmt()

		while self.token.type not in [EOF, RBRACE]:
			if self.token.type == SPACE:
				self.eat(SPACE)
			if self.token.type == NEWLINE:
				self.eat(NEWLINE)
				continue
			if self.token.type in [EOF, RBRACE]:
				break

			res.stmts += [self.statement()]
			if self.token.type == NEWLINE:
				self.eat(NEWLINE)
			else:
				break

		return res
	
	# program : compound_stmt EOF
	def program(self):
		compound = self.compound_stmt()
		eof = AST_EOF(self.token)
		self.eat(EOF)
		return AST_Program(compound, eof)

	def parse(self):
		return self.program()