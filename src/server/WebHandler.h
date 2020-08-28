#include <string>
#include <vector>
#include "TcpServer.h"

#ifndef WEBHANDLER_H
#define WEBHANDLER_H

enum LAST_REQUEST_STATUS {
    NON_INITIALIZED, // at the beginning
    BAD,
    EMPTY,
    GOOD
};

class WebHandler {
    private:
        TcpServer *tcpServer;

        LAST_REQUEST_STATUS lastRequestStatus;
        std::string lastRequestEndpoint;
        std::string lastRequestType;
        std::vector<std::pair<std::string, std::string>> lastRequestParams;
        bool lastRequestHasContentRange;
        std::pair<std::uintmax_t, std::uintmax_t> lastRequestContentRange;


    public:
        WebHandler(std::string address, int port);
        ~WebHandler();

        std::string getRequestLine();
        std::vector<std::string> getRequestHeader();
        int parseRequest(std::vector<std::string> strSource);
        void handlerLoop();

        void sendForbiddenResponse();
        void sendMusicFile();
        void sendMusicDB();
        void sendHTTPOptions();
};

#endif
