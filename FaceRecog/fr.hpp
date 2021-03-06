#include <string>
#include <map>
#include <curl/curl.h>

#include "azure.hpp"
//#include "luna.hpp"

const std::string families_list = "./data/json/families.json";

// SAMPLE PICTURE
const std::string great_grand_father = "great_grand_father.jpg";
const std::string great_grand_mother = "great_grand_mother.jpg";
const std::string grand_father = "grand_father.jpg";
const std::string grand_mother = "grand_mother.jpg";
const std::string father = "father.jpg";
const std::string mother = "mother.jpg";
const std::string me = "me.jpg";
const std::string wife = "wife.jpg";
const std::string older_uncle = "older_uncle.jpg";
const std::string older_aunt = "older_aunt.jpg";
const std::string younger_uncle = "younger_uncle.jpg";
const std::string younger_aunt = "younger_aunt.jpg";
const std::string older_brother = "older_brother.jpg";
const std::string older_sister = "older_sister.jpg";
const std::string younger_brother = "younger_brother.jpg";
const std::string younger_sister = "younger_sister.jpg";
const std::string son = "son.jpg";
const std::string daughter = "daughter.jpg";
const std::string smile_baby = "smile_baby.jpg";
const std::string crying_baby = "crying_baby.jpg";
const std::string not_baby = "not_baby.jpg";

enum class EServiceType
{
    SERVICE_AZURE = 0,
    SERVICE_LUNA
};

class FR_Service
{
public:
    FR_Service();
    ~FR_Service();

    AzureManager azuremanager;
    bool load_group_list(int imgParam, int familyParam, std::map<std::string, std::string>& group_list, const std::string path);
    void add_group_faceId_list(EImageSource image);
    void request_Azure(int imgParam, int familyParam, const std::string input, std::string& response);
    std::map<std::string, std::string> group_list_;
    ///inline const std::map<std::string, std::string>& grouplist() { return group_list_; }

private:
    std::string contents_for_stream;
};