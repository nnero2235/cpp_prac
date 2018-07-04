#include <nlohmann/json.hpp>
#include "string"
#include <iostream>

using json = nlohmann::json;

int main(){
    json j;
    j["name"] = "nnero";
    j["age"] = 13;
    j["category"] = "person";
    j["likes"]["name"] = "computer";
    j["likes"]["like_year"] = "13";
    j["list"] = {{{"fuck",true},{"thus",nullptr}},{{"fuck",true},{"thus",nullptr}}};
    std::string s = j.dump();
    std::cout<<s<<std::endl;
    json j2 = "{\"fake\":\"cxx\",\"nice\":12,\"list\":[\"string\",\"yeak\"]}"_json;
    std::cout<<j2["fake"]<<std::endl;
    return 0;
}
