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
# let x = 3;
# while (x > 0) {
#     print(x);
#     x = x - 1;
# }
# let condition = true;
# if (condition) {
#     print(100);
# } else {
#     print(200);
# }
# let another = false;
# if (another) {
#     print(999);
# } else {
#     print(888);
# }
# function add(a, b) {
#     return a + b;
# }
# function square(x) {
#     return x * x;
# }

# print(add(5, 7));      
# print(square(4));      
code = '''

let x = 10;
{
    let x = 5;
    print(x);
}
print(x);

let count = 3;
while (count > 0) {
    print(count);
    count = count - 1;
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

print(5 == 5);
print(5 != 3);
print(2 < 3);
print(2 <= 2);
print(4 > 3);
print(4 >= 4);
print(1 == 0);
print(1 != 1);

function add(a, b) {
    return a + b;
}
function square(x) {
    return x * x;
}
print(add(5, 7));
print(square(4));

function makeCounter() {
    let count = 0;
    function increment() {
        count = count + 1;
        return count;
    }
    return increment;
}
let counter = makeCounter();
print(counter());
print(counter());
print(counter());

function applyTwice(f, x) {
    return f(f(x));
}
function addOne(n) {
    return n + 1;
}
print(applyTwice(addOne, 5));




'''




tokens = tokenize(code)
print("Tokens:", tokens)

parser = Parser(tokens)
ast = parser.parse()
print("AST:", ast)


interp = Interpreter()
interp.exec(ast)
