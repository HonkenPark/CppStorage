#include <iostream>
#include <string>

#include "fr.hpp"

int main()
{
    FR_Service service;
    std::cout << "Start FR Service" << std::endl;

    ///////////////////////PASS FROM IMAX MSG///////////////////////////////
    std::string response = "";
    service.request_Azure(
    2, // 1 - URL,  2 - FILE
    3, // Family No. (1~3)
    "leesoonjae.jpg",
    // "https://file.mk.co.kr/meet/neds/2021/05/image_readtop_2021_425252_16200070234632285.jpg",
    response);
    
    ///////////////////////FOR TEST, INPUT MANUALLY ///////////////////////

    std::cout << "End FR Service" << std::endl;
    return 0;
}
