#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

vector<string> Namelist;

void Read_Name_List()
{
    char ch;
    int flag = 0;
    string name;
    name = "";

    int endtag = 0;

    while (ch = getchar())
    {
        //cout << flag << ch << endl;
        if (flag == 0 && ch == '"')
        {
            flag = 1;
            continue;
        }

        if (flag == 1)
            if (ch == 'n')
                flag = 2;
            else
                flag = 0;

        if (flag == 2 && ch == ':')
            flag = 3;

        if (flag == 3 && ch == '"')
        {
            flag = 4;
            continue;
        }

        if (flag == 4 && ch == '"')
        {
            flag = 0;
            Namelist.push_back(name);
            name = "";
        }

        if (flag == 4)
            name = name + ch;

        if (ch != '#')
            endtag = 0;

        if (ch == '#')
            endtag++;

        if (endtag >= 4)
            break;
    }
}

void Print_Name_List()
{
    vector<string>::iterator it;
    for (it = Namelist.begin(); it != Namelist.end();)
        if ((*it)[0] != 'J' || (*it).size() <= 6 || (*it).find("Jordan") == (*it).npos)
            Namelist.erase(it);
        else
        {
            it++;
        }

    for (int i = 0; i < Namelist.size(); ++i)
        cout << Namelist[i] << endl;
}

int main()
{
    freopen("Jul4info.html", "r", stdin);
    freopen("July4_list.txt", "w", stdout);
    Read_Name_List();
    Print_Name_List();
}