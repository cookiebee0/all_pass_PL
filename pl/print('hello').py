import re

TOKEN_TYPES = [
    ("NUMBER",   r'\d+(\.\d+)?'),
    ("STRING",   r'"[^"]*"'),
    ("IDENT",    r'[a-zA-Z_]\w*'),
    ("OP",       r'[+\-*/=<>!]'),
    ("LPAREN",   r'\('),
    ("RPAREN",   r'\)'),
    ("LBRACE",   r'\{'),
    ("RBRACE",   r'\}'),
    ("COMMA",    r','),
    ("SEMICOLON",r';'),
    ("WHITESPACE", r'\s+'),
]

KEYWORDS = {"let", "function", "return", "if", "else", "while", "print", "true", "false"}

def tokenize(code):
    tokens = []
    index = 0
    while index < len(code):
        for token_type, pattern in TOKEN_TYPES:
            regex = re.compile(pattern)
            match = regex.match(code, index)
            if match:
                text = match.group(0)
                if token_type == "WHITESPACE":
                    break
                elif token_type == "IDENT" and text in KEYWORDS:
                    tokens.append(("KEYWORD", text))
                else:
                    tokens.append((token_type, text))
                index = match.end(0)
                break
        else:
            raise SyntaxError(f"Unexpected character: {code[index]}")
    return tokens
