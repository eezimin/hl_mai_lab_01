#include "user_service/http_user_service.h"


int main(int argc, char*argv[]) 
{
    HTTPWebServer app;
    return app.run(argc, argv);
}