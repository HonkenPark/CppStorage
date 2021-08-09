#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"

#include "azure.hpp"

using namespace rapidjson;

static std::unique_ptr<AzureManager> inst_;

AzureManager::AzureManager()
{
  
}
AzureManager::~AzureManager()
{

}

const std::unique_ptr<AzureManager>& AzureManager::inst()
{
    static std::once_flag flag;
    std::call_once(flag, []{
        inst_ = std::make_unique<AzureManager>();
    });
    return inst_;
}

#ifdef SET_DEBUG_MODE_FOR_CURL
static void dump(const char* text, FILE* stream, unsigned char* ptr, size_t size)
{
  size_t i;
  size_t c;
  unsigned int width=0x10;
 
  fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n",
          text, (long)size, (long)size);
 
  for(i=0; i<size; i+= width) {
    fprintf(stream, "%4.4lx: ", (long)i);
 
    /* show hex to the left */
    for(c = 0; c < width; c++) {
      if(i+c < size)
        fprintf(stream, "%02x ", ptr[i+c]);
      else
        fputs("   ", stream);
    }
 
    /* show data on the right */
    for(c = 0; (c < width) && (i+c < size); c++) {
      char x = (ptr[i+c] >= 0x20 && ptr[i+c] < 0x80) ? ptr[i+c] : '.';
      fputc(x, stream);
    }
 
    fputc('\n', stream); /* newline */
  }
}

static int my_trace(CURL* handle, curl_infotype type, char* data, size_t size, void* userp)
{
  const char* text;
  (void)handle; /* prevent compiler warning */
  (void)userp;
 
  switch (type) {
  case CURLINFO_TEXT:
    fprintf(stderr, "== Info: %s", data);
  default: /* in case a new one is introduced to shock us */
    return 0;
 
  case CURLINFO_HEADER_OUT:
    text = "=> Send header";
    break;
  case CURLINFO_DATA_OUT:
    text = "=> Send data";
    break;
  case CURLINFO_SSL_DATA_OUT:
    text = "=> Send SSL data";
    break;
  case CURLINFO_HEADER_IN:
    text = "<= Recv header";
    break;
  case CURLINFO_DATA_IN:
    text = "<= Recv data";
    break;
  case CURLINFO_SSL_DATA_IN:
    text = "<= Recv SSL data";
    break;
  }
 
  dump(text, stderr, (unsigned char*)data, size);
  return 0;
}
#endif

static unsigned int WriteCallback(void* contents, unsigned int size, unsigned int nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size* nmemb);
    return size* nmemb;
}

std::string AzureManager::get_faceId_from_json(std::string json)
{
  int count = 0;
  size_t pos = 0;
  std::string ret = "";

  if ((json.front() == '[') && (json.back() == ']'))
  {
    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "Front '[' and Last ']' will be deleted !!" << json << std::endl;
    json.erase(0, 1);
    json.pop_back();
  }

  if (json.empty())
  {
    return "";
  }

  std::string tmp = json;

  while((pos = tmp.find("faceId")) != std::string::npos)
  {
    count++;
    tmp = tmp.substr(pos + 1);
  }

  if (count > 1)
  {
    // TODO: Temporary, 2nd~ result will be deleted.
    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "Onlt 1th faceId will be processed !!" << json << std::endl;
    if ((pos = json.find(",{")) != std::string::npos)
    {
      tmp = json.substr(0, pos);
    }
    json = tmp;
  }

  Document d;
  d.Parse(json.c_str());
  assert(d.IsObject());
  assert(d.HasMember("faceId"));
  assert(d["faceId"].IsString());
  ret = d["faceId"].GetString();

  return ret;
}

std::string AzureManager::get_json_from_faceId(std::string faceId, std::string candidate)
{
  std::string ret = "";
  std::string faceId_key = "faceId";
  std::string faceId_value = faceId;
  int max_faceIds = 0;
  std::string faceIds_key = "faceIds";
  std::string faceIds_value = "";
  std::string candidate_key = "maxNumOfCandidatesReturned";
  std::string candidate_value = candidate;
  std::string mode_key = "mode";
  std::string mode_value = "matchPerson";

  for (auto iter : group_faceId_)
  {
    if (max_faceIds > 0)
    {
      faceIds_value.append(",");
    }
    faceIds_value.append("\"").append(iter.first).append("\"");
    max_faceIds++;
  }
  if (faceIds_value.back() == ',')
  {
    faceIds_value.pop_back();
  }

  ret.append("{");
  ret.append("\"").append(faceId_key).append("\"");
  ret.append(":").append("\"").append(faceId_value).append("\"").append(",");

  ret.append("\"").append(faceIds_key).append("\"");
  ret.append(":").append("[").append(faceIds_value).append("]").append(",");

  ret.append("\"").append(candidate_key).append("\"");
  ret.append(":").append(candidate_value).append(",");

  ret.append("\"").append(mode_key).append("\"");
  ret.append(":").append("\"").append(mode_value).append("\"");
  ret.append("}");

  return ret;
}

std::string AzureManager::generate_key_header()
{
  std::string ret = "";
  ret.append(face_header_prefix)
     .append(face_header_key);
  
 std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "generate_key_header : [" << ret << "]" << std::endl;
  return ret;
}

