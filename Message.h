//
// Created by Shinan on 2019/1/19.
//

#ifndef SEVICEFRAMEWORK_MESSAGES_H
#define SEVICEFRAMEWORK_MESSAGES_H

#include <string>
using namespace std;

class Message {
#define HEADER_LEN 4 //表明随后payload字节数
public:
    Message();
    ~Message();

    bool isRetMsg_() const;

    void setRetMsg_(bool RetMsg_);

    const string &getFrom_() const;

    void setFrom_(const string &from_);

    const string &getTo_() const;

    void setTo_(const string &to_);

    int getId_() const;

    void setId_(int id_);

    const string &getData() const;

    void setData(const string &data);

private:
    bool   RetMsg_;
    string from_;
    string to_;
    int    id_;
    string data;

};


#endif //SEVICEFRAMEWORK_MESSAGES_H
