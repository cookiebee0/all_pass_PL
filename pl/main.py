from lexer import tokenize
from interpreter import Interpreter
from parser import Parser
from ast_nodes import *

# let x = 10;
# {
#     let x = 5;
#     print(x); // Should print 5
# }
# print(x); // Should print 10
code = '''




let x = 3;
while (x > 0) {
    print(x);
    x = x - 1;
}
let condition = true;
if (condition) {
    print(100);
} else {
    print(200);
}
let another = false;
if (another) {
    print(999);
} else {
    print(888);
}
function add(a, b) {
    return a + b;
}
function square(x) {
    return x * x;
}

print(add(5, 7));      
print(square(4));      
'''




tokens = tokenize(code)
print("Tokens:", tokens)

parser = Parser(tokens)
ast = parser.parse()
print("AST:", ast)


interp = Interpreter()
interp.exec(ast)

