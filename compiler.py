# primitive types
INT = 'INT'
FLOAT = 'FLOAT'
FUN = 'FUN'
STRING = 'STRING'
BOOL = 'BOOL'

NULL = 'NULL'
EOF = 'EOF'

# arithmetic ops
ADD = 'ADD'
SUB = 'SUB'
MUL = 'MUL'
DIV = 'DIV'

# syntax
SPACE = 'SPACE'
NEWLINE = 'NEWLINE'
LPAREN = 'LPAREN'
RPAREN = 'RPAREN'

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

	def get_int(self):
		i = ""
		while not self.eof() and self.char.isdigit():
			i += self.get()
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
		if self.char.isdigit():
			return self.get_int()
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

	# factor : [SPACE] INT [SPACE]
	#        | [SPACE] LPAREN expr RPAREN [SPACE]
	#        | [SPACE] ( ADD | SUB )* factor [SPACE]
	def factor(self):
		while self.token.type == SPACE:
			self.eat(SPACE)

		if self.token.type in [ADD, SUB]:
			unary = 1
			while self.token.type in [ADD, SUB]:
				if self.token.type == ADD:
					unary *= +1
					self.eat(ADD)
				if self.token.type == SUB:
					unary *= -1
					self.eat(SUB)
			res = unary * self.factor()
		elif self.token.type == INT:
			res = self.token.value
			self.eat(INT)
		else:
			self.eat(LPAREN)
			res = self.expr()
			self.eat(RPAREN)

		while self.token.type == SPACE:
			self.eat(SPACE)
		return res

	# term : factor (( MUL | DIV ) factor )*
	def term(self):
		res = self.factor()
		while self.token.type in [MUL, DIV]:
			if self.token.type == MUL:
				self.eat(MUL)
				res *= self.factor()
			if self.token.type == DIV:
				self.eat(DIV)
				res /= self.factor()
		return res

	# expr : term (( ADD | SUB ) term )*
	def expr(self):
		res = self.term()
		while self.token.type in [ADD, SUB]:
			if self.token.type == ADD:
				self.eat(ADD)
				res += self.term()
			if self.token.type == SUB:
				self.eat(SUB)
				res -= self.term()
		return res

	# program : expr ( NEWLINE expr )* ( NEWLINE | SPACE )* EOF
	def program(self):
		print self.expr()
		while self.token.type == NEWLINE:
			self.eat(NEWLINE)
			print self.expr()
		while self.token.type in [NEWLINE, SPACE]:
			if self.token.type == NEWLINE:
				self.eat(NEWLINE)
			if self.token.type == SPACE:
				self.seat(SPACE)
		self.eat(EOF)

Parser(' ---(-2 + 18) / (2 + 3) \n 10/(3 + 1)').program()
Parser('--1--2').program()


