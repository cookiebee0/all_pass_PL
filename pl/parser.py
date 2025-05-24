from ast_nodes import *

class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0

    def parse(self):
        statements = []
        while self.pos < len(self.tokens):
            statements.append(self.statement())
        return Block(statements)

    # --- Statement parsing ---
    def statement(self):
        if self.match("KEYWORD", "let"):
            return self.let_stmt()
        if self.match("KEYWORD", "print"):
            return self.print_stmt()
        if self.match("KEYWORD", "while"):
            return self.while_stmt()
        if self.match("KEYWORD", "if"):
            return self.if_stmt()
        if self.match("KEYWORD", "function"):
            return self.function_stmt()
        if self.match("KEYWORD", "return"):
            return self.return_stmt()
        if self.check("LBRACE"):
            return self.block()
        return self.expr_stmt()

    def let_stmt(self):
        name = self.consume("IDENT")[1]
        self.consume("OP", "=")
        value = self.expression()
        self.consume("SEMICOLON")
        return Assign(name, value)

    def print_stmt(self):
        expr = self.expression()
        self.consume("SEMICOLON")
        return Print(expr)

    def while_stmt(self):
        self.consume("LPAREN")
        condition = self.expression()
        self.consume("RPAREN")
        body = self.block()
        return While(condition, body)

    def if_stmt(self):
        self.consume("LPAREN")
        condition = self.expression()
        self.consume("RPAREN")
        then_branch = self.block()
        else_branch = None
        if self.match("KEYWORD", "else"):
            else_branch = self.block()
        return If(condition, then_branch, else_branch)

    def function_stmt(self):
        name = self.consume("IDENT")[1]
        self.consume("LPAREN")
        params = []
        if not self.check("RPAREN"):
            while True:
                params.append(self.consume("IDENT")[1])
                if not self.match("COMMA"):
                    break
        self.consume("RPAREN")
        body = self.block()
        return Function(name, params, body)

    def return_stmt(self):
        value = self.expression()
        self.consume("SEMICOLON")
        return Return(value)

    def expr_stmt(self):
        expr = self.expression()
        self.consume("SEMICOLON")
        return expr

    def block(self):
        self.consume("LBRACE")
        stmts = []
        while not self.check("RBRACE"):
            stmts.append(self.statement())
        self.consume("RBRACE")
        return Block(stmts)

    # --- Expression parsing ---
    def expression(self):
        return self.assignment()

    def assignment(self):
        expr = self.equality()
        if self.match("OP", "="):
            if not isinstance(expr, Variable):
                raise SyntaxError("Invalid assignment target.")
            value = self.assignment()
            return Assign(expr.name, value)
        return expr

    def equality(self):
        expr = self.comparison()
        while self.match("OP", "==") or self.match("OP", "!="):
            op = self.tokens[self.pos - 1][1]
            right = self.comparison()
            expr = Binary(expr, op, right)
        return expr

    def comparison(self):
        expr = self.term()
        while self.match("OP", ">") or self.match("OP", "<") or self.match("OP", ">=") or self.match("OP", "<="):
            op = self.tokens[self.pos - 1][1]
            right = self.term()
            expr = Binary(expr, op, right)
        return expr

    def term(self):
        expr = self.factor()
        while self.match("OP", "+") or self.match("OP", "-"):
            op = self.tokens[self.pos - 1][1]
            right = self.factor()
            expr = Binary(expr, op, right)
        return expr

    def factor(self):
        expr = self.unary()
        while self.match("OP", "*") or self.match("OP", "/"):
            op = self.tokens[self.pos - 1][1]
            right = self.unary()
            expr = Binary(expr, op, right)
        return expr

    def unary(self):
        if self.match("OP", "-"):
            right = self.unary()
            return Binary(Number(0), "-", right)
        return self.primary()

    def primary(self):
        if self.match("NUMBER"):
            return Number(self.tokens[self.pos - 1][1])
        if self.match("STRING"):
            return String(self.tokens[self.pos - 1][1])
        if self.match("KEYWORD", "true"):
            return Number(1)
        if self.match("KEYWORD", "false"):
            return Number(0)
        if self.match("IDENT"):
            name = self.tokens[self.pos - 1][1]
            if self.match("LPAREN"):
                args = []
                if not self.check("RPAREN"):
                    while True:
                        args.append(self.expression())
                        if not self.match("COMMA"):
                            break
                self.consume("RPAREN")
                return Call(Variable(name), args)
            return Variable(name)
        if self.match("LPAREN"):
            expr = self.expression()
            self.consume("RPAREN")
            return expr
        raise SyntaxError(f"Unexpected token: {self.tokens[self.pos]}")

    # --- Utility ---
    def match(self, type_, value=None):
        if self.pos < len(self.tokens):
            ttype, tval = self.tokens[self.pos]
            if ttype == type_ and (value is None or tval == value):
                self.pos += 1
                return True
        return False

    def consume(self, type_, value=None):
        if self.pos < len(self.tokens):
            ttype, tval = self.tokens[self.pos]
            if ttype == type_ and (value is None or tval == value):
                self.pos += 1
                return (ttype, tval)
            expected = f"{type_}" + (f"('{value}')" if value else "")
            actual = f"{ttype}" + (f"('{tval}')" if tval else "")
            raise SyntaxError(f"Expected {expected} but got {actual}")
        raise SyntaxError("Unexpected end of input")

    def check(self, type_, value=None):
        if self.pos < len(self.tokens):
            ttype, tval = self.tokens[self.pos]
            return ttype == type_ and (value is None or tval == value)
        return False
