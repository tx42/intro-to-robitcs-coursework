#include "wifilog.h"

#define MAX_PACKET_SIZE 1024

// defining streams
#define TEXT_STREAM_ID 0
#define PLOT_STREAM_ID 1
#define PART_STREAM_ID 2

// defining commands
#define CMD_TXT_PRINT 0

#define CMD_PLT_POINT 0
#define CMD_PLT_CLEAR 1
#define CMD_PLT_PLOT_OBJ 2
#define CMD_PLT_RM_OBJ 3

#define CMD_PAR_UPDATE 0
#define CMD_PAR_PROPOSE 1

bool RemoteDebugger::checkClient(){
    if(!m_client){
        // attempt to establish the client
        m_client = m_server.available();
    }

    return m_client;
}

char packetBuffer[MAX_PACKET_SIZE];
size_t packetSize;

/* LOW-LEVEL STUFF */
// adds new field to the packet
#define addPacketField(x) addVariablePacketField(&x, sizeof(x))

void addVariablePacketField(void* data_ptr, size_t len){
    if(packetSize + len > MAX_PACKET_SIZE){
        return;
    }

    memcpy(packetBuffer + packetSize, data_ptr, len);
    packetSize += len;
}

void newPacket(char stream_id, char command_id){
    packetSize = 0;
    
    addPacketField(stream_id);
    addPacketField(command_id);
}

void RemoteDebugger::sendPacket(){
    if(!checkClient()) return;

    // sending a packet
    m_client.write(packetBuffer, packetSize);
}

/* TEXT CONNECTION */
void RemoteDebugger::print(String msg){
    newPacket(TEXT_STREAM_ID, CMD_TXT_PRINT);

    addVariablePacketField(msg.c_str(), msg.length());
    sendPacket();
}

void RemoteDebugger::println(String msg){
    print(msg);
    print("\n");
}

int RemoteDebugger::available(){
    if(!checkClient()) return 0;

    return m_client.available();
}

char RemoteDebugger::read(){
    if(!checkClient()) return 0;

    return m_client.read();
}

/* PLOTTING FUNCITONS */
void RemoteDebugger::plotPoint(float x, float y, int group){
    newPacket(PLOT_STREAM_ID, CMD_PLT_POINT);

    addPacketField(x);
    addPacketField(y);
    addPacketField(group);

    sendPacket();
}

void RemoteDebugger::clear(int group){
    newPacket(PLOT_STREAM_ID, CMD_PLT_CLEAR);

    addPacketField(group);
    sendPacket();
}

void RemoteDebugger::plotObject(int id, float x, float y, float angle){
    newPacket(PLOT_STREAM_ID, CMD_PLT_PLOT_OBJ);

    addPacketField(id);
    addPacketField(x);
    addPacketField(y);
    addPacketField(angle);

    sendPacket();
}

void RemoteDebugger::removeObject(int id){
    newPacket(PLOT_STREAM_ID, CMD_PLT_RM_OBJ);

    addPacketField(id);

    sendPacket();
}

/* SENSOR FUNCTIONS */
void RemoteDebugger::updatePart(int part_id, float measurement){
    newPacket(PART_STREAM_ID, CMD_PAR_UPDATE);

    addPacketField(part_id);
    addPacketField(measurement);

    sendPacket();
}

void RemoteDebugger::proposePartName(int part_id, const char* name){
    newPacket(PART_STREAM_ID, CMD_PAR_PROPOSE);

    addPacketField(part_id);
    addVariablePacketField(name, strlen(name));

    sendPacket();
}
