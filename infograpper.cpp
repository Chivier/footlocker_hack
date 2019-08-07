#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <time.h>
#include <set>
#include <unistd.h>
//#include "quickmail.h"
#include <map>
#include <fstream>

#define MAX_SHOE_NUM 50000
//#include "mail.h"
using namespace std;

// *  "products":[     is the final tag
// *  "style":"        is the color tag

const string Detail_Tag = "details\":{";
const string Style_Tag = "\"style\":\"";
const string Large_Tag = "large";

#define NOTHING_NEW 0000
#define FIND_NEW_SHOE 0101
#define FIND_NEW_COLOR 0012
#define PRICE_LOSS 2015

typedef struct Shoes_Node_Tp
{
    float price;
    string name;
    vector<string> colors;

    void init()
    {
        price = 0;
        name = "";
        colors.clear();
    }

    void color_check() //* unique color
    {
        std::size_t found;

        for (auto i = colors.begin(); i < colors.end(); ++i)
        {
            found = (*i).find(Large_Tag);
            if (found != std::string::npos)
                colors.erase(i), --i;
        }
        sort(colors.begin(), colors.end());
        auto result = unique(colors.begin(), colors.end());
        colors.resize(distance(colors.begin(), result));
    }

    void update_price(const float x)
    {
        price = x;
    }

    void update_name(const string b)
    {
        name = b;
    }

    void add_colors(const string col)
    {
        colors.push_back(col);
    }

    bool operator==(const Shoes_Node_Tp other) const
    {
        return this->name == other.name;
    }

    bool operator!=(const Shoes_Node_Tp other) const
    {
        return this->name != other.name;
    }

    bool operator<(const Shoes_Node_Tp other) const
    {
        return this->price < other.price;
    }

    bool operator>(const Shoes_Node_Tp other) const
    {
        return this->price > other.price;
    }

    void out_test()
    {
        cout << price << "  " << name << "    cols = " << colors.size() << endl;
        for (int i = 0; i < colors.size(); ++i)
            cout << colors[i] << "   ";
        cout << endl;
    }
} shoes_;

bool Compare_color(const shoes_ A, const shoes_ B)
{
    // * to be improved more delicated
    if (A.colors.size() == B.colors.size())
        return 1;
    else
        return 0;
}

int Count_Colors(const std::string &str, const std::string &sub)
{
    int num = 0;
    size_t len = sub.length();
    if (len == 0)
    {
        len = 1;
    }
    for (size_t i = 0; (i = str.find(sub, i)) != std::string::npos; num++, i += len)
        ;
    return num;
}

int Cnt = 0;
shoes_ Cage[MAX_SHOE_NUM];

int Store_Cnt = 0;
shoes_ Storage[MAX_SHOE_NUM];

float price_transfer(const std::string &floatstr)
{
    float num = 0;
    float dot = 1;
    bool dotted = 0;
    for (auto iter = floatstr.begin() + 1; iter < floatstr.end(); iter++)
    {
        if ((*iter) >= '0' && (*iter) <= '9')
            num = num * 10 + (*iter) - '0';

        if (dotted)
            dot *= 10;

        if ((*iter) == '.')
            dotted = 1;
    }
    return num / dot;
}

void ShopList_Deal(const std::string &str)
{
    int brackets = 0;
    auto iter = str.begin();

    int stringtag = 0;
    string words;

    int flag = 0;
    int cnt_before = Cnt;
    //* flag = 0  --- usual read job
    //* flag = 1  --- name read
    //* flag = 2  --- read price
    //* flag = 3  --- read color
    //* flag = 4  --- num of price

    shoes_ shoe_now;
    shoes_ shoe_none;

    shoe_now.init();
    shoe_none.init();

    Cnt = 0;

    for (iter = str.begin(); iter < str.end(); ++iter)
    {
        if ((*iter) == '{')
            brackets++;
        if ((*iter) == '}')
            brackets--;

        if (brackets == 0 && shoe_now != shoe_none)
            Cage[++Cnt] = shoe_now,
            shoe_now = shoe_none;

        if (!stringtag && (*iter) == '"')
        {
            stringtag = 1,
            words = "";
            continue;
        }

        if (stringtag && (*iter) == '"')
        {
            if (flag == 1)
                shoe_now.update_name(words);

            if (flag == 4)
            {
                // cout << words << endl;
                float tmp = price_transfer(words);
                shoe_now.update_price(tmp);
            }

            if (flag == 3)
            {
                words.erase(words.end() - 6, words.end());
                shoe_now.add_colors(words);
            }

            //cout << flag;
            if (words == "name")
                flag = 1;
            else if (words == "price")
                flag = 2;
            else if (words == "altText")
                flag = 3;
            else if (words == "formattedValue")
                flag = 4;
            else
                flag = 0;

            stringtag = 0;
            // cout << words << "   " << flag << endl;
            continue;
        }

        if (stringtag)
            words += (*iter);
    }

    for (int i = 1; i <= Cnt; ++i)
    {
        Cage[i].color_check();
        //Cage[i].out_test();
    }
}

