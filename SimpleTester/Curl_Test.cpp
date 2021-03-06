#include <string>
#include <fstream>
#include "Curl_Test.h"

#define URL  "==================================/face/v1.0/detect?detectionModel=detection_01&returnFaceId=true&returnFaceLandmarks=false&returnFaceAttributes=age,gender,headPose,smile,facialHair,glasses,emotion,hair,makeup,occlusion,accessories,blur,exposure,noise"

#define HEADER1 "Ocp-Apim-Subscription-Key: ==============================="
#define HEADER2_URL "Content-Type: application/json"
#define HEADER2_STREAM "Content-Type: application/octet-stream"

#define POST_DATA_URL "{\"url\":\"https://file.mk.co.kr/meet/neds/2021/05/image_readtop_2021_425252_16200070234632285.jpg\"}"
#define POST_DATA_STREAM "/mnt/d/Other/CppStorage/SimpleTester/phk07.jpg"

static
void dump(const char *text,
          FILE *stream, unsigned char *ptr, size_t size)
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

static
int my_trace(CURL *handle, curl_infotype type,
             char *data, size_t size,
             void *userp)
{
  const char *text;
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
 
  dump(text, stderr, (unsigned char *)data, size);
  return 0;
}

unsigned int ReadCallback(void *contents, unsigned int size, unsigned int nmemb, void *userp)
{
    FILE *readhere = (FILE*)userp;
    curl_off_t nread;

    unsigned int ret = fread(contents, size, nmemb, readhere);
    nread = (curl_off_t)ret;
    fprintf(stderr, "### We Read %" CURL_FORMAT_CURL_OFF_T " bytes from file\n", nread);
    return ret;
}

unsigned int WriteCallback(void *contents, unsigned int size, unsigned int nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void func(std::string &buf)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    
    if(curl)
    {
        /* Set Debugger */
        #ifdef PRINT_DEBUG
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        #endif

        curl_easy_setopt(curl, CURLOPT_URL, URL);



        /* Set Header */
        curl_slist *responseHeaders = nullptr;
        responseHeaders = curl_slist_append(responseHeaders, HEADER1);
        // responseHeaders = curl_slist_append(responseHeaders, HEADER2_URL);
        responseHeaders = curl_slist_append(responseHeaders, HEADER2_STREAM);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, responseHeaders);


        /* Set POST data */
        // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POST_DATA_URL);

        std::string contents;
        std::ifstream in(POST_DATA_STREAM, std::ios::in | std::ios::binary);

        if (in)
        {
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
        }

        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, contents.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, contents.c_str());




        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        curl_slist_free_all(responseHeaders);
        curl_easy_cleanup(curl);

        buf = readBuffer;
    }
}
