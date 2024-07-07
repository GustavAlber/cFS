
#include "sensor_sim_app.h"
#include "sensor_sim_eventids.h"
#include "osapi.h"


/*
** global data
*/
SENSOR_SIM_APP_Data_t SENSOR_SIM_APP_Data;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/*                                                                            */
/* Application entry point and main process loop                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void SENSOR_SIM_APP_Main(void)
{
    CFE_Status_t     status;
    int count = 0;

    /*
    ** Perform application-specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = SENSOR_SIM_APP_Init();

    if (status != CFE_SUCCESS)
    {
        SENSOR_SIM_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    status = CFE_MSG_Init(CFE_MSG_PTR(SENSOR_SIM_APP_Data.message.TelemetryHeader), /* Location of SB Message Data Buffer */
                            CFE_SB_ValueToMsgId(0xA0),                     /* SB Message ID associated with Data */
                            sizeof(SENSOR_SIM_APP_Data.message));      /* Size of Buffer */

    /*
    ** Sample App Runloop
    */
    while (CFE_ES_RunLoop(&SENSOR_SIM_APP_Data.RunStatus) == true)
    {
        
        for(int i = 0; i < 3; i++)
        {
            CFE_SB_TimeStampMsg(CFE_MSG_PTR(SENSOR_SIM_APP_Data.message.TelemetryHeader));
            SENSOR_SIM_APP_Data.message.Temp = 26.12;
            if(count == 5)
            {
                SENSOR_SIM_APP_Data.message.Temp++;
                count = 0;
            }
            status = CFE_SB_TransmitMsg(CFE_MSG_PTR(SENSOR_SIM_APP_Data.message.TelemetryHeader),true);
            if (status != CFE_SUCCESS)
            {
                CFE_ES_WriteToSysLog("Temp Sim App MSG transmit error, RC = 0x%08lX\n", (unsigned long)status);
            }
        }
        count++;
        OS_TaskDelay(1000);


    }


    CFE_ES_PerfLogExit(0xFE);
    CFE_ES_ExitApp(SENSOR_SIM_APP_Data.RunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* Initialization                                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t SENSOR_SIM_APP_Init(void)
{
    CFE_Status_t status;
    char         VersionString[256] = " V0.000.0001";

    /* Zero out the global data structure */
    memset(&SENSOR_SIM_APP_Data, 0, sizeof(SENSOR_SIM_APP_Data));

    SENSOR_SIM_APP_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    * Events
    * mostly testing out if anything is working
    */

    status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Hello App: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
    }

    /*
    ** Pipeline
    */
    SENSOR_SIM_APP_Data.PipeDepth = 32;
    strncpy(SENSOR_SIM_APP_Data.PipeName, "SENSOR_SIM_APP_DIST_PIPE", sizeof(SENSOR_SIM_APP_Data.PipeName));
    SENSOR_SIM_APP_Data.PipeName[sizeof(SENSOR_SIM_APP_Data.PipeName) - 1] = 0;
    status = CFE_SB_CreatePipe(&SENSOR_SIM_APP_Data.DataPipe, SENSOR_SIM_APP_Data.PipeDepth, SENSOR_SIM_APP_Data.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SENSOR_SIM_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                            "hello App: Error creating SB Data Pipe, RC = 0x%08lX", (unsigned long)status);
    }
    if (status == CFE_SUCCESS)
    {
        status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0xA0), SENSOR_SIM_APP_Data.DataPipe);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(SENSOR_SIM_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                              "Hello App: Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)status);
        }
    }
	
    CFE_EVS_SendEvent(SENSOR_SIM_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, "sensor_sim_app Initialized.%s",
                          VersionString);
    return status;
}
