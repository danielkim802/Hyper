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
LPAREN   = 'LPAREN'
RPAREN   = 'RPAREN'
LBRACE   = 'LBRACE'
RBRACE   = 'RBRACE'
LBRACKET = 'LBRACKET'
RBRACKET = 'RBRACKET'

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

	def syntax_error(self, msg):
		raise SyntaxError(msg)

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
		while self.is_space(self.char):
			self.get()
		return Token(SPACE, ' ', self.line)

	def get_newline(self):
		while self.is_newline(self.char):
			self.get()
		return Token(NEWLINE, '\\n', self.line)

	def get_num(self):
		i = ""
		dot = False
		while not self.eof() and self.char.isdigit() or self.char == '.':
			if self.char == '.':
				if not dot:
					dot = True
				else:
					self.syntax_error("unexpected token %s" % self.char)
			i += self.get()

		if dot:
			return Token(FLOAT, float(i), self.line)
		return Token(INT, int(i), self.line)

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
		if self.char.isdigit() or self.char == '.':
			return self.get_num()
		if self.is_space(self.char):
			return self.get_space()
		if self.is_newline(self.char):
			return self.get_newline()
		if self.char in ['+', '-', '*', '/']:
			return self.get_binop()
		if self.char == '(':
			self.get()
			return Token(LPAREN, '(', self.line)
		if self.char == ')':
			self.get()
			return Token(RPAREN, ')', self.line)

		self.syntax_error("unexpected token %s" % self.char)

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

class Parser(object):
	def __init__(self, src):
		self.lexer = Lexer(src)
		self.token = self.lexer.get_token()

	def type_error(self, msg):
		raise TypeError(msg)

	def syntax_error(self, msg):
		raise SyntaxError(msg)

	def eat(self, type):
		if self.token.type != type:
			self.type_error("expected %s but got %s at line %i" % (type, self.token.type, self.token.line))
		self.token = self.lexer.get_token()

	# factor : [SPACE] ( INT | FLOAT ) [SPACE]
	#        | [SPACE] LPAREN expr RPAREN [SPACE]
	#        | [SPACE] ( ADD | SUB )* factor [SPACE]
	def factor(self):
		if self.token.type == SPACE:
			self.eat(SPACE)

		if self.token.type in [ADD, SUB]:
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

		if self.token.type == SPACE:
			self.eat(SPACE)
		return res

	# term : factor (( MUL | DIV ) factor )*
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

	# expr : term (( ADD | SUB ) term )*
	def expr(self):
		res = self.term()
		while self.token.type in [ADD, SUB]:
			res = AST_BinOp(res, self.token, None)
			if self.token.type == ADD:
				self.eat(ADD)
			if self.token.type == SUB:
				self.eat(SUB)
			res.right = self.term()
		return res

tree = Parser(' (-3. + 0.5) / (10.25 + .2)').expr()
print tree.right.right.value