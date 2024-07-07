

#include "cfe.h"
#include "sample_app_msgids.h"

#ifndef SENSOR_SIM_APP_H
#define SENSOR_SIM_APP_H

typedef struct
{
  CFE_MSG_TelemetryHeader_t TelemetryHeader;

  /*
  ** Task command interface counters...
  */
  uint8          CmdCounter;
  uint8          ErrCounter;
  double         Temp;
} SIM_TempPacket_t;

typedef struct
{


    /*
    ** Run Status variable used in the main processing loop
    */
    uint32 RunStatus;

    /*
    ** Operational data (not reported in housekeeping)...
    */
    CFE_SB_PipeId_t DataPipe;

    /*
    ** Initialization data (not reported in housekeeping)...
    */
    char   PipeName[CFE_MISSION_MAX_API_LEN];
    uint16 PipeDepth;

    SIM_TempPacket_t message;

} SENSOR_SIM_APP_Data_t;


void         SENSOR_SIM_APP_Main(void);
CFE_Status_t SENSOR_SIM_APP_Init(void);

#endif /* SENSOR_SIM_APP_H */
