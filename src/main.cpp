#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <optional>
#include <vector>

#include "tokenization.hpp"
#include "generation.hpp"
#include "parser.hpp"


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

    // std::string buf = "";
    // for (int i = 0; i < contents.size(); i++) {
    //     char c = contents[i];
    //     if (std::isalpha(c)) {
    //         buf += c;
    //         while (i + 1 < contents.size() && std::isalnum(contents[i + 1])) {
    //             buf += contents[i + 1];
    //             i++;
    //         }
    //         if (buf == "return") {
    //             tokens.push_back(Token{TokenType::_return});
    //             buf = "";
    //         } else {
    //             std::cerr << "Unexpected identifier: " << buf << std::endl;
    //             exit(EXIT_FAILURE);
    //         }
    //     } else if (std::isdigit(c)) {
    //         buf += c;
    //         while (i + 1 < contents.size() && std::isdigit(contents[i + 1])) {
    //             buf += contents[i + 1];
    //             i++;
    //         }
    //         tokens.push_back(Token{.type=TokenType::int_lit, .value=buf});
    //         buf = "";
    //     } else if (c == ';') {
    //         tokens.push_back(Token{TokenType::semi});
    //     } else if (std::isspace(c)) {
    //         continue;
    //     } else {
    //         std::cerr << "Unexpected character: " << c << std::endl;
    //         exit(EXIT_FAILURE);
    //     }
    // }

    return tokens;
}

std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;
    output <<  "global _start\n_start:\n";
    // TODO: go through the tokens and print the content

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == TokenType::_return) {
            if (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens[i + 2].type == TokenType::semi) {
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens[i + 1].value.value() << std::endl;
                    output << "    syscall" << std::endl;
                }
            }
        }
    }
    // for (int i = 0; i < tokens.size(); i++) {
    //     if (tokens[i].type == TokenType::_return) {
    //         if (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::int_lit) {
    //             if (i + 2 < tokens.size() && tokens[i + 2].type == TokenType::semi) {
    //                 output << "    mov rax, 60\n";
    //                 output << "    mov rdi, " << tokens[i + 1].value.value() << "\n";
    //                 output << "    syscall\n";
    //             }
    //         }
    //     } else if (tokens[i].type == TokenType::int_lit) {

    //     } else if (tokens[i].type == TokenType::semi) {

    //     }
    // }
    return output.str();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect usage..." << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;

    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    // std::cout << contents << std::endl;
    std::vector<Token> tokens = tokenize(contents);
    // Output tokens
    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    return EXIT_SUCCESS;
}