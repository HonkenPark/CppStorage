#include <string>
#include <curl/curl.h>

#define SET_DEBUG_MODE_FOR_CURL

enum class EImageSource
{
    SOURCE_URL = 0,
    SOURCE_STREAM
}

enum class ERequestType
{
    TYPE_DETECT = 0,
    TYPE_GET_ATTRIBUTE,
    TYPE_FIND_SIMILAR
}

class MS_AZURE
{
public:
    MS_AZURE();
    ~MS_AZURE();

    /* TODO: Need to be re-arranged to be edieted each other */
    const std::string url = "=============================/face/v1.0/detect?detectionModel=detection_01&returnFaceId=true&returnFaceLandmarks=false&returnFaceAttributes=age,gender,headPose,smile,facialHair,glasses,emotion,hair,makeup,occlusion,accessories,blur,exposure,noise";
    const std::string header = "Ocp-Apim-Subscription-Key: ===================================";
    const std::string header_url = "Content-Type: application/json";
    const std::string header_stream = "Content-Type: application/octet-stream";
    const std::string post_data_url = "{\"url\":\"https://file.mk.co.kr/meet/neds/2021/05/image_readtop_2021_425252_16200070234632285.jpg\"}";
    const std::string post_data_stream = "${PWD}/img/";
    
    unsigned int WriteCallback(void *contents, unsigned int size, unsigned int nmemb, void *userp);
    void func(EImageSource imgSource, std::string &buf);

private:
    CURL* curl;
    CURLcode res;
    std::string result;
    std::string contents_for_stream;
};
