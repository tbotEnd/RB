#include "nnxt.h"
#include "nnxt_pcf8574lb.h"
#include "event.h"

void sendAddress(){
    if(eventIsSet(&eventType, newByteEV)){
        clearEvent(&eventType, newByteEV);
        int i = 0;
        //warte erste 4 bites ab
        while(i < 3){
            if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);
                ++i;
            }
        }
        //schreibe 3. bit
        if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);
                WritePort(Port_1, 15);
        }
        if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);
                DeletePort(Port_1, 15);
        }
        //warte nächste 2 bit
        if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);
        }
        if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);
        }

        //setze schreib bit
        if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);
                WritePort(Port_1, 15);
        }
        if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);
                DeletePort(Port_1, 15);
        }

    }
}



void sclTact(){
    while(1){
    setEvent(&eventType, newByteEV);
    for(int i = 0; i < 9; i++){
        WritePort(Port_1, 14);
        Delay(50);
        DeletePort(Port_1, 14);
        Delay(50);
        setEvent(&eventType, sclLowEV);
    }
    //ack
    WritePort(Port_1, 14);
    Delay(50);
    DeletePort(Port_1, 14);
    Delay(50);
    }
}

void sdaPulse(){
   if(eventIsSet(&eventType, newByteEV)){
        clearEvent(&eventType, newByteEV);
        int i = 0;
        //warte erste 4 bits ab
        while(i < 6){
            if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);
                ++i;
            }
        }
        if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);
                setEvent(&eventType, setBlinkEV);
        }
        if(eventIsSet(&eventType, sclLowEV)){
                clearEvent(&eventType, sclLowEV);

        }

    }
}

void blink(){
    while(1){
        if(eventIsSet(&eventType, setBlinkEV)){
            clearEvent(&eventType, setBlinkEV);
            WritePort(Port_1, 15);
            Delay(500);
        }
    }
}

int main(){

    SensorPortInit(Port_1);
    WritePort(Port_1, 14);
    WritePort(Port_1, 15);
    Delay(500);
    DeletePort(Port_1, 15);
    DeletePort(Port_1, 14);
    CreateAndStartTask(sclTact);
    sendAddress();
    CreateAndStartTask(sdaPulse);
    CreateAndStartTask(blink);



   	return 0;
}

