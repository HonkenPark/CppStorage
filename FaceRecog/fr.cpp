#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "fr.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"

using namespace rapidjson;

FR_Service::FR_Service()
{
  
}
FR_Service::~FR_Service()
{

}

bool FR_Service::registGroupList(int imgParam, int familyParam, const std::string path)
{
  std::ifstream ifs(path.c_str());
  IStreamWrapper isw(ifs);

  group_list_.clear();
  
  Document d;
  d.ParseStream(isw);

  /* TODO: SELECT 1th, 2nd or 3rd Family's List */

  const Value& family = d["families"];
  if (family.IsArray())
  {
    std::string name;
    std::string value;

    const Value& select = family[familyParam - 1];

    for (auto& m : select.GetObject())
    {
      name.clear();
      name.append(m.name.GetString());

      value.clear();
      const Value& sel_val = select[m.name];
      value.append(sel_val[imgParam - 1].GetString());

      group_list_.insert(std::pair<std::string, std::string>(name, value));
      std::cout << name << " : " << value << std::endl;
    }
    return true;
  }
  else
  {
    return false;
  }
}

std::string FR_Service::requestToAzure(int imgParam, int familyParam, const std::string input, std::string& result)
{
  MS_AZURE azure;

  registGroupList(imgParam, familyParam, "./data/json/families.json");
  return "";

  /* TODO: input will be analyzed and parce the parameters*/
  EImageSource image;
  std::string param = input;
  if (imgParam == 1) image = EImageSource::SOURCE_URL;
  else image = EImageSource::SOURCE_STREAM;
  
  ERequestType mode = ERequestType::TYPE_FIND_SIMILAR;

  azure.face(image, mode, param, result);
  std::string ret;
  return ret;
}

