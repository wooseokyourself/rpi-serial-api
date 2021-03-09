#include "../include/BG96.hpp"

BG96::BG96 (const char* port, const int baudRate) : Serial(port, baudRate) {
    // Wait until modem ready
    this->putATcmd("AT\r");
    this->waitResponseUntil("OK", 3);
    this->putATcmd("ATE0\r");
    this->waitResponseUntil("OK", 3);
    this->putATcmd("AT+CEREG=2\r");
    this->waitResponseUntil("OK", 3);
}

BG96::BG96 (const std::string port, const int baudRate) : Serial(port.c_str(), baudRate) {
    // Wait until modem ready
    this->putATcmd("AT\r");
    this->waitResponseUntil("OK", 3);
    this->putATcmd("ATE0\r");
    this->waitResponseUntil("OK", 3);
    this->putATcmd("AT+CEREG=2\r");
    this->waitResponseUntil("OK", 3);
}

BG96::~BG96 () { }

/**
 * @brief Get RSSI of modem through AT command.
 *          Try 10 times to get RSSI if AT response is invalid value as RSSI.
 * @return The RSSI of modem. -1 if failed.
 */
int BG96::getRssi() {
    // try 10 times to get rssi
    for (int trying = 0 ; trying < 10 ; trying ++) {
        this->putATcmd("AT+CSQ\r");
        std::string response = this->getResponse();
        int colonIdx = -1;
        // response form = "+CSQ: 31,99"
        for (int i = 0 ; i < response.length() ; i ++) {
            if (response[i] == ':') {
                colonIdx = i;
                break;
            }
        }
        if (colonIdx != -1)
            return std::stoi(response.substr(colonIdx + 2, colonIdx + 3));
        usleep(2500000);
    }
    return -1;
}

/**
 * @brief Send HTTP Post request through AT command.
 * @param host The host url. There should be no "http://" prefix.
 *              ex) www.myhome.com
 * @param uri The uri of url. There should be "/" prefix.
 *              ex) /my-room/my-desk
 * @param fields The fields object. This will not be checked for invalid values.
 * @param timeoutSecs Timeout.
 * @return The AT response.
 */
std::string BG96::httpPostMultipart (const std::string host,
                                     const std::string uri,
                                     const HttpPostFormData& fields, 
                                     const int timeoutSecs) {

    if (this->prepareHttpPost(host, uri, MULTIPART, timeoutSecs) == true)
        return this->postMultipart(host, uri, fields, timeoutSecs);
    return "AT command HTTP ERROR";
}


/**
 * @brief Send HTTPS Post request through AT command.
 * @param host The host url. There should be no "http://" prefix.
 *              ex) www.myhome.com
 * @param uri The uri of url. There should be "/" prefix.
 *              ex) /my-room/my-desk
 * @param fields The fields object. This will not be checked for invalid values.
 * @param timeoutSecs Timeout.
 * @return The AT response.
 */
std::string BG96::httpsPostMultipart (const std::string host,
                                      const std::string uri,
                                      const HttpPostFormData& fields, 
                                      const int timeoutSecs) {

    if (this->prepareHttpPost(host, uri, MULTIPART, timeoutSecs) == true)
        return this->postMultipart(host, uri, fields, timeoutSecs);
    return "AT command HTTPS ERROR";
}

/**
 * @brief Send AT command. All AT syntax must be followed.
 *          There shouldn't be null character in the command.
 *          If it is, use an overridden method that includes "len" in the parameter.
 * @param cmd String of command. There shouldn't be null character. 
 */
void BG96::putATcmd (const char* cmd) {
    Serial::puts(cmd);
}

/**
 * @brief Send AT command. All AT syntax must be followed.
 *          There shouldn't be null character in the command.
 *          If it is, use an overridden method that includes "len" in the parameter.
 * @param cmd String of command. There shouldn't be null character. 
 */
