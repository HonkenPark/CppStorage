#include <iostream>
#include <string>

#include "fr.hpp"

int main()
{
    FR_Service service;
    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "Start FR Service" << std::endl;

    ///////////////////////PASS FROM IMAX MSG///////////////////////////////
    std::string response = "";
    service.request_Azure(
    1   , // 1 - URL,  2 - FILE
    2, // Family No. (1~3)
    // "leesoonjae.jpg",
    // "20180430010401_1208613_600_818.jpg",
    "https://img.hankyung.com/photo/202005/03.19553819.1.jpg",
    response);
    
    ///////////////////////FOR TEST, INPUT MANUALLY ///////////////////////

    if (response.empty())
    {
        std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "*** Not matched detected Person ***" << std::endl;
    }
    else
    {
        std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "*** Detected Person is [ " << response << " ] ***" << std::endl;
    }
    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "End FR Service" << std::endl;
    return 0;
}
