#include <string>
#include <fstream>
#include "fr.hpp"

FR_Service::FR_Service()
{
  
}
FR_Service::~FR_Service()
{

}

std::string FR_Service::requestToAzure(const std::string input, std::string& result)
{
  MS_AZURE azure;

  /* TODO: input will be analyed and parce the parameters*/
  std::string param = input;
  EImageSource image = EImageSource::SOURCE_URL;
  ERequestType mode = ERequestType::TYPE_DETECT;

  azure.face(image, mode, param, result);
  std::string ret;
  return ret;
}

