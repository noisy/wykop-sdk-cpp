#include <iostream>
#include <map>
#include <string>
#include <jsoncpp/json/json.h>
#include "WykopSDK.h"
#include "restclient.h"

using namespace std;

int main()
{

    RestClient::response r = RestClient::get("http://a.wykop.pl/link/index/54321/appkey,12345");
    cout<<r.body<<endl;

    Json::Value root;
    Json::Reader reader;

    bool parsedSuccess = reader.parse(r.body, root, false);

    //const Json::Value error = root["dupa"];
    //Json::ValueType vt = error.type();

    /*if(root["error"] != Json::nullValue){
        cout<<"Error!"<<endl;
        Json::Value code = root["error"]["code"];
        cout<<"Kod: "<<code.asInt()<<endl;
        cout<<"Wiadomosc: "<<root["error"]["message"].asString()<<endl;
    }*/

    string appkey = "", secretkey = "", login = "", accountkey = "", password = "";

    WykopSDK  api(appkey, secretkey, login, accountkey, password);

    api.user_login_password("noisy", "");
    api.add_entry("test #cpp #wykopsdk #programowanie");

    /*map<string, string> dict = map<string, string>();
    dict.insert(pair<string,string>("embed", "http://serwer/plik.jpg"));
    dict.insert(pair<string,string>("body", "przykładowy komentarz"));

    w.get_request_sign("http://a.wykop.pl/link/index/54321/appkey,12345", dict);

    std::vector<std::string> vec;
    vec.push_back("1987058");

    cout<<w.construct_url("link", "index", vec);
    */
    //cout<<root.toStyledString()<<endl;

}