void BG96::putATcmd (std::string cmd) {
    // std::cout << "[Send AT command]" << std::endl;
    // std::cout << cmd << std::endl;
    Serial::puts(cmd.c_str());
}

/**
 * @brief Send AT command. All AT syntax must be followed.
 * @param cmd String of command.
 * @param len Length of cmd.
 */
void BG96::putATcmd (const char* cmd, const size_t len) {
    Serial::puts(cmd, len);
}

/**
 * @brief Send AT command. All AT syntax must be followed.
 * @param cmd String of command.
 * @param len Length of cmd.
 */
void BG96::putATcmd (std::string cmd, const size_t len) {
    // std::cout << "[Send AT command]" << std::endl;
    // std::cout << cmd << std::endl;
    int chunks = len / 512;
    int remain = len % 512;
    for (int i = 0 ; i < chunks ; i ++) {
        Serial::puts(cmd.substr(i * 512, (i+1) * 512).c_str(), 512);
        usleep(250000); // 0.25s
    }
    Serial::puts(cmd.substr(chunks * 512, len - 1).c_str(), remain);
}

/**
 * @brief Get AT response.
 * @return AT response or "No data read"
 */
std::string BG96::getResponse () {
    int len = Serial::remaining();
    if (len < 0)
        return "ERROR";
    else if (len == 0)
        return "No data read";
    else {
        std::string buf(len, ' ');
        int i = 0;
        while (len = Serial::remaining() > 0)
            buf[i++] = Serial::getchar();
        return buf;
    }
}

/**
 * @brief Wait AT response until it has sub-string of "expected".
 * @param expected Expected string within the AT response.
 * @param timeoutSecs Timeout.
 * @return The AT response.
 */
std::string BG96::waitResponseUntil (const std::string expected, const int timeoutSecs) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::string response;
    while (true) {
        response = this->getResponse();
        if (response.find(expected) != -1) {
            // std::cout << expected << std::endl;
            break;
        }
        else if (response.find("ERROR") != -1) {
            std::cerr << response << std::endl;
            break;
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() > timeoutSecs) {
            std::cerr << "TIMEOUT" << std::endl;
            break;
        }
        usleep(2500000); // 2.5s
    }
    return response;
}

bool BG96::prepareHttpPost (const std::string host, 
                            const std::string uri, 
                            const int CONTENT_TYPE, 
                            const int timeoutSecs) {
    this->putATcmd("AT+QHTTPCFG=\"contextid\",1\r");
    this->waitResponseUntil("OK", timeoutSecs);

    this->putATcmd("AT+QHTTPCFG=\"contenttype\"," + std::to_string(CONTENT_TYPE) + "\r");
    this->waitResponseUntil("OK", timeoutSecs);

    this->putATcmd("AT+QHTTPCFG=\"requestheader\",1\r");
    this->waitResponseUntil("OK", timeoutSecs);

    const std::string fullUrl = "http://" + host + uri;
    this->putATcmd("AT+QHTTPURL=" + std::to_string(fullUrl.length()) + "\r");
    this->waitResponseUntil("CONNECT", timeoutSecs);
    this->putATcmd(fullUrl);
    this->waitResponseUntil("OK", timeoutSecs);
    return true;
}

