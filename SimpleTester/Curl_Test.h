#include <curl/curl.h>

unsigned int WriteCallback(void *contents, unsigned int size, unsigned int nmemb, void *userp);
void func(std::string &buf);