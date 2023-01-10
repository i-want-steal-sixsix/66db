#pragma once

#include "ql/ql_defs.h"
#include "ql/ql_manager.h"




//* sel_cols  类型：vector<selColMeta> selColMeta: {std::string<表名/别名>，std::string<列名>, std::string<结果名>}

//* tab_names 类型：vector<selTabMeta> selTabMeta: {std::string<表别名>(如果没有，则别名就是本来的表名)  std::string<对应的表名>}

//* conds     类型：（自定义） 只需要两个vector，一个是值，一个是类型即可

//void select_from(std::vector<TabCol> sel_cols, const std::vector<std::string> &tab_names,
//                 std::vector<Condition> conds);