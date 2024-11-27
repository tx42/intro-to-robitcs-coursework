#pragma once

#include <WiFiEsp.h>

class RemoteDebugger{
private:
    WiFiEspServer& m_server;
    WiFiEspClient m_client;

    bool checkClient();
    void sendPacket();
public:
    RemoteDebugger::RemoteDebugger(WiFiEspServer& server) : m_server(server) {}

    void print(String msg);
    void println(String msg);
    int available();
    char read();

    void plotPoint(float x, float y, int group);
    void clear(int group);
    void plotObject(int id, float x, float y, float angle);
    void removeObject(int id);

    void updatePart(int part_id, float measurement);
}
