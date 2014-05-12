#include "WykopSDK.h"
#include "restclient.h"

#include <openssl/md5.h>
#include <cstring>
#include <cstdio>
#include <sstream>

#include <cctype>
#include <iomanip>
#include <string>

template <typename Iter>
std::string join(Iter begin, Iter end, std::string const& separator)
{
  std::ostringstream result;
  if (begin != end)
    result << *begin++;
  while (begin != end)
    result << separator << *begin++;
  return result.str();
}

std::string paramsencode(std::map<std::string, std::string> dict){
    std::vector<std::string> v;
    for (std::map<std::string, std::string>::iterator i = dict.begin(); i != dict.end(); i++){
        v.push_back(i->first + ","  + i->second);
    }
    return join(v.begin(), v.end(), ",");
}

std::string url_encode(const std::string &value) {
    std::ostringstream escaped;
    static char hex[] = "0123456789abcdef";
    escaped.fill('0');
    escaped << hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        }
        else if (c == ' ')  {
            escaped << '+';
        }
        else {
            escaped << '%' << std::setw(2) << ((int) c) << std::setw(0);
        }
    }

    return escaped.str();
}


WykopSDK::WykopSDK(std::string appkey, std::string secretkey, std::string login, std::string accountkey, std::string password):
    appkey(appkey),
    secretkey(secretkey),
    login(login),
    password(password),
    accountkey(accountkey),
    userkey(""),
    protocol("http://"),
    domain("a.wykop.pl/")
{
    //ctor
}

WykopSDK::~WykopSDK()
{
    //dtor
}


std::string WykopSDK::get_request_sign(std::string url, std::map<std::string, std::string> dict){

    std::string params = "";
    if(not dict.empty()){
        params = dict.begin()->second;
    }

    if(dict.size()>1){
        std::map<std::string, std::string>::iterator i = dict.begin();
        i++;
        for (; i != dict.end(); i++)
        {
            params +=","  + i->second;
        }
    }

    url = this->secretkey + url + params;

    //printf("\n%s\n", params.c_str());

    unsigned char digest[MD5_DIGEST_LENGTH];
    char napis[1000];

    const char* nowy_napis = url.c_str();
    memcpy(napis, nowy_napis, strlen(nowy_napis));

    MD5((unsigned char*)&napis, strlen(napis), (unsigned char*)&digest);

    char mdString[33];

    for(int i = 0; i < 16; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    //printf("\n\nmd5 digest: %s\n\n", mdString);

    return std::string(mdString);

}

std::string WykopSDK::construct_url(std::string rtype, std::string rmethod, std::vector<std::string> rmethod_params, std::map<std::string, std::string> api_params){

    std::vector<std::string> url_parts;

    url_parts.push_back(rtype);
    url_parts.push_back(rmethod);

    for(std::vector<std::string>::iterator i = rmethod_params.begin(); i != rmethod_params.end(); i++){
        url_parts.push_back(*i);
    }

    std::map<std::string, std::string> api_params_all;
    api_params_all["appkey"] = this->appkey;
    api_params_all["userkey"] = this->userkey;

    for(std::map<std::string, std::string>::iterator i = api_params.begin(); i != api_params.end(); i++){
        api_params_all[i->first] = i->second;
    }

    std::string api_params_str = paramsencode(api_params_all);

    url_parts.push_back(api_params_str);

    std::string path = join(url_parts.begin(), url_parts.end(), "/");

    std::string ret = this->protocol + this->domain + path;

    return ret;
}

Json::Value WykopSDK::construct_request(std::string rtype,
                        std::string rmethod,
                        std::vector<std::string> rmethod_params,
                        std::map<std::string, std::string> api_params,
                        std::map<std::string, std::string> post_params,
                        bool raw_response
){

    //rmethod_params = rmethod_params? rmethod_params:std::vector<std::string>();
    //api_params = api_params? api_params:std::map<std::string, std::string>();
    //post_params = post_params? post_params:std::map<std::string, std::string>();

    std::string url = construct_url(rtype, rmethod, rmethod_params, api_params);
    std::string apisign = get_request_sign(url, post_params);

    return request(url, post_params, apisign);
}

Json::Value WykopSDK::request(std::string url, std::map<std::string, std::string> data, std::string sign){


    std::vector<std::string> data_pairs;

    for(std::map<std::string, std::string>::iterator i = data.begin(); i != data.end(); i++){
        data_pairs.push_back(i->first + "=" + i->second);
    }
    std::string encoded = join(data_pairs.begin(), data_pairs.end(), "&");

    //printf("url: %s\n", url.c_str());
    //printf("encoded: %s\n", encoded.c_str());

    RestClient::response r = RestClient::post(url.c_str(), "application/x-www-form-urlencoded", encoded.c_str(), sign);

    Json::Value root;
    Json::Reader reader;

    bool parsedSuccess = reader.parse(r.body, root, false);
    printf(root.toStyledString().c_str());
    return root;
}


Json::Value WykopSDK::authenticate(std::string login="", std::string accountkey="", std::string password=""){
    std::string login_ = (login == "" ? this->login:login);
    std::string accountkey_ = (accountkey == "" ? this->accountkey:accountkey);
    std::string password_ = (password == "" ? this->password:password);

    if(login_ != "" and accountkey_ != ""){
        user_login(login_, accountkey_);
    }else if (login_ != "" and password_ != ""){
        user_login_password(login_, password_);
    }
}

Json::Value WykopSDK::user_login(std::string login, std::string accountkey){
    std::map<std::string, std::string> post_params;
    post_params["login"] = login;
    post_params["accountkey"] = accountkey;

    std::vector<std::string> rmethod_params;
    std::map<std::string, std::string> api_params;

    Json::Value res = construct_request("user", "login", rmethod_params, api_params, post_params);

    if(res["userkey"].type()){
        this->userkey = std::string(res["userkey"].asString());
    }
    return res;
}

Json::Value WykopSDK::user_login_password(std::string login, std::string password){
    std::map<std::string, std::string> post_params;
    post_params["login"] = login;
    post_params["password"] = password;

    std::vector<std::string> rmethod_params;
    std::map<std::string, std::string> api_params;

    Json::Value res = construct_request("user", "login", rmethod_params, api_params, post_params);

    if(res["userkey"].type()){
        this->userkey = std::string(res["userkey"].asString());
    }
    return res;
}

Json::Value WykopSDK::add_entry(std::string body){
    if(this->userkey == "")
        authenticate();

    std::map<std::string, std::string> post_params;
    post_params["body"] = body;

    std::vector<std::string> rmethod_params;
    std::map<std::string, std::string> api_params;

    return construct_request("entries", "add", rmethod_params, api_params, post_params);
}