string Get_Details()
{
    system("curl 'https://www.footlocker.com/category/brands/jordan.html' -H 'authority: www.footlocker.com' -H 'pragma: no-cache' -H 'cache-control: no-cache' -H 'upgrade-insecure-requests: 1' -H 'user-agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.100 Safari/537.36' -H 'accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3' -H 'accept-encoding: gzip, deflate, br' -H 'accept-language: en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7' --compressed > rawdata.txt");

    int brackets = 0, tagB = -1; // * tags for brackets
    int colorcount = 0;          // * tags for color count
    int flag = 0;                // * flags for recognize
    int stringtag = 0;           // * tags for reading strings
    char ch;

    bool read_detail = 0; // * tags for detail information begins
    string details_info;  // * detail information

    details_info = "";

    string words;

    while (cin >> ch)
    {
        if (ch == '{')
            brackets++;
        if (ch == '}')
            brackets--;

        if (flag == 1 && ch == ':')
            continue;
        if (flag == 1 && ch == '[')
        {
            //details_info += ch;
            flag = 2;
            continue;
        }
        if (flag == 2 && ch == ']')
        {
            //details_info += ch;
            flag = 0;
            continue;
        }

        if (flag == 2 && ch == '{')
        {
            details_info += ch;
            tagB = brackets - 1;
            flag = 3;
            continue;
        }

        if (brackets == tagB && flag == 3)
        {
            details_info += ch;
            tagB = -1;
            flag = 2;
            continue;
        }

        if (flag == 3)
            details_info += ch;

        if (!stringtag && ch == '"')
        {
            stringtag = 1,
            words = "";
            continue;
        }

        if (stringtag && ch == '"')
        {
            if (words == "products")
                flag = 1;
            stringtag = 0;
            //cout << words << "   " << flag << endl;
            continue;
        }

        if (stringtag)
            words += ch;
    }

    return details_info;
}

void Clean_List()
{
    map<string, bool> Hash;
    Hash.clear();
    for (int i = 1; i <= Cnt; ++i)
    {
        if (Hash.find(Cage[i].name) == Hash.end())
        {
            Hash[Cage[i].name] = 1;
            Storage[++Store_Cnt] = Cage[i];
        }
    }
}

int Check_For_One(shoes_ check)
{
    //bool flag =0 ;
    for (int i = 1; i <= Store_Cnt; ++i)
    {
        if (check.name == Storage[i].name)
            return i;
    }
    return -1;
}

int Check_All()
{
    map<string, bool> Hash;
    Hash.clear();
    for (int i = 1; i <= Cnt; ++i)
    {
        if (Hash.find(Cage[i].name) != Hash.end())
        {
            for (int j = i; j <= Cnt; ++j)
                Cage[j] = Cage[j + 1];

            --Cnt;
        }
    }

    for (int i = 1; i <= Cnt; ++i)
    {
        int id = Check_For_One(Cage[i]);

        if (id == -1)
            return FIND_NEW_SHOE;
        if (!Compare_color(Storage[id], Cage[i]))
            return FIND_NEW_COLOR;
        if (Storage[id].price > Cage[i].price)
            return PRICE_LOSS;
    }

    return NOTHING_NEW;
}

void Put_out_Storage_Test()
{
    for (int i = 1; i <= Store_Cnt; ++i)
        Storage[i].out_test();
}

int main()
{
    freopen("rawdata.txt", "r", stdin);
    //freopen("list.txt", "w", stdout);

    ShopList_Deal(Get_Details());
    Clean_List();
    ofstream outfile;
    outfile.open("./list.txt");
    outfile.close();
    Put_out_Storage_Test();
    //while (1)
    for (int i = 1; i <= 10; ++i)
    {
        sleep(1);
        ShopList_Deal(Get_Details());

        switch (Check_All())
        {
        case FIND_NEW_COLOR:
            system("ssmtp A695139207@outloock.com < email_tag2.txt");
            break;
        case FIND_NEW_SHOE:
            system("ssmtp A695139207@outloock.com < email_tag1.txt");
            break;
        case PRICE_LOSS:
            system("ssmtp A695139207@outloock.com < email_tag3.txt");
            break;
        default:
            break;
        }
    }
}