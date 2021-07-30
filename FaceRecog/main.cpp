#include <iostream>
#include <string>

#include "fr.hpp"

int main()
{
    FR_Service service;
    std::cout << "Start FR Service" << std::endl;

    ///////////////////////PASS FROM IMAX MSG///////////////////////////////
    std::string response = "";
    service.requestToAzure(
    2, // 1 - URL,  2 - FILE
    3, // Family No. (1~3)
    "test_input.jpg",
    response);
    std::cout << response << std::endl;
    ///////////////////////FOR TEST, INPUT MANUALLY ///////////////////////

    std::cout << "End FR Service" << std::endl;
    return 0;
}
