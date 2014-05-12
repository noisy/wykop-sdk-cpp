#ifndef WYKOPSDK_H
#define WYKOPSDK_H

#include <string>
#include <map>
#include <vector>

#include <jsoncpp/json/json.h>

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
        Json::Value authenticate(std::string login="", std::string accountkey="", std::string password="");
        Json::Value user_login(std::string login, std::string accountkey);
        Json::Value user_login_password(std::string login, std::string password);


        // Comments
        Json::Value add_comment(int link_id, int comment_id, std::string body);
        Json::Value plus_comment(int link_id, int comment_id);
        Json::Value minus_comment(int link_id, int comment_id);
        Json::Value edit_comment(int comment_id, std::string body);
        Json::Value delete_comment(int comment_id);

        /*
        // Link
        Json::Value get_link(int link_id);
        Json::Value dig_link(int link_id);
        Json::Value cancel_link(int link_id);
        Json::Value bury_link(int link_id, int bury_id);
        Json::Value get_link_comments(int link_id);
        Json::Value get_link_reports(int link_id);
        Json::Value get_link_digs(int link_id);
        Json::Value get_link_related(int link_id);
        Json::Value get_link_buryreasons();
        Json::Value observe_link(int link_id);
        Json::Value favorite_link(int link_id);

        // Links
        Json::Value get_links_promoted(int page, std::string sort);
        Json::Value get_links_upcoming(int page, std::string sort);
        */
        // Profile
        //TODO

        // Search
        //TODO

        // User
        //TODO

        // Top
        //TODO

        // Related
        //TODO

        // Entries
        Json::Value add_entry(std::string body);
        //TODO

        // Rank
        //TODO

        // Observatory
        //TODO

        // Favorites
        //TODO

        // Stream
        //TODO

        // Tag
        //TODO

        // PM
        Json::Value send_message(std::string username, std::string body);
        Json::Value delete_conversation(std::string username);

    protected:

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


};

#endif // WYKOPSDK_H
