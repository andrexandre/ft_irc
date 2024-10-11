#pragma once


#include "../../webserv.hpp"

class httpRequest{

    private:
        string method;
        string urlRequest;
        string httpVersion;
        string host;
        string userAgent;
        string accept;
        string acceptLang;
        string acceptEnc;
        string connection;
        string upInsReq;
        string secFDest;
        string secFMode;
        string secFSite;
        string secFUser;
        string priority;
    public:
        httpRequest(void) {}
        ~httpRequest(void) {}
};

class httpResponse
{
    private:
        string httpVersion;
        string statusCode;
        string contentType;
        string contentLength;
    
    public:
        httpResponse(void) {}
        ~httpResponse(void) {}

};
