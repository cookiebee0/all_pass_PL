from ast_nodes import *

class ReturnException(Exception):
    def __init__(self, value):
        self.value = value

class Environment:
    def __init__(self, parent=None):
        self.values = {}
        self.parent = parent

    def define(self, name, value):
        self.values[name] = value

    def get(self, name):
        if name in self.values:
            return self.values[name]
        elif self.parent:
            return self.parent.get(name)
        raise NameError(f"Undefined variable '{name}'")

    def set(self, name, value):
        if name in self.values:
            self.values[name] = value
        elif self.parent:
            self.parent.set(name, value)
        else:
            raise NameError(f"Undefined variable '{name}'")

class Interpreter:
    def __init__(self):
        self.env = Environment()
        self.functions = {}

    def eval(self, node):
        if isinstance(node, Number): return node.value
        if isinstance(node, String): return node.value
        if isinstance(node, Variable): return self.env.get(node.name)
        if isinstance(node, Binary):
            left, right = self.eval(node.left), self.eval(node.right)
            if node.op == "+": return left + right
            if node.op == "-": return left - right
            if node.op == "*": return left * right
            if node.op == "/": return left / right
            if node.op == ">": return left > right
            if node.op == "<": return left < right
            if node.op == "==": return left == right
            if node.op == "!=": return left != right
            raise RuntimeError(f"Unknown operator {node.op}")
        if isinstance(node, Call):
            func = self.functions.get(node.callee.name)
            if not func:
                raise NameError(f"Undefined function '{node.callee.name}'")
            args = [self.eval(arg) for arg in node.args]
            return func(*args)

    def exec(self, stmt):
        if isinstance(stmt, Assign):
            value = self.eval(stmt.value)
            self.env.define(stmt.name, value)
        elif isinstance(stmt, Print):
            print(self.eval(stmt.expr))
        elif isinstance(stmt, Block):
            for s in stmt.statements:
                self.exec(s)
        elif isinstance(stmt, If):
            if self.eval(stmt.condition):
                self.exec(stmt.then_branch)
            elif stmt.else_branch:
                self.exec(stmt.else_branch)
        elif isinstance(stmt, While):
            while self.eval(stmt.condition):
                self.exec(stmt.body)
        elif isinstance(stmt, Function):
            def func(*args):
                local = Environment(self.env)  # 支援巢狀作用域
                for name, val in zip(stmt.params, args):
                    local.define(name, val)
                old_env = self.env
                self.env = local
                try:
                    for s in stmt.body.statements:
                        self.exec(s)
                except ReturnException as r:
                    self.env = old_env
                    return r.value
                self.env = old_env
            self.functions[stmt.name] = func
        elif isinstance(stmt, Return):
            value = self.eval(stmt.value)
            raise ReturnException(value)