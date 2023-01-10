#include <iostream>
#include <stdio.h>
#include <memory>
#include "ast.h"
#include "parsor_bison.tab.h"
#include "lex.yy.h"

int main(int argc, char** argv){
    while (1) {
        std::cout << "input> ";
        char *line_read = new char[110];
        std::cin.getline(line_read, 100, ';');

        if (line_read == nullptr) {
            // EOF encountered
            break;
        }
        std::string line = line_read;
        line.append(";");
        delete line_read;
        //std::cout << "OK!INPUT IS: " << line << std::endl;

        if (!line.empty()) {
            YY_BUFFER_STATE buf = yy_scan_string(line.c_str());
            if (yyparse() == 0) {
                if (ast::parse_tree != nullptr) {
                    ast::parse_tree.get()->debug_print();
                }
                Interp::i
            }
            yy_delete_buffer(buf);
        }
    }
    return 0;
}