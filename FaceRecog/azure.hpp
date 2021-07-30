#include <string>
#include <map>
#include <curl/curl.h>

// #define SET_DEBUG_MODE_FOR_CURL

enum class EImageSource
{
    SOURCE_URL = 0,
    SOURCE_STREAM
};

enum class ERequestType
{
    TYPE_DETECT = 0,
    TYPE_GET_ATTRIBUTE,
    TYPE_FIND_SIMILAR
};

class MS_AZURE
{
public:
    MS_AZURE();
    ~MS_AZURE();

    /* TODO: Need to be re-arranged to be edieted each other */
    const std::string face_url_endpoint          = "";
    const std::string face_header_key            = "";

    const std::string face_url_version           = "/face/v1.0/";
    const std::string face_mode_detect           = "detect?";
    const std::string face_mode_find             = "findsimilars";
    const std::string face_detection_model       = "detectionModel=";
    const std::string face_detection1            = "detection_01";
    const std::string face_detection2            = "detection_02";
    const std::string face_detection3            = "detection_03";
    const std::string face_retFaceId             = "returnFaceId=";
    const std::string face_retFaceLandmark       = "returnFaceLandmarks=";
    const std::string face_retFaceAttribute      = "returnFaceAttributes=";
    const std::string face_attr_age              = "age";
    const std::string face_attr_gender           = "gender";
    const std::string face_attr_headPose         = "headPose";
    const std::string face_attr_smile            = "smile";
    const std::string face_attr_facialHair       = "facialHair";
    const std::string face_attr_glasses          = "glasses";
    const std::string face_attr_emotion          = "emotion";
    const std::string face_attr_hair             = "hair";
    const std::string face_attr_makeup           = "makeup";
    const std::string face_attr_occlusion        = "occlusion";
    const std::string face_attr_accessories      = "accessories";
    const std::string face_attr_blur             = "blur";
    const std::string face_attr_exposure         = "exposure";
    const std::string face_attr_noise            = "noise";
    const std::string face_header_prefix         = "Ocp-Apim-Subscription-Key: ";
    const std::string face_content_type_url      = "Content-Type: application/json";
    const std::string face_content_type_stream   = "Content-Type: application/octet-stream";
    const std::string face_test_post_data_stream = "./data/img/";

    // SAMPLE DATA
    const std::string face_test_post_data_url    = "{\"url\":\"https://file.mk.co.kr/meet/neds/2021/05/image_readtop_2021_425252_16200070234632285.jpg\"}";
    
    std::string generate_key_header();
    std::string generate_content_header(EImageSource source);
    std::string generate_post_url();
    std::string generate_content_type();

    void face(EImageSource source, ERequestType mode, std::string& param, std::string& response);

private:
    CURL* curl;
    CURLcode res;
    std::string result; // Result from Azure after request

    std::string faceId_;
    std::map<std::string, std::string> group_faceId;
};
