#include "order.h"
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
    
    void Order::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Order` (`id_order` INT NOT NULL AUTO_INCREMENT,"
                        << "`id_product` INT NOT NULL,"
                        << "`login` VARCHAR(256) NOT NULL,"
                        << "PRIMARY KEY (`id_order`));",
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

    Poco::JSON::Object::Ptr Order::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("id_order", _id_order);
        root->set("id_product", _id_product);
        root->set("login", _login);
        return root;
    }

    Order Order::fromJSON(const std::string &str)
    {
        Order order;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
        order.id_order() = object->getValue<long>("id_order");
        order.id_product() = object->getValue<long>("id_product");
        order.login() = object->getValue<std::string>("login");
        return order;
    }

    void Order::add_product(Order order)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            insert << "INSERT INTO Order (id_product, login) VALUES(?, ?)",
                use(order._id_product),
                use(order._login);
            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(order._id_order),
                range(0, 1); //  iterate over result set one row at a time
            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted: id=" << order._id_order << std::endl;
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

    void Order::remove_product(Order order)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement remove(session);
            remove << "DELETE FROM Order WHERE id_product=? AND login=?",
                use(order._id_product),
                use(order._login);
            remove.execute();
            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(order._id_order),
                range(0, 1); //  iterate over result set one row at a time
            if (!select.done())
            {
                select.execute();
            }
            std::cout << "deleted: id_product=" << order._id_product << "; login=" << order._login << std::endl;
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

    bool Order::login_found(std::string login)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            std::string tmp;
            select << "SELECT login FROM User where login=?",
                into(tmp),
                use(login);
            select.execute();
            if (tmp.empty()) return false;
            return true;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl; 
        }
        return false;
    }

    bool Order::id_found(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            std::string tmp;
            select << "SELECT id FROM Product where id=?",
                into(tmp),
                use(id);
            select.execute();
            if (tmp.empty()) return false;
            return true;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl; 
        }
        return false;
    }

    long Order::get_id_order() const
    {
        return _id_order;
    }

    long Order::get_id_product() const
    {
        return _id_product;
    }
    
    const std::string &Order::get_login() const
    {
        return _login;
    }

    long &Order::id_order()
    {
        return _id_order;
    }

    long &Order::id_product()
    {
        return _id_product;
    }

    std::string &Order::login()
    {
        return _login;
    }

}