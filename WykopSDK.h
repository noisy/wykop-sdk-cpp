#ifndef WYKOPSDK_H
#define WYKOPSDK_H

#include <string>
#include <map>
#include <vector>

#include "/usr/include/jsoncpp/json/json.h"


class WykopSDK
{
    std::string appkey;
    std::string secretkey;
    std::string login;
    std::string password;
    std::string accountkey;
    std::string userkey;
    std::string protocol;
    std::string domain;

    public:
        WykopSDK(std::string appkey, std::string secretkey, std::string login, std::string accountkey, std::string password);
        virtual ~WykopSDK();
        std::string get_request_sign(std::string url, std::map<std::string, std::string> dict);
        std::string construct_url(std::string rtype,
                                std::string rmethod,
                                std::vector<std::string> rmethod_params = std::vector<std::string>(),
                                std::map<std::string, std::string> api_params = std::map<std::string, std::string>()
        );

        Json::Value construct_request(std::string rtype,
                                std::string rmethod,
                                std::vector<std::string> rmethod_params = std::vector<std::string>(),
                                std::map<std::string, std::string> api_params = std::map<std::string, std::string>(),
                                std::map<std::string, std::string> post_params = std::map<std::string, std::string>(),
                                bool raw_response = false
        );


        Json::Value request(std::string url, std::map<std::string, std::string> data, std::string sign);
        Json::Value authenticate(std::string login, std::string accountkey, std::string password);
        Json::Value user_login(std::string login, std::string accountkey);
        Json::Value user_login_password(std::string login, std::string password);


        Json::Value add_entry(std::string body);

    protected:

};

#endif // WYKOPSDK_H
