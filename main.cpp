#include <iostream>
#include <map>
#include <string>
#include <jsoncpp/json/json.h>
#include "WykopSDK.h"
#include "restclient.h"

using namespace std;

int main()
{
    string appkey = "aNd401dAPp", secretkey = "3lWf1lCxD6", login = "twoj_login", password = "twoje_haslo";
    WykopSDK  api(appkey, secretkey, login, "", password);

    api.add_entry("test #cpp #wykopsdk #programowanie");
    api.send_message("noisy", "test #test wykop-cpp-sdk");
}
