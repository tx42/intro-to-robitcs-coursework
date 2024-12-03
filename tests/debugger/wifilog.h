#pragma once

#include <Arduino.h>
#include <WiFiEsp.h>

class RemoteDebugger{
private:
    WiFiEspServer m_server;
    WiFiEspClient m_client;

    const char* m_ssid;
    const char* m_pass;
    int m_port;

    int m_status = WL_IDLE_STATUS;
    int m_req_count = 0;

    bool checkClient();
    void sendPacket();
public:
    RemoteDebugger(const char* ssid, const char* pass, int port) : 
                    m_ssid(ssid), m_pass(pass), m_port(port), m_server(port) {};

    int start();

    void print(String msg);
    void println(String msg);
    int available();
    char read();

    void plotPoint(float x, float y, int group);
    void clear(int group);
    void plotObject(int id, float x, float y, float angle);
    void removeObject(int id);

    void updatePart(int part_id, float measurement);
    void proposePartName(int part_id, const char* name);
};
