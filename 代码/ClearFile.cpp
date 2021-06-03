#include "header.h"

bool ClearFile(string route)
{
    DeleteFile(route);
    CreateFile(route);
    return true;
}