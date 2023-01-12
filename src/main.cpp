#include "error.h"
#include "sm/sm_def.h"
#include "sm/sm_manager.h"
#include "rm/rm_page_handle.h"
#include "ql/ql_manager.h"
#include "ql/ql_scan.h"
#include "defs.h"
#include "managers.h"
#include "interpret.h"

#include "sqlparser/ast.h"
#include "sqlparser/parsor_bison.tab.h"
#include "sqlparser/lex.yy.h"


#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <direct.h>

#include <vector>
#include <string.h>

PfPageManager sys_page_mgr;
std::map<std::string,std::string> Interp::_alt2tab;
std::map<std::string,std::vector<std::string>> Interp::tab2col;
std::vector<std::string> SmManager::DB_LIST;


int main(){

    // 生成data文件夹
    struct _stat dirStat;
    if(!((_stat(".\\data", &dirStat) == 0) && (dirStat.st_mode & _S_IFDIR))){
        //std::cout << "Data missing" << std::endl;
        if(_mkdir(".\\data") != 0){
            throw(WindowsError());
        }
    }

    //文件存储信息结构体
    struct _finddata_t fileinfo;
    //保存文件句柄
    long fHandle;
    //文件数记录器
    int i = 0;

    if( (fHandle=_findfirst( "./data/*.dbf", &fileinfo )) == -1L )
    {   
        //std::cout<<"no such .dbf file in the dictionary."<<std::endl;
    }
    else{
        do{
            i++;
            //std::cout<<"find file : "<<fileinfo.name<<"            size of file : "<<fileinfo.size<<"B"<<std::endl;
            std::string tmpdbname(fileinfo.name);
            tmpdbname.pop_back();tmpdbname.pop_back();tmpdbname.pop_back();tmpdbname.pop_back();
            SmManager::DB_LIST.push_back(tmpdbname);
        }while( _findnext(fHandle,&fileinfo)==0);
    }
    //关闭文件
    _findclose( fHandle );
    //std::cout<<"file amounts : "<<i<<std::endl;
   
    //test
    //for(int i=0;i<SmManager::DB_LIST.size();i++){
    //    std::cout<<"list "<<SmManager::DB_LIST[i]<<std::endl;
    //}
    std::cout << std::endl <<
            "+==========================================+\n"
            "|         __    __                         |\n"
            "|        /\\ \\  /\\ \\         +==+           |\n"
            "|       /::\\ \\/##\\ \\         | +=+         |\n"
            "|      /:/\\:\\_\\/\\#\\ \\        | | |         |\n"
            "|     /::\\-\\/_/\\-\\/_/   +----| | |         |\n"
            "|    /:/\\:\\_\\/\\#\\_\\     | +--| | |====+    |\n"
            "|    \\:\\/:/ /\\/#/ /     | +==+ | |==+ |    |\n"
            "|     \\::/ /\\##/ /      +======| +--+ |    |\n"
            "|      \\/_/  \\/_/              +------+    |\n"
            "|                                          |\n"
            "+==========================================+\n";
    std::cout << "\nEnter HELP; for help.\nEnter EXIT; to close the program."<< std::endl << std::endl;

    while (1) {
        if(SmManager::sys_state == SYS_DATABASE){
            std::cout << SmManager::db.name + " > ";
        }
        else{   
            std::cout << "~HOME > ";
        }
        char *line_read = new char[1100];
        std::cin.getline(line_read, 1000, ';');

        if (line_read == nullptr) {
            // EOF encountered
            break;
        }
        std::string line = line_read;
        line.append(";");
        delete [] line_read;
        //std::cout << "OK!INPUT IS: " << line << std::endl;

        if (!line.empty()) {
            YY_BUFFER_STATE buf = yy_scan_string(line.c_str());
            if (yyparse() == 0) {
                //if (ast::parse_tree != nullptr) {
                //    ast::parse_tree.get()->debug_print();
                //}
                if(Interp::interp_sql(ast::parse_tree) == 1){
                    break;
                }
            }
            yy_delete_buffer(buf);
        }
    }

    return 0;
}