std::string AzureManager::generate_content_header(EImageSource source)
{
  std::string ret = "";
  if (EImageSource::SOURCE_URL == source)
  {
    ret.append(face_content_type_url);
  }
  else if (EImageSource::SOURCE_STREAM == source)
  {
    ret.append(face_content_type_stream);
  }
  else
  {
    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "[ERROR] Unknown Image Source" << std::endl;
  }

 std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "generate_content_header : [" << ret << "]" << std::endl;
  return ret;
}

std::string AzureManager::generate_post_url(ERequestType type)
{
  std::string ret = "";
  ret.append(face_url_endpoint)
     .append(face_url_version);
  switch(type)
  {
    case ERequestType::TYPE_DETECT:
      ret.append(face_type_detect)
         .append(face_detection_model)
         .append(face_detection3)
         .append("&").append(face_retFaceId).append("true")
         .append("&").append(face_retFaceLandmark).append("false");
    break;
    case ERequestType::TYPE_GET_ATTRIBUTE:
      ret.append(face_type_detect)
         .append(face_detection_model)
         .append(face_detection1)
         .append("&").append(face_retFaceId).append("true")
         .append("&").append(face_retFaceLandmark).append("false")
         .append("&").append(face_retFaceAttribute).append(face_attr_age).append(",").append(face_attr_gender).append(",")
                                                   .append(face_attr_headPose).append(",").append(face_attr_smile).append(",")
                                                   .append(face_attr_facialHair).append(",").append(face_attr_glasses).append(",")
                                                   .append(face_attr_emotion).append(",").append(face_attr_hair).append(",")
                                                   .append(face_attr_makeup).append(",").append(face_attr_occlusion).append(",")
                                                   .append(face_attr_accessories).append(",").append(face_attr_blur).append(",")
                                                   .append(face_attr_exposure).append(",").append(face_attr_noise);
    break;
    case ERequestType::TYPE_FIND_SIMILAR:
      ret.append(face_type_find);
    break;
    default:
      std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "[ERROR] Unknown Request Type" << std::endl;
      ret.clear();
    break;
  }

 std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "generate_post_url : [" << ret << "]" << std::endl;
  return ret;
}

std::string AzureManager::generate_post_data(EImageSource source, ERequestType type, std::string input)
{
  std::string ret = "";
  std::string path_target = "";
  switch(type)
  {
    case ERequestType::TYPE_DETECT:
    case ERequestType::TYPE_GET_ATTRIBUTE:
    if (EImageSource::SOURCE_URL == source)
    {
      ret.append(face_test_post_data_url_prefix)
         .append(input).append(face_test_post_data_url_postfix);
      std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << ret << std::endl;
    }
    else if (EImageSource::SOURCE_STREAM == source)
    {
      path_target.append(face_test_post_data_stream);
      std::ifstream in(path_target.append(input).c_str(), std::ios::in | std::ios::binary);
      if (in)
      {
        in.seekg(0, std::ios::end);
        ret.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&ret[0], ret.size());
        in.close();
      }
      else
      {
        std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "[ERROR] ifstream failed" << std::endl;
        ret.clear();
      }
    }
    else
    {
      std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "[ERROR] Unknown Image Source" << std::endl;
    }
    break;
    case ERequestType::TYPE_FIND_SIMILAR:
      ret = get_json_from_faceId(faceId_, "10");
      std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << ret << std::endl;
    break;
    default:
      std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "[ERROR] Unknown Request Type" << std::endl;
      ret.clear();
    break;
  }

  return ret;
}

void AzureManager::add_group_faceId_list(std::string name, std::string value)
{
  //TODO: 중복체크필요
  group_faceId_.insert(std::pair<std::string, std::string>(name, value));
}

void AzureManager::faceId(EImageSource source, ERequestType type, std::string& input, std::string& response)
{
  std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "faceId  S" << std::endl;
  std::string contents_for_postfield = "";

  if (ERequestType::TYPE_FIND_SIMILAR == type)
  {
    source = EImageSource::SOURCE_URL;
  }
  
  result.clear();
	curl = curl_easy_init();
    
  if(curl)
  {
    #ifdef SET_DEBUG_MODE_FOR_CURL
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    #endif

    curl_easy_setopt(curl, CURLOPT_URL, generate_post_url(type).c_str());

    /* Set Header */
    curl_slist* responseHeaders = nullptr;
    responseHeaders = curl_slist_append(responseHeaders, generate_key_header().c_str());
    responseHeaders = curl_slist_append(responseHeaders, generate_content_header(source).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, responseHeaders);

    contents_for_postfield = generate_post_data(source, type, input);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, contents_for_postfield.c_str());

    if (EImageSource::SOURCE_STREAM == source)
    {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, contents_for_postfield.size());
    }
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
    res = curl_easy_perform(curl);

    curl_slist_free_all(responseHeaders);
    curl_easy_cleanup(curl);

    /* TODO: Get faceId from result Json */

    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "RESPONSE from AZURE \n" << result << std::endl;
    response = get_faceId_from_json(result);
  }
  else
  {
    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "[ERROR] curl is nullptr !!" << std::endl;
    return;
  }

  faceId_ = response;

  if (ERequestType::TYPE_FIND_SIMILAR == type)
  {
    response.clear();
    for (auto iter : group_faceId_)
    {
      if (iter.first == faceId_)
      {
        std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << iter.second << std::endl;
        response = iter.second;
        break;
      }
    }
    //TODO: result --> [{"faceId":"4e4610b2-60b3-439c-9e7e-a26b8aefcf3d","confidence":1.0}]   faceId parcing 후, 리스트에서 검색
  }
  std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << "faceId  E" << std::endl;
}
