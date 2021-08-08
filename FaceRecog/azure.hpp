#include <string>
#include <map>
#include <memory>
#include <curl/curl.h>

//  #define SET_DEBUG_MODE_FOR_CURL

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

class AzureManager
{
public:
    AzureManager();
    ~AzureManager();
    static const std::unique_ptr<AzureManager>& inst();

    /* TODO: Need to be re-arranged to be edieted each other */
    const std::string face_url_endpoint          = "";
    const std::string face_header_key            = "";

    const std::string face_url_version           = "/face/v1.0/";
    const std::string face_type_detect           = "detect?";
    const std::string face_type_find             = "findsimilars";
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

    // SAMPLE DATA
    const std::string face_test_post_data_url_prefix    = "{\"url\":\"";
    const std::string face_test_post_data_url_postfix    = "\"}";
    const std::string face_test_post_data_stream = "./data/img/";

    std::string get_faceId_from_json(std::string json);
    std::string get_json_from_faceId(std::string faceId, std::string candidate);
    
    std::string generate_key_header();
    std::string generate_content_header(EImageSource source);
    std::string generate_post_url(ERequestType type);
    std::string generate_post_data(EImageSource source, ERequestType type, std::string input);

    void faceId(EImageSource source, ERequestType mode, std::string& param, std::string& faceId);
    void add_group_faceId_list(std::string name, std::string value);

private:
    CURL* curl;
    CURLcode res;
    std::string result; // Result from Azure after request

    std::string faceId_;
    std::map<std::string, std::string> group_faceId_;
};
