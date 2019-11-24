#pragma once

#include "iot.h"
#include "messages.h"

class SendTest : public iot::Module
{
    private:

    public:
        void setup();
        void loop();
};

class ReceiveTest : public iot::Module
{
    private:

    public:
        void setup();
        void loop();
        void receiveTestMessage(ReceiveTestMessage* msg);
};

class InterceptorTest : public iot::Interceptor
{
    private: 

    public:
        void setup();
        void editTestMessage(ReceiveTestMessage *msg);
        bool filterTestMessage(TestMessage *msg);
        void testTranslateDirect(TestMessage* inMsg, ReceiveTestMessage *outMsg);
        void testTranslateEvent(ReceiveTestMessage *outMsg);
        void testTranslateOneToMany(TestMessage* inMsg);
};