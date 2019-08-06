#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <errno.h>
#include <set>
#include <unistd.h>
//#include "quickmail.h"
#include <map>

#define MAX_SHOE_NUM 50000
//#include "mail.h"
using namespace std;

// *  "products":[     is the final tag
// *  "style":"        is the color tag

const char From_Mail[] = "huangyeqi@mail.ustc.edu.cn";
const char To_Mail[] = "chivier.humber@gmail.com";
const char Subject_New_Update[] = "New Shoes";
const char Message_New_Update[] = "New Shoes!";
const string Detail_Tag = "details\":{";
const string Style_Tag = "\"style\":\"";
const string Large_Tag = "large";

int sendmail(const char *to, const char *from, const char *subject, const char *message)
{
    int retval = -1;
    FILE *mailpipe = popen("/usr/lib/sendmail -t", "w");
    if (mailpipe != NULL)
    {
        fprintf(mailpipe, "To: %s\n", to);
        fprintf(mailpipe, "From: %s\n", from);
        fprintf(mailpipe, "Subject: %s\n\n", subject);
        fwrite(message, 1, strlen(message), mailpipe);
        fwrite(".\n", 1, 2, mailpipe);
        pclose(mailpipe);
        retval = 0;
    }
    else
    {
        perror("Failed to invoke sendmail");
    }
    return retval;
}

int main()
{
    sendmail(From_Mail, To_Mail, Subject_New_Update, Message_New_Update);
}