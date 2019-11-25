#include <Arduino.h>
#include "modules/test.h"

void SendTest::setup()
{

}

void SendTest::loop()
{
    delay(1000);
    Serial.println("Sending test message");
    send<TestMessage>(1);
    send<TestMessage>(2);
    send<TestMessage>(3);
}

void ReceiveTest::setup()
{
    subscribe<ReceiveTestMessage>(&ReceiveTest::receiveTestMessage);
}

void ReceiveTest::loop()
{

}

void ReceiveTest::receiveTestMessage(ReceiveTestMessage* msg)
{
    Serial.print("Received value: ");
    Serial.println(msg->val);
}

void InterceptorTest::setup()
{
    //edit<ReceiveTestMessage>(&InterceptorTest::editTestMessage);
    
    //translate<TestMessage, ReceiveTestMessage>(&InterceptorTest::testTranslateDirect);
    //translate<TestMessage>(&InterceptorTest::testTranslateEvent);
    translate<TestMessage>(&InterceptorTest::testTranslateOneToMany);

    //filter<TestMessage>(&InterceptorTest::filterTestMessage);

}

void InterceptorTest::editTestMessage(ReceiveTestMessage* msg)
{
    if(msg->val == 3)
        msg->val = 4;
}

bool InterceptorTest::filterTestMessage(TestMessage* msg)
{
    if(msg->val == 3)
        return true;
    return false;
}

void InterceptorTest::testTranslateDirect(TestMessage* inMsg, ReceiveTestMessage* outMsg)
{
    outMsg->val = inMsg->val + 1;
}

void InterceptorTest::testTranslateEvent(ReceiveTestMessage* outMsg)
{
    outMsg->val = 1;
}

void InterceptorTest::testTranslateOneToMany(TestMessage *inMsg)
{
    switch(inMsg->val)
    {
        case 1:
            send<ReceiveTestMessage>(-1);
            break;
        default:
            send<ReceiveTestMessage>(inMsg->val + 1);
            break;
    }
}