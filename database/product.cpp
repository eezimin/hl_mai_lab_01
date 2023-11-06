#include "product.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Product::init()
    {
        try
        {
            
            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Product` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`name` VARCHAR(256) NOT NULL,"
                        << "`category` VARCHAR(256) NOT NULL,"
                        << "`description` VARCHAR(256) NOT NULL,"
                        << "`price` VARCHAR(256) NOT NULL,"
                        << "PRIMARY KEY (`id`));",
                now;

        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Product::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("name", _name);
        root->set("category", _category);
        root->set("description", _description);
        root->set("price", _price);

        return root;
    }

    Product Product::fromJSON(const std::string &str)
    {
        Product product;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        product.id() = object->getValue<long>("id");
        product.name() = object->getValue<std::string>("name");
        product.category() = object->getValue<std::string>("category");
        product.description() = object->getValue<std::string>("description");
        product.price() = object->getValue<float>("price");

        return product;
    }

    std::optional<Product> Product::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Product a;
            select << "SELECT id, name, category, description, price FROM Product where id=?",
                into(a._id),
                into(a._name),
                into(a._category),
                into(a._description),
                into(a._price),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            
        }
        return {};
    }

    std::vector<Product> Product::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Product> result;
            Product a;
            select << "SELECT id, name, category, description, price FROM Product",
                into(a._id),
                into(a._name),
                into(a._category),
                into(a._description),
                into(a._price),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }


    void Product::save_to_mysql()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            insert << "INSERT INTO Product (name,category,description,price) VALUES(?, ?, ?, ?)",
                use(_name),
                use(_category),
                use(_description),
                use(_price);

            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<Product> Product::search_order(std::string login)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Product> result;
            Product a;
            select << "SELECT Product.id, name, category, description, price FROM Product INNER JOIN Order ON Product.id=Order.id_product WHERE Order.login=? GROUP BY Order.id_product",
                into(a._id),
                into(a._name),
                into(a._category),
                into(a._description),
                into(a._price),
                use(login),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }

            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Product::get_id() const
    {
        return _id;
    }
    
    const std::string &Product::get_name() const
    {
        return _name;
    }

    const std::string &Product::get_category() const
    {
        return _category;
    }

    const std::string &Product::get_description() const
    {
        return _description;
    }

    const float &Product::get_price() const
    {
        return _price;
    }

    long &Product::id()
    {
        return _id;
    }

    std::string &Product::name()
    {
        return _name;
    }

    std::string &Product::category()
    {
        return _category;
    }

    std::string &Product::description()
    {
        return _description;
    }

    float &Product::price()
    {
        return _price;
    }

}