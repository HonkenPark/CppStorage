#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <unistd.h>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"

#include "fr.hpp"

using namespace rapidjson;

FR_Service::FR_Service()
{
  
}
FR_Service::~FR_Service()
{

}

bool FR_Service::load_group_list(int imgParam, int familyParam, std::map<std::string, std::string>& group_list, const std::string path)
{
  // TEMP
  imgParam = 2;

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

      // TODO: 중복 체크 후 인서트
      group_list_.insert(std::pair<std::string, std::string>(name, value));
      std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << name << " : " << value << std::endl;
    }
    return true;
  }
  else
  {
    return false;
  }
}

void FR_Service::add_group_faceId_list(EImageSource image)
{
  std::string faceId = "";
  for (auto iter : group_list_)
  {
    faceId.clear();
    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "ITERATOR : fisrt[" << iter.first << "], second[" << iter.second << "]" << std::endl;
    AzureManager::inst()->faceId(image, ERequestType::TYPE_DETECT, iter.second, faceId);
    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << faceId << std::endl;

    if (faceId.empty())
    {
      std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "value is empty !! ignore [" << iter.first << "]" << std::endl;
      continue;
    }
    AzureManager::inst()->add_group_faceId_list(faceId, iter.first);
  }
}

void FR_Service::request_Azure(int imgParam, int familyParam, const std::string input_, std::string& response)
{
  /* TODO: input will be analyzed and parce the parameters*/
  EImageSource image;
  std::string input = input_;
  std::string result;
  if (imgParam == 1) image = EImageSource::SOURCE_URL;
  else image = EImageSource::SOURCE_STREAM;

  ERequestType mode = ERequestType::TYPE_GET_ATTRIBUTE;

  std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "===== REGIST GROUT PICTURE  S =====" << std::endl;
  load_group_list(imgParam, familyParam, group_list_, "./data/json/families.json");
  // add_group_faceId_list(image);
  add_group_faceId_list(EImageSource::SOURCE_STREAM);
  std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "===== REGIST GROUT PICTURE  E =====" << std::endl;

  std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "***** DETECT PICTURE  S *****" << std::endl;
  std::string faceId;
  AzureManager::inst()->faceId(image, ERequestType::TYPE_DETECT, input, faceId);
  std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "Detected picture fid is " << faceId << std::endl;
  std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "***** DETECT PICTURE  E *****" << std::endl;

  AzureManager::inst()->faceId(image, ERequestType::TYPE_FIND_SIMILAR, faceId, response);
}

