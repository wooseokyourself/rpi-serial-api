#include "../include/Common.hpp"
#include "../include/BG96.hpp"
#include "../include/HttpPostFormData.hpp"

using namespace std;

const string MODEM_PORT = "/dev/ttyS0";

int main (void) {
    BG96 modem(MODEM_PORT, 115200);
    cout << "BG96 ready" << endl;
    HttpPostFormData fields;

    /**
     * @brief Print RSSI.
     */
    cout << modem.getRssi() << endl;

    /**
     * @brief Send AT command to BG96 modem and print response.
     */
    modem.putATcmd("AT\r");
    cout << modem.waitResponseUntil("OK", 3) << endl;
    modem.putATcmd("ATE0\r");
    cout << modem.waitResponseUntil("OK", 3) << endl;
    modem.putATcmd("AT+CEREG=2\r");
    cout << modem.waitResponseUntil("OK", 3) << endl;

    /**
     * @brief Send AT command for http post multipart/form-data request to BG96 modem and print response.
     *          This is not supporting SSL.
     *          Check this site for example: http://ptsv2.com
     */
    const string HOST = "ptsv2.com";
    const string URI = "/t/nse4v-1614930112/post";
    string fileBytes;
    const string filePath = "bin/example.jpg";
    getFileBytes(fileBytes, filePath);
    fields.addField("text/plain", "time", "1996-03-05");
    fields.addField("text/plain", "rssi", to_string(modem.getRssi()));
    fields.addField("image/jpeg", "myImage.jpg", fileBytes);
    cout << modem.postMultipart(HOST, URI, fields, 20) << endl;
    fields.clear();

    return 0;
}