#ifndef HTTPREQUESTFACTORY_H
#define HTTPREQUESTFACTORY_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../helper.h"

#include "handlers/user_handler.h"
#include "handlers/order_handler.h"
#include "handlers/product_handler.h"

class HTTPRequestFactory : public HTTPRequestHandlerFactory
{
private:
    bool hasSubstr(const std::string &str, const std::string &substr)
    {
        if (str.size() < substr.size())
            return false;
        for (size_t i = 0; i <= str.size() - substr.size(); ++i)
        {
            bool ok{true};
            for (size_t j = 0; ok && (j < substr.size()); ++j)
                ok = (str[i + j] == substr[j]);
            if (ok)
                return true;
        }
        return false;
    }

public:
    HTTPRequestFactory(const std::string &format) : _format(format)
    {
    }

    HTTPRequestHandler *createRequestHandler(
        const HTTPServerRequest &request)
    {
        static const std::string user = "/user";
        static const std::string order = "/order";
        static const std::string product = "/product";
        if (hasSubstr(request.getURI(), user))
            return new UserHandler(_format);
        if (hasSubstr(request.getURI(), order))
            return new OrderHandler(_format);
        if (hasSubstr(request.getURI(), product))
            return new ProductHandler(_format);
        return new WebPageHandler(_format);
        return 0;
    }

private:
    std::string _format;
};

#endif