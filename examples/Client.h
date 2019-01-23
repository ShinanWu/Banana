//
// Created by Shinan on 2019/1/23.
//

#ifndef SEVICEFRAMEWORK_CLIENT_H
#define SEVICEFRAMEWORK_CLIENT_H

#include <vector>
#include "Message.h"

using namespace std;

class Client {
#define DEFAULT_BUFF_LEN (1024) //1k
#define RECV_BUFF_MAX_SIZE (1024 * 1024 * 500) //500M
#define SEND_BUFF_MAX_SIZE (1024 * 1024 * 100) //100M 均需根据实际业务作调整，比如即时通信客户端多，buffer小，取较小合适值即可
    enum {STARED, RECVING, RECVED, SENDING, SENDED, TOCLOSE, RECVHEAD, RECVBODY};
public:
    Client(int clientFd);

    ~Client();
    bool recvOnePck();
    bool getPck(vector<char>& recvPck);
    bool sendPck();
    bool setPck(const vector<char>& sendPck);

    int getFd_() const;

    void setFd_(int fd_);

    int getStat_() const;

    void setStat_(int stat_);

private:
    int fd_;
    int recvLen_;
    int needRecvLen_;
    int sendLen_;
    int needSendLen_;
    int stat_;
    vector<char> recvBuf_(DEFAULT_BUFF_LEN);
    vector<char> sendBuf_(DEFAULT_BUFF_LEN);
};


#endif //SEVICEFRAMEWORK_CLIENT_H
