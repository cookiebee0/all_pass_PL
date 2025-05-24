class Expr: pass
class Stmt: pass

class Number(Expr):
    def __init__(self, value): self.value = float(value)
    def __repr__(self): return f"Number({self.value})"

class String(Expr):
    def __init__(self, value): self.value = value.strip('"')
    def __repr__(self): return f'String("{self.value}")'

class Variable(Expr):
    def __init__(self, name): self.name = name
    def __repr__(self): return f"Variable('{self.name}')"

class Binary(Expr):
    def __init__(self, left, op, right): self.left, self.op, self.right = left, op, right
    def __repr__(self): return f"Binary({self.left}, '{self.op}', {self.right})"

class Assign(Stmt):
    def __init__(self, name, value): self.name, self.value = name, value
    def __repr__(self): return f"Assign('{self.name}', {self.value})"

class Print(Stmt):
    def __init__(self, expr): self.expr = expr
    def __repr__(self): return f"Print({self.expr})"

class Block(Stmt):
    def __init__(self, statements): self.statements = statements
    def __repr__(self): return f"Block({self.statements})"

class If(Stmt):
    def __init__(self, condition, then_branch, else_branch=None):
        self.condition, self.then_branch, self.else_branch = condition, then_branch, else_branch
    def __repr__(self):
        return f"If({self.condition}, {self.then_branch}, {self.else_branch})"

class While(Stmt):
    def __init__(self, condition, body): self.condition, self.body = condition, body
    def __repr__(self): return f"While({self.condition}, {self.body})"

class Function(Stmt):
    def __init__(self, name, params, body): self.name, self.params, self.body = name, params, body
    def __repr__(self): return f"Function('{self.name}', {self.params}, {self.body})"

class Return(Stmt):
    def __init__(self, value): self.value = value
    def __repr__(self): return f"Return({self.value})"

class Call(Expr):
    def __init__(self, callee, args): self.callee, self.args = callee, args
    def __repr__(self): return f"Call({self.callee}, {self.args})"