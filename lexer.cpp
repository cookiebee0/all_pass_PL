#include <iostream>
#include <vector>
// #include <string>
#include <regex>
#include "lexer.h"
#include "token.h"

using namespace std;

// enum class TokenType {
//     FRACTION, NUMBER, STRING, KEYWORD, IDENTIFIER,
//     OPERATOR, COMPARISON, PUNCTUATION, UNKNOWN
// };

// struct Token {
//     TokenType type;
//     string value;
// };

string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::FRACTION: return "Fraction";
        case TokenType::NUMBER: return "Number";
        case TokenType::STRING: return "String";
        case TokenType::KEYWORD: return "Keyword";
        case TokenType::IDENTIFIER: return "Identifier";
        case TokenType::OPERATOR: return "Operator";
        case TokenType::COMPARISON: return "Comparison";
        case TokenType::PUNCTUATION: return "Punctuation";
        case TokenType::UNKNOWN: return "Unknown";
        default: return "Invalid";
    }
}

vector<pair<TokenType, regex>> tokenPatterns = {
    {TokenType::KEYWORD, regex("\\b(fra|def|if|when|print|return)\\b")},
    {TokenType::FRACTION, regex("\\d+\\s*/\\s*\\d+")}, // 支援空格
    {TokenType::NUMBER, regex("\\d+")},
    {TokenType::IDENTIFIER, regex("[a-zA-Z_][a-zA-Z0-9_]*")},
    {TokenType::STRING, regex("\"[^\"]*\"")},
    {TokenType::OPERATOR, regex("[+\\-*/=]")},
    {TokenType::COMPARISON, regex("==|!=|<=|>=|<|>")},
    {TokenType::PUNCTUATION, regex("[{}();]")}
};


std::vector<Token> lexer(const std::string& code) {  // lexer
    vector<Token> tokens;
    smatch match;
    string::const_iterator start = code.begin();
    string::const_iterator end = code.end();

    while (start != end) {
        bool matched = false;

        for (const auto &[type, pattern] : tokenPatterns) {
            if (regex_search(start, end, match, pattern, regex_constants::match_continuous)) {
                string value = match.str();
                if (type == TokenType::FRACTION) {
                    value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
                }

                tokens.push_back({type, value});
                start += match.length();
                matched = true;
                break;
            }
        }

        if (!matched) {
            if (isspace(*start)) {
                ++start;
            } else {
                cerr << "Lexer Error: Unknown symbol '" << *start << "'\n";
                tokens.push_back({TokenType::UNKNOWN, string(1, *start)});
                ++start;
            }
        }
    }

    return tokens;
}

