#ifndef __BG96__
#define __BG96__

#include <iostream>
#include <chrono>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include "Serial.hpp"
#include "HttpPostFormData.hpp"

class BG96 : public Serial {
public:
    BG96 (const char* port, const int baudRate);
    BG96 (const std::string port, const int baudRate);
    ~BG96 ();

    int getRssi ();
    std::string httpPostMultipart (const std::string host,
                                   const std::string uri,
                                   const HttpPostFormData& fields, 
                                   const int timeoutSecs);
    std::string httpsPostMultipart (const std::string host,
                                    const std::string uri,
                                    const HttpPostFormData& fields,
                                    const int timeoutSecs);

public:
    void putATcmd (const char* cmd);
    void putATcmd (std::string cmd);
    void putATcmd (const char* cmd, const size_t len);
    void putATcmd (std::string cmd, const size_t len);
    std::string getResponse ();
    std::string waitResponseUntil (const std::string expected, const int timeoutSecs);

protected:
    bool prepareHttpPost (const std::string host, 
                          const std::string uri, 
                          const int CONTENT_TYPE, 
                          const int timeoutSecs);
    bool prepareHttpsPost (const std::string host, 
                           const std::string uri, 
                           const int CONTENT_TYPE, 
                           const int timeoutSecs);
    std::string postMultipart (const std::string host, 
                               const std::string uri,
                               const HttpPostFormData& fields,
                               const int timeoutSecs);
};

#endif