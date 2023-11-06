#include "product_service/http_product_service.h"

int main(int argc, char*argv[]) 
{
    HTTPWebServer app;
    return app.run(argc, argv);
}