bool BG96::prepareHttpsPost (const std::string host, 
                             const std::string uri, 
                             const int CONTENT_TYPE, 
                             const int timeoutSecs) {
    this->putATcmd("AT+QHTTPCFG=\"contextid\",1\r");
    std::cout << this->waitResponseUntil("OK", timeoutSecs) << std::endl;

    this->putATcmd("AT+QHTTPCFG=\"contenttype\"," + std::to_string(CONTENT_TYPE) + "\r");
    std::cout << this->waitResponseUntil("OK", timeoutSecs) << std::endl;

    this->putATcmd("AT+QHTTPCFG=\"requestheader\",1\r");
    std::cout << this->waitResponseUntil("OK", timeoutSecs) << std::endl;

    // Set SSL context ID.
    this->putATcmd("AT+QHTTPCFG=\"sslctxid\",1\r");
    std::cout << this->waitResponseUntil("OK", timeoutSecs) << std::endl;

    // Set SSL version as 1 which means TLsV1.0.
    this->putATcmd("AT+QSSLCFG=\"sslversion\",1,1\r");
    std::cout << this->waitResponseUntil("OK", timeoutSecs) << std::endl;

    // Set SSL cipher suite as 0x0005 which means RC4-SHA.
    this->putATcmd("AT+QSSLCFG=\"ciphersuite\",1,0x0005\r");
    std::cout << this->waitResponseUntil("OK", timeoutSecs) << std::endl;

    /*
    // Set SSL verify level as 2 which means you should
    // upload CA certificate, client certificate and
    // client private key by AT+QFUPL command.
    this->putATcmd("AT+QSSLCFG=\"seclevel\",1,2\r");
    this->waitResponseUntil("OK", timeoutSecs);

    this->putATcmd("AT+QSSLCFG=\"cacert\",1,\"cacert.pem\"\r");
    this->waitResponseUntil("OK", timeoutSecs);

    this->putATcmd("AT+QSSLCFG=\"clientcert\",1,\"clientcert.pem\"\r");
    this->waitResponseUntil("OK", timeoutSecs);

    this->putATcmd("AT+QSSLCFG=\"clientkey\",1,\"clientkey.pem\"\r");
    this->waitResponseUntil("OK", timeoutSecs);
    */

    const std::string fullUrl = "https://" + host + uri;
    this->putATcmd("AT+QHTTPURL=" + std::to_string(fullUrl.length()) + "\r");
    this->waitResponseUntil("CONNECT", timeoutSecs);
    this->putATcmd(fullUrl);
    this->waitResponseUntil("OK", timeoutSecs);
    return true;
}

std::string BG96::postMultipart (const std::string host,
                                 const std::string uri,
                                 const HttpPostFormData& fields, 
                                 const int timeoutSecs) {
    // multipart/form-data boundary
    std::string boundary = "flesruoykoesoow";

    // Construct body of request form
    std::string body;
    for (int i = 0 ; i < fields.size() ; i ++) {
        std::string contentType = fields.getContentType(i);
        std::string contentName = fields.getContentName(i);
        std::string content = fields.getContent(i);
        if (contentType == "text/plain") {
            body += ("--" + boundary + "\r\n" + 
                    "Content-Type: " + contentType + "\r\n" + 
                    "Content-Disposition: form/data; name=\"" + contentName + "\"\r\n" + 
                    "\r\n" + 
                    content + "\r\n"
            );
        }
        else if (contentType == "image/jpeg") {
            body += ("--" + boundary + "\r\n" + 
                    "Content-Type: " + contentType + "\r\n" + 
                    "Content-Disposition: form/data; name=\"files\"; filename=\"" + contentName + "\"\r\n" + 
                    "\r\n" + 
                    content + "\r\n"
            );
        }
    }
    body += ("--" + boundary + "--\r\n");

    // Construct header of request form
    std::string header = (
        std::string("POST ") + uri + " HTTP/1.1\r\n" + 
        "Host: " + host + "\r\n" + 
        "Content-Length: " + std::to_string(body.length()) + "\r\n" +  
        "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n" + 
        "\r\n"
    );

    std::string maxInputBodyTime = std::to_string(timeoutSecs);
    std::string maxResponseTime = std::to_string(timeoutSecs);
    this->putATcmd("AT+QHTTPPOST="
                    + std::to_string(header.length() + body.length()) + ","
                    + maxInputBodyTime + "," 
                    + maxResponseTime + "\r");
    this->waitResponseUntil("CONNECT", timeoutSecs);
    this->putATcmd(header);
    this->putATcmd(body, body.length());
    this->waitResponseUntil("OK", timeoutSecs);

    this->putATcmd("AT+QHTTPREAD=80\r");
    return this->waitResponseUntil("CONNECT", timeoutSecs);
}