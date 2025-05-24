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
        if isinstance(node, Number):
            return node.value
        if isinstance(node, String):
            return node.value
        if isinstance(node, Variable):
            return self.env.get(node.name)
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
            if node.op == "<=": return left <= right
            if node.op == ">=": return left >= right
            raise RuntimeError(f"Unknown operator {node.op}")
        if isinstance(node, Call):
            func = self.eval(node.callee)  # 注意這裡不是只找函數名稱，支援函式值
            args = [self.eval(arg) for arg in node.args]
            if not callable(func):
                raise RuntimeError("Calling non-function")
            return func(self, args)   # 呼叫函式，傳入Interpreter實例與引數列表

    def exec(self, stmt):
        if isinstance(stmt, Assign):
            value = self.eval(stmt.value)
            try:
                self.env.set(stmt.name, value)
            except NameError:
                self.env.define(stmt.name, value)
        elif isinstance(stmt, Print):
            print(self.eval(stmt.expr))
        elif isinstance(stmt, Block):
            self.execute_block(stmt.statements, Environment(self.env))
        elif isinstance(stmt, If):
            if self.eval(stmt.condition):
                self.exec(stmt.then_branch)
            elif stmt.else_branch:
                self.exec(stmt.else_branch)
        elif isinstance(stmt, While):
            while self.eval(stmt.condition):
                self.exec(stmt.body)
        elif isinstance(stmt, Function):
            def func(interpreter, args):
                local_env = Environment(self.env)  # 支援閉包，保存定義時環境
                for name, val in zip(stmt.params, args):
                    local_env.define(name, val)
                # 呼叫時暫時切換環境
                old_env = interpreter.env
                interpreter.env = local_env
                try:
                    interpreter.execute_block(stmt.body.statements, local_env)
                except ReturnException as r:
                    interpreter.env = old_env
                    return r.value
                interpreter.env = old_env
            self.env.define(stmt.name, func)
        elif isinstance(stmt, Return):
            value = self.eval(stmt.value)
            raise ReturnException(value)
        else:
            # 假設是表達式語句
            self.eval(stmt)

    def execute_block(self, statements, env):
        old_env = self.env
        self.env = env
        try:
            for stmt in statements:
                self.exec(stmt)
        finally:
            self.env = old_env
