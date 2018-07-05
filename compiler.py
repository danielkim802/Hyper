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

# control, keywords
IF       = 'IF'
ELIF     = 'ELIF'
ELSE     = 'ELSE'
FOR      = 'FOR'
WHILE    = 'WHILE'
FUNDEF   = 'FUNDEF'

keywords = {
	"if"    : IF,
	"elif"  : ELIF,
	"else"  : ELSE,
	"for"   : FOR,
	"while" : WHILE,
	"fun"   : FUNDEF
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
		self.char = self.text[self.pos]
		self.line = 1

	def error(self, char, msg):
		print "line %i: '%s' %s" % (self.line, char, msg)
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
		try:
			return self.char[self.pos + 1]
		except IndexError:
			return None

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
				self.syntax_error("unexpected token %s" % self.char)
		return Token(INT, int(i), self.line)

	def get_name_or_keyword(self):
		global keywords
		res = ""
		while not self.eof() and (self.char.isalnum() or self.char == '_'):
			res += self.get()

		if res in keywords:
			return Token(keywords[res], res, self.line)
		return Token(NAME, res, self.line)

	def get_binop(self):
		return {
			'+' : Token(ADD, '+', self.line),
			'-' : Token(SUB, '-', self.line),
			'*' : Token(MUL, '*', self.line),
			'/' : Token(DIV, '/', self.line)
		}[self.get()]

	def get_token(self):
		if self.eof():
			return Token(EOF, 'EOF', self.line)
		if self.char.isalpha() or self.char == '_':
			return self.get_name_or_keyword()
		if self.char.isdigit() or self.char == '.':
			return self.get_num()
		if self.is_space(self.char):
			return self.get_space()
		if self.is_newline(self.char):
			return self.get_newline()
		if self.char == '=':
			self.get()
			return Token(ASSIGN, '=', self.line)
		if self.char in ['+', '-', '*', '/']:
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

class AST_Int(AST):
	def __init__(self, token):
		self.value = token.value
		self.token = token

class AST_Float(AST):
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

class AST_FunDef(AST):
	def __init__(self, token, args, stmts):
		self.token = token
		self.args = args
		self.content = stmts

class AST_Statement(AST):
	def __init__(self, stmt):
		self.stmt = stmt

class AST_CompountStmt(AST):
	def __init__(self):
		self.stmts = []

class Parser(object):
	def __init__(self, src):
		self.lexer = Lexer(src)
		self.token = self.lexer.get_token()
		self.trace = False

	def type_error(self, msg):
		raise TypeError(msg)

	def syntax_error(self, msg):
		raise SyntaxError(msg)

	def error(self, token, msg):
		print "line %i: '%s' %s" % (token.line, token.value, msg)
		if self.trace:
			raise Exception()
		sys.exit(0)

	def eat(self, type):
		if self.token.type != type:
			self.error(self.token, "Unexpected token")
		self.token = self.lexer.get_token()

	def peek(self):
		return self.lexer.peek_token()

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

	# fun_def : FUNDEF (SPACE NAME)* [SPACE] LBRACE compound_stmt RBRACE
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
		self.eat(LBRACE)
		stmts = self.compound_stmt()
		self.eat(RBRACE)
		return AST_FunDef(token, args, stmts)

	# factor : [SPACE] (INT | FLOAT | variable | fun_def) [SPACE]
	#        | [SPACE] LPAREN expr RPAREN [SPACE]
	#        | [SPACE] (ADD | SUB)* factor [SPACE]
	def factor(self):
		self.eat_space()

		if self.token.type == FUNDEF:
			res = self.fun_def()
		elif self.token.type == NAME:
			res = self.variable()
		elif self.token.type in [ADD, SUB]:
			curr = res = AST_UnaryOp(None, None)
			while self.token.type in [ADD, SUB]:
				curr.right = AST_UnaryOp(self.token, None)
				curr = curr.right
				if self.token.type == ADD:
					self.eat(ADD)
				if self.token.type == SUB:
					self.eat(SUB)
			curr.right = self.factor()
			res = res.right
		elif self.token.type == INT:
			res = AST_Int(self.token)
			self.eat(INT)
		elif self.token.type == FLOAT:
			res = AST_Float(self.token)
			self.eat(FLOAT)
		else:
			self.eat(LPAREN)
			res = self.expr()
			self.eat(RPAREN)

		self.eat_space()
		return res

	# term : factor ((MUL | DIV) factor)*
	def term(self):
		res = self.factor()
		while self.token.type in [MUL, DIV]:
			res = AST_BinOp(res, self.token, None)
			if self.token.type == MUL:
				self.eat(MUL)
			if self.token.type == DIV:
				self.eat(DIV)
			res.right = self.factor()
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

	# expr : arith_expr
	def expr(self):
		return self.arith_expr()

	# assign_stmt : variable ASSIGN expr
	def assign_stmt(self):
		expr = self.expr()

		if self.token.type != ASSIGN:
			return expr

		token = self.token
		self.eat(ASSIGN)
		right = self.expr()

		if type(expr) != AST_Name:
			self.error(token, "Invalid assignment target")

		return AST_Assign(expr, token, right)

	# statement : expr | assign_stmt
	def statement(self):
		return self.assign_stmt()

	# compound_stmt : ((statement | [SPACE]) NEWLINE)* [statement | SPACE]
	def compound_stmt(self):
		res = AST_CompountStmt()

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
		self.eat(EOF)
		return compound

text = open("script.txt", "r").read()
tree = Parser(text).program()
print tree.stmts[0].right.args