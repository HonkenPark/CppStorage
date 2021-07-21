#include <stdio.h>
#include "Logger.h"
#define TESTMSG Msg
int main()
{
    Logger printLog;
    char msg = "Test\n";
    printLog.output(msg);
    return 0;
}
