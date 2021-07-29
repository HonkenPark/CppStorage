#include <iostream>
#include <string>

#include "fr.hpp"

int main()
{
    FR_Service service;
    std::cout << "Start FR Service" << std::endl;

    std::string result = "";
    service.requestToAzure(result);
    std::cout << result << std::endl;

    std::cout << "End FR Service" << std::endl;
    return 0;
}
