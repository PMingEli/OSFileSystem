#include "header.h"

inode *OpenMutipleFile(string route)
{
    vector<string> directory;
    directory = split(route, "/");
    int n = directory.size();
    if (strcmp(directory[0].c_str(), ab_dir[0]) == 0)
    {
        while (dir_pointer > 1)
        {
            OpenDir("..");
        }
        for (int i = 1; i < n - 1; i++)
        {
            OpenDir(directory[i].c_str());
        }
        return OpenFile(directory[n - 1].c_str());
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            OpenDir(directory[i].c_str());
        }
        return OpenFile(directory[n - 1].c_str());
    }
}