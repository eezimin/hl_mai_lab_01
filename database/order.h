#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Order{
        private:
            long _id_order;
            long _id_product;
            std::string _login;

        public:
            static Order fromJSON(const std::string &str);

            long             get_id_order() const;
            long             get_id_product() const;
            const std::string &get_login() const;

            long&        id_order();
            long&        id_product();
            std::string &login();

            static void init();
            static void add_product(Order order);
            static void remove_product(Order order);
            static bool id_found(long id);
            static bool login_found(std::string login);

            Poco::JSON::Object::Ptr toJSON() const;
    };
}

#endif