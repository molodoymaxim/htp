#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include<variant>
#include <unordered_map>
#include <cctype>

struct OpeningBracket {};
struct ClosingBracket {};
struct Number { int value; };
struct UnknownToken { std::string value; };
struct MinToken {};
struct AbsToken {};
struct Plus {};
struct Minus {};
struct Multiply {};
struct Modulo {};
struct Divide {};
struct MaxToken {};
struct SqrToken {};

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken,
    MinToken, AbsToken, Plus, Minus, Multiply, Modulo,
    MaxToken, SqrToken>;

const std::unordered_map<std::string, Token> kName2Token{
    {"max", MaxToken{}}, {"sqr", SqrToken{}}
};

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size() - 1) {
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    return Number{ value };
}

Token ParseName(const std::string& input, size_t& pos) {
    std::string name;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isalpha(symbol)) {
        name += symbol;
        if (pos == input.size() - 1) {
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    if (auto it = kName2Token.find(name); it != kName2Token.end()) {
        return it->second;
    }
    else {
        return UnknownToken{ name };
    }
}

std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;

    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);

        if (std::isspace(symbol)) {
            ++pos;
        }
        else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        }
        else if (std::isalpha(symbol)) {
            tokens.emplace_back(ParseName(input, pos));
        }
        else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            tokens.emplace_back(it->second);
            ++pos;
        }
        else {
            tokens.emplace_back(UnknownToken{ std::string(1, static_cast<char>(symbol)) });
            ++pos;
        }
    }

    return tokens;
}