#include <WiFiEsp.h>
#include "wifilog.h"

RemoteDebugger debugger("amogus", "12345678", 500);

void setup(){
    debugger.start();
    delay(3000);
}

void loop(){
    debugger.print("amogus");
}