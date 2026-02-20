#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    _return,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    inline Tokenizer(std::string &src) : m_src(std::move(src)) 
    {

    } 
    std::vector<Token> tokenize(const std::string& contents) {
        std::vector<Token> tokens;
        // use buf to store the current token being processed
        std::string buf = "";
        // For loop
        for (int i = 0; i < contents.length(); i++) {
            char c = contents[i];
            // Scenario 1: if c is an alphabet, it might be exit
            // Scenario 2: if c is a digit, it might be an integer literal
            // Scenario 3: if c is a semicolon, it is a semicolon token
            if (std::isalpha(c)) {
                buf += c;
                while (i + 1 < contents.size() && std::isalnum(contents[i + 1])) {
                    buf += contents[i + 1];
                    i++;
                }
                if (buf == "exit") {
                    tokens.push_back(Token{TokenType::_return});
                    buf = "";
                } else {
                    std::cerr << "Unexpected identifier: " << buf << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else if (std::isdigit(c)) {
                buf += c;
                while (i + 1 < contents.size() && std::isdigit(contents[i + 1])) {
                    buf += contents[i + 1];
                    i++;
                }
                tokens.push_back(Token{.type=TokenType::int_lit, .value=buf});
                buf = "";
            } else if (c == ';') {
                tokens.push_back(Token{.type=TokenType::semi});
            } else if (std::isspace(c)) {
                continue;
            } else {
                std::cerr << "Unexpected character: " << c << std::endl;
                exit(EXIT_FAILURE);
            }
            
        }

        return tokens;
    }
    
private:
    std::optional<char> peak() const {
        if (idx < m_src.size()) {
            return m_src.at(idx);
        }
        return {};
    }
    
    std::string m_src;
    int idx = 0;
};