#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Product{
        private:
            long _id;
            std::string _name;
            std::string _category;
            std::string _description;
            float _price;

        public:
            static Product fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_name() const;
            const std::string &get_category() const;
            const std::string &get_description() const;
            const float &get_price() const;

            long&        id();
            std::string &name();
            std::string &category();
            std::string &description();
            float &price();

            static void init();
            static std::optional<Product> read_by_id(long id);
            static std::vector<Product> read_all();
            void save_to_mysql();
            static std::vector<Product> search_order(std::string login);

            Poco::JSON::Object::Ptr toJSON() const;
    };
}

#endif