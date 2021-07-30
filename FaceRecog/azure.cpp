#include <iostream>
#include <string>
#include <fstream>
#include "azure.hpp"

MS_AZURE::MS_AZURE()
{
  
}
MS_AZURE::~MS_AZURE()
{

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

std::string MS_AZURE::generate_key_header()
{
  std::string ret = "";
  ret.append(face_header_prefix).append(face_header_key);
  return ret;
}

std::string MS_AZURE::generate_content_header(EImageSource source)
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
    std::cout << "[ERROR] Unknown Image Source" << std::endl;
  }
  return ret;
}

std::string MS_AZURE::generate_post_url()
{
  std::string ret = "";
}

std::string MS_AZURE::generate_content_type()
{
  std::string ret = "";
}

void MS_AZURE::face(EImageSource imgSource, ERequestType mode, std::string& param, std::string& response)
{
  std::string contents_for_stream;
	curl = curl_easy_init();
    
  if(curl)
  {
    #ifdef SET_DEBUG_MODE_FOR_CURL
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    #endif

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* Set Header */
    curl_slist* responseHeaders = nullptr;
    responseHeaders = curl_slist_append(responseHeaders, generate_key_header().c_str());
    responseHeaders = curl_slist_append(responseHeaders, generate_content_header(imgSource).c_str());

    if (EImageSource::SOURCE_URL == imgSource)
    {
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, responseHeaders);

      switch (mode)
      {
      case ERequestType::TYPE_DETECT:
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, test_post_data_url.c_str());
        break;
      case ERequestType::TYPE_GET_ATTRIBUTE:
        break;
      case ERequestType::TYPE_FIND_SIMILAR:
        break;
      default:
        break;
      }
    }
    else if (EImageSource::SOURCE_STREAM == imgSource)
    {
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, responseHeaders);

      std::string path_stream = face_test_post_data_stream;
      std::ifstream in(path_stream.append(param).c_str(), std::ios::in | std::ios::binary);

      if (in)
      {
        in.seekg(0, std::ios::end);
        contents_for_stream.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents_for_stream[0], contents_for_stream.size());
        in.close();
      }

      switch (mode)
      {
      case ERequestType::TYPE_DETECT:
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, contents_for_stream.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, contents_for_stream.c_str());
        break;
      case ERequestType::TYPE_GET_ATTRIBUTE:
        break;
      case ERequestType::TYPE_FIND_SIMILAR:
        break;
      default:
        std::cout << "[ERROR] Unknow Request Type" << std::endl;
        return;
      }
    }
    else
    {
      std::cout << "[ERROR] Unknow Image Source" << std::endl;
      return;
    }
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
    res = curl_easy_perform(curl);

    curl_slist_free_all(responseHeaders);
    curl_easy_cleanup(curl);

    response = result;
  }
  else
  {
    std::cout << "[ERROR] curl is nullptr !!" << std::endl;
    return;
  }
}
