#include "Logger.h"
#include "Curl_Test.h"

#include <iostream>
#include <string>

int main()
{
    std::string result;
    func(result);
    std::cout << result << std::endl;

    char msg[10] = "Test\n";
    output(msg);

    return 0;
}
