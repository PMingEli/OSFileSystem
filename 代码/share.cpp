#include "header.h"
bool History(string para1)
{
    if (!para1_history.empty())
        para1_history.pop_back();
    para1_history.push_back(para1);
    para1_ptr = para1_history.size();
    para1_history.push_back("");
    return true;
}