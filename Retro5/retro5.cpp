#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <time.h>
#include <unistd.h>
#include "quickmail.h"

//#include "mail.h"
using namespace std;

// *  "details":{     is the final tag
// *  "style":"       is the color tag

const string Detail_Tag = "details\":{";
const string Style_Tag = "\"style\":\"";

int Count_Colors(const std::string &str, const std::string &sub)
{
    int num = 0;
    size_t len = sub.length();
    if (len == 0) {
        len = 1;    //应付空子串调用
    }
    for (size_t i = 0; (i = str.find(sub, i)) != std::string::npos; num++, i += len)
        ;
    return num;
}

void Get_Details()
{
    system("curl 'https://www.footlocker.com/product/jordan-retro-5-mens/D2720400.html' -H 'authority: www.footlocker.com' -H 'pragma: no-cache' -H 'cache-control: no-cache' -H 'upgrade-insecure-requests: 1' -H 'user-agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.100 Safari/537.36' -H 'accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3' -H 'referer: https://www.footlocker.com/category/collection/jordan/retro-5.html' -H 'accept-encoding: gzip, deflate, br' -H 'accept-language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7' --compressed > retro5.txt");

    int brackets = 0, tagB = -1;
    char ch;
    int colorcount = 0;
    string details_info;
    details_info = "";

    int flag = 0;
    int tagn = 10;
    int stringtag = 0;
    string words;

    bool read_detail = 0;
    while (cin >> ch) {
        //cout << ch;

        if (ch == '{') {
            brackets++;
        }
        if (ch == '}') {
            brackets--;
        }

        if (flag == 1 && ch == ':') {
            continue;
        }
        if (flag == 1 && ch == '{') {
            tagB = brackets - 1;
            flag = 2;
            continue;
        }
        if (brackets == tagB && flag == 2) {
            tagB = -1;
            flag = 0;
            continue;
        }
        if (flag == 2) {
            details_info += ch;
        }

        if (!stringtag && ch == '"') {
            stringtag = 1,
            words = "";
            continue;
        }

        if (stringtag && ch == '"') {
            if (words == "details") {
                flag = 1;
            }
            stringtag = 0;
            //cout << words << "   " << flag << endl;
            continue;
        }

        if (stringtag) {
            words += ch;
        }
    }

    //cout << details_info << endl;

    int ans = Count_Colors(details_info, Style_Tag) - 1;

    if (ans == -1) {
        sleep(10);
    }
    cout << "\n\nColor num =   " << ans << endl
         << endl;

    if (ans >= 4)
        while (1) {
            cout << '\a';
        }
}

int main()
{
    freopen("retro5.txt", "r", stdin);
    //freopen("detailinfo.txt", "w", stdout);
    Get_Details();
    sleep(1);
    //SendEmail("smtp.gmail.com", "chivier.humber@gmail.com", "2365574885", "2365574885@qq.com", "it is a test from vm ...");
}