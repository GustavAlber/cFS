/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 *   This file contains the source code for the Sample App.
 */

/*
** Include Files:
*/
#include "hello_app.h"
#include "hello_app_eventids.h"
#include "osapi.h"


/*
** global data
*/
HELLO_APP_Data_t HELLO_APP_Data;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/*                                                                            */
/* Application entry point and main process loop                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void HELLO_APP_Main(void)
{
    CFE_Status_t     status;
    CFE_SB_Buffer_t *SBBufPtr;
    CFE_SB_Buffer_t *SBBufPtrVote1;
    CFE_SB_Buffer_t *SBBufPtrVote2;
    CFE_SB_Buffer_t *SBBufPtrCon1;
    CFE_SB_Buffer_t *SBBufPtrCon2;
    bool error1 = true;
    bool error2 = true;

    /*
    ** Perform application-specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = HELLO_APP_Init();

    /*Give time to initalize all the pipes*/
    OS_TaskDelay(100);

    if (status != CFE_SUCCESS)
    {
        HELLO_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    do
    {
        status = CFE_SB_GetPipeIdByName(&HELLO_APP_Data.RecPipe, HELLO_APP_Data.RecPipeName);
        if (status != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("Hello App0: Error reqesting pipeline, RC = 0x%08lX\n", (unsigned long)status);
        }
    }while (status != CFE_SUCCESS );

    /*
    * Initialize Voting messages
    */

    status = CFE_MSG_Init(CFE_MSG_PTR(HELLO_APP_Data.VotingMessage1.TelemetryHeader), /* Location of SB Message Data Buffer */
                            CFE_SB_ValueToMsgId(0xA1),                     /* SB Message ID associated with Data */
                            sizeof(HELLO_APP_Data.VotingMessage1));      /* Size of Buffer */

    status = CFE_MSG_Init(CFE_MSG_PTR(HELLO_APP_Data.VotingMessage2.TelemetryHeader), /* Location of SB Message Data Buffer */
                            CFE_SB_ValueToMsgId(0xA2),                     /* SB Message ID associated with Data */
                            sizeof(HELLO_APP_Data.VotingMessage2));      /* Size of Buffer */

    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0xA1), HELLO_APP_Data.Data1Pipe);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                            "Hello App: Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)status);
    }

    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0xA2), HELLO_APP_Data.Data2Pipe);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                            "Hello App: Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)status);
    }
    /*
    * Initialize Consent messages
    */
    status = CFE_MSG_Init(CFE_MSG_PTR(HELLO_APP_Data.ConsentMessage1.TelemetryHeader), /* Location of SB Message Data Buffer */
                            CFE_SB_ValueToMsgId(0xA3),                     /* SB Message ID associated with Data */
                            sizeof(HELLO_APP_Data.ConsentMessage1));      /* Size of Buffer */

    status = CFE_MSG_Init(CFE_MSG_PTR(HELLO_APP_Data.ConsentMessage2.TelemetryHeader), /* Location of SB Message Data Buffer */
                            CFE_SB_ValueToMsgId(0xA4),                     /* SB Message ID associated with Data */
                            sizeof(HELLO_APP_Data.ConsentMessage2));      /* Size of Buffer */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0xA3), HELLO_APP_Data.Data1Pipe);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                            "Hello App: Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)status);
    }
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0xA4), HELLO_APP_Data.Data2Pipe);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                            "Hello App: Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)status);
    }
    
    /*
    * Initialize Up messages
    */
    status = CFE_MSG_Init(CFE_MSG_PTR(HELLO_APP_Data.UpMessage1.TelemetryHeader), /* Location of SB Message Data Buffer */
                            CFE_SB_ValueToMsgId(0xA5),                     /* SB Message ID associated with Data */
                            sizeof(HELLO_APP_Data.UpMessage1));      /* Size of Buffer */

    status = CFE_MSG_Init(CFE_MSG_PTR(HELLO_APP_Data.UpMessage2.TelemetryHeader), /* Location of SB Message Data Buffer */
                            CFE_SB_ValueToMsgId(0xA6),                     /* SB Message ID associated with Data */
                            sizeof(HELLO_APP_Data.UpMessage2));      /* Size of Buffer */
    
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0xA5), HELLO_APP_Data.Data1Pipe);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                            "Hello App: Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)status);
    }
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0xA6), HELLO_APP_Data.Data2Pipe);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                            "Hello App: Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)status);
    }

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(HELLO_APP_Data.UpMessage1.TelemetryHeader));
    status = CFE_SB_TransmitMsg(CFE_MSG_PTR(HELLO_APP_Data.UpMessage1.TelemetryHeader),true);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("UP MSG transmit error, RC = 0x%08lX\n", (unsigned long)status);
    }
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(HELLO_APP_Data.UpMessage2.TelemetryHeader));
    status = CFE_SB_TransmitMsg(CFE_MSG_PTR(HELLO_APP_Data.UpMessage2.TelemetryHeader),true);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("UP MSG transmit error, RC = 0x%08lX\n", (unsigned long)status);
    }
    
    /*
    ** Sample App Runloop
    */
    while (CFE_ES_RunLoop(&HELLO_APP_Data.RunStatus) == true)
    {
        if(error1 == true)
        {
            status = CFE_SB_GetPipeIdByName(&HELLO_APP_Data.RecVote1Pipe, HELLO_APP_Data.RecVote1PipeName);
            if (status == CFE_SUCCESS)
            {
                status= CFE_SB_ReceiveBuffer(&SBBufPtr, HELLO_APP_Data.RecVote1Pipe,CFE_SB_POLL);
                if (status == CFE_SUCCESS)
                {
                    CFE_SB_MsgId_t MsgId;
                    SIM_TempPacket_t*rec = (SIM_TempPacket_t*)SBBufPtr;
                    CFE_MSG_GetMsgId((CFE_MSG_Message_t*)rec, &MsgId);
                    if(CFE_SB_MsgIdToValue(MsgId) == 0xB5)
                    {
                        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"up message 0xB5 received");
                        error1 = false;
                    }
                    else
                    {
                        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"different message1 received %x", CFE_SB_MsgIdToValue(MsgId));
                    }
                }
            }
        }
        if(error2 == true)
        {
            status = CFE_SB_GetPipeIdByName(&HELLO_APP_Data.RecVote2Pipe, HELLO_APP_Data.RecVote2PipeName);
            if (status == CFE_SUCCESS)
            {
                status= CFE_SB_ReceiveBuffer(&SBBufPtr, HELLO_APP_Data.RecVote2Pipe,CFE_SB_POLL);
                if (status == CFE_SUCCESS)
                {
                    CFE_SB_MsgId_t MsgId;
                    SIM_TempPacket_t*rec = (SIM_TempPacket_t*)SBBufPtr;
                    CFE_MSG_GetMsgId((CFE_MSG_Message_t*)rec, &MsgId);
                    if(CFE_SB_MsgIdToValue(MsgId) == 0xC5)
                    {
                        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"up message 0xC5 received");
                        error2 = false;
                    }
                    else
                    {
                        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"different message2 received %x", CFE_SB_MsgIdToValue(MsgId));
                    }
                }
            }
        }


        /* DATA INCOME */
        status = CFE_SB_ReceiveBuffer(&SBBufPtr, HELLO_APP_Data.RecPipe, CFE_SB_PEND_FOREVER);
        SIM_TempPacket_t* rec = (SIM_TempPacket_t*)SBBufPtr;
        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"data received %f", rec->Temp);

        /*Simulate processing*/
        osal_id_t task_id;
        const char task_name[254] = "runtime task";

        int32 task=
        OS_TaskCreate(&task_id,
                      task_name,
                      computing,
                      0,
                      4096,
                      10,
                      0 );

        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"task created %i", task);

        
        if ((error1 == false) && (error2 == false))
        {
            /*Sed data to siblings to vote*/
            CFE_SB_TimeStampMsg(CFE_MSG_PTR(HELLO_APP_Data.VotingMessage1.TelemetryHeader));
            HELLO_APP_Data.VotingMessage1.Temp = rec->Temp;
            status = CFE_SB_TransmitMsg(CFE_MSG_PTR(HELLO_APP_Data.VotingMessage1.TelemetryHeader),true);
            if (status != CFE_SUCCESS)
            {
                CFE_ES_WriteToSysLog("Temp Sim App MSG transmit error, RC = 0x%08lX\n", (unsigned long)status);
            }

            CFE_SB_TimeStampMsg(CFE_MSG_PTR(HELLO_APP_Data.VotingMessage2.TelemetryHeader));
            HELLO_APP_Data.VotingMessage2.Temp = rec->Temp;
            status = CFE_SB_TransmitMsg(CFE_MSG_PTR(HELLO_APP_Data.VotingMessage2.TelemetryHeader),true);
            if (status != CFE_SUCCESS)
            {
                CFE_ES_WriteToSysLog("Temp Sim App MSG transmit error, RC = 0x%08lX\n", (unsigned long)status);
            }

            /*Expect voting messages from siblings*/
            status = CFE_SB_ReceiveBuffer(&SBBufPtrVote1, HELLO_APP_Data.RecVote1Pipe, CFE_SB_PEND_FOREVER);
            rec = (SIM_TempPacket_t*)SBBufPtrVote1;
            //CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"hello App 0 Vote1 %f", rec->Temp);

            status = CFE_SB_ReceiveBuffer(&SBBufPtrVote2, HELLO_APP_Data.RecVote2Pipe, CFE_SB_PEND_FOREVER);
            rec = (SIM_TempPacket_t*)SBBufPtrVote2;
            //CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"hello App 0 Vote2 %f", rec->Temp);

            /*Vote*/
            double own = ((SIM_TempPacket_t*)SBBufPtr)->Temp;
            double app1 = ((SIM_TempPacket_t*)SBBufPtrVote1)->Temp;
            double app2 = ((SIM_TempPacket_t*)SBBufPtrVote2)->Temp;
            if((own == app1) && (own == app2))
            {
                CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"hello App 0 Votind ident");
                HELLO_APP_Data.ConsentMessage1.con[0] = true;
                HELLO_APP_Data.ConsentMessage1.con[1] = true;
                HELLO_APP_Data.ConsentMessage1.con[2] = true;
                HELLO_APP_Data.ConsentMessage2.con[0] = true;
                HELLO_APP_Data.ConsentMessage2.con[1] = true;
                HELLO_APP_Data.ConsentMessage2.con[2] = true;
            }
            else if(own == app1)
            {
                CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"hello App 0 Votind APP2 down %f", app2);
                HELLO_APP_Data.ConsentMessage1.con[0] = true;
                HELLO_APP_Data.ConsentMessage1.con[1] = true;
                HELLO_APP_Data.ConsentMessage1.con[2] = false;
                HELLO_APP_Data.ConsentMessage2.con[0] = true;
                HELLO_APP_Data.ConsentMessage2.con[1] = true;
                HELLO_APP_Data.ConsentMessage2.con[2] = false;
                error2 = true;
            }
            else if(own == app2)
            {
                CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"hello App 0 Votind APP1 down%f", app1);
                HELLO_APP_Data.ConsentMessage1.con[0] = true;
                HELLO_APP_Data.ConsentMessage1.con[1] = false;
                HELLO_APP_Data.ConsentMessage1.con[2] = true;
                HELLO_APP_Data.ConsentMessage2.con[0] = true;
                HELLO_APP_Data.ConsentMessage2.con[1] = false;
                HELLO_APP_Data.ConsentMessage2.con[2] = true;
                error1 = true;
            }
            else if(app2 == app1)
            {
                CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"hello App 0 Votind APP0 down%f", own);
                HELLO_APP_Data.ConsentMessage1.con[0] = false;
                HELLO_APP_Data.ConsentMessage1.con[1] = true;
                HELLO_APP_Data.ConsentMessage1.con[2] = true;
                HELLO_APP_Data.ConsentMessage2.con[0] = false;
                HELLO_APP_Data.ConsentMessage2.con[1] = true;
                HELLO_APP_Data.ConsentMessage2.con[2] = true;
            }
            else 
            {
                CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"hello App 0 Votind fucked");
                HELLO_APP_Data.ConsentMessage1.con[0] = false;
                HELLO_APP_Data.ConsentMessage1.con[1] = false;
                HELLO_APP_Data.ConsentMessage1.con[2] = false;
                HELLO_APP_Data.ConsentMessage2.con[0] = false;
                HELLO_APP_Data.ConsentMessage2.con[1] = false;
                HELLO_APP_Data.ConsentMessage2.con[2] = false;
                error1 = true;
                error2 = true;
            }
            
            /*Distribute voting result*/

            CFE_SB_TimeStampMsg(CFE_MSG_PTR(HELLO_APP_Data.ConsentMessage1.TelemetryHeader));
            status = CFE_SB_TransmitMsg(CFE_MSG_PTR(HELLO_APP_Data.ConsentMessage1.TelemetryHeader),true);
            if (status != CFE_SUCCESS)
            {
                CFE_ES_WriteToSysLog("Temp Sim App MSG transmit error, RC = 0x%08lX\n", (unsigned long)status);
            }

            CFE_SB_TimeStampMsg(CFE_MSG_PTR(HELLO_APP_Data.ConsentMessage2.TelemetryHeader));
            status = CFE_SB_TransmitMsg(CFE_MSG_PTR(HELLO_APP_Data.ConsentMessage2.TelemetryHeader),true);
            if (status != CFE_SUCCESS)
            {
                CFE_ES_WriteToSysLog("Temp Sim App MSG transmit error, RC = 0x%08lX\n", (unsigned long)status);
            }

            if(error1)
            {
                CFE_SB_TimeStampMsg(CFE_MSG_PTR(HELLO_APP_Data.UpMessage1.TelemetryHeader));
                status = CFE_SB_TransmitMsg(CFE_MSG_PTR(HELLO_APP_Data.UpMessage1.TelemetryHeader),true);
                if (status != CFE_SUCCESS)
                {
                    CFE_ES_WriteToSysLog("UP MSG transmit error, RC = 0x%08lX\n", (unsigned long)status);
                }
            }
            if(error2)
            {
                CFE_SB_TimeStampMsg(CFE_MSG_PTR(HELLO_APP_Data.UpMessage2.TelemetryHeader));
                status = CFE_SB_TransmitMsg(CFE_MSG_PTR(HELLO_APP_Data.UpMessage2.TelemetryHeader),true);
                if (status != CFE_SUCCESS)
                {
                    CFE_ES_WriteToSysLog("UP MSG transmit error, RC = 0x%08lX\n", (unsigned long)status);
                }
            }

            /*Consense?*/
            SIM_ConPacket_t* comp1 = (SIM_ConPacket_t*)SBBufPtrCon1;
            SIM_ConPacket_t* comp2 = (SIM_ConPacket_t*)SBBufPtrCon2;
            status = CFE_SB_ReceiveBuffer(&SBBufPtrCon1, HELLO_APP_Data.RecVote1Pipe, CFE_SB_PEND_FOREVER);
            comp1 = (SIM_ConPacket_t*)SBBufPtrCon1;
            //CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"con1 [%d] [%d] [%d]", comp1->con[0], comp1->con[1], comp1->con[2]);
        
            status = CFE_SB_ReceiveBuffer(&SBBufPtrCon2, HELLO_APP_Data.RecVote2Pipe, CFE_SB_PEND_FOREVER);
            comp2 = (SIM_ConPacket_t*)SBBufPtrCon2;
            //CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"con2 [%d] [%d] [%d]", comp2->con[0], comp2->con[1], comp2->con[2]);


            for(int i = 0; i < 3; i++)
            {
                if((comp1->con[i] != HELLO_APP_Data.ConsentMessage1.con[i]) || (comp2->con[i] != HELLO_APP_Data.ConsentMessage1.con[i]))
                {
                    CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"Consens BROKE!!");
                }
            }

            if(HELLO_APP_Data.ConsentMessage1.con[0] == true)
            {
                CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"Use APP0 result");
            }
            else
            {
                CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"Use APP1 result");
            }

            if(HELLO_APP_Data.ConsentMessage1.con[0] == false)
            {
                char                               LocalApp[OS_MAX_API_NAME] = "HELLO_APP0";
                CFE_ES_AppId_t                     AppID;
                int32                              Result;
                Result = CFE_ES_GetAppIDByName(&AppID, LocalApp);
                if (Result == CFE_SUCCESS)
                {
                    Result = CFE_ES_RestartApp(AppID);
                }
            }


        }
        else
        {
            CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,"We are in error mode");

        }

    }
            

    



    CFE_ES_PerfLogExit(0xFE);
    CFE_ES_ExitApp(HELLO_APP_Data.RunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* Initialization                                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t HELLO_APP_Init(void)
{
    CFE_Status_t status;
    char         VersionString[256] = " V0.000.0001";

    /* Zero out the global data structure */
    memset(&HELLO_APP_Data, 0, sizeof(HELLO_APP_Data));

    HELLO_APP_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    * Events
    * mostly testing out if anything is working
    */

    status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Hello App0: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
    }

    /*pipe to receive data*/
    strncpy(HELLO_APP_Data.RecPipeName, "SENSOR_SIM_APP_DIST_PIPE", sizeof(HELLO_APP_Data.RecPipeName));
    HELLO_APP_Data.RecPipeName[sizeof(HELLO_APP_Data.RecPipeName) - 1] = 0;

    strncpy(HELLO_APP_Data.RecVote1PipeName, "HELLO_APP_10_PIPE", sizeof(HELLO_APP_Data.RecVote1PipeName));
    HELLO_APP_Data.RecVote1PipeName[sizeof(HELLO_APP_Data.RecVote1PipeName) - 1] = 0;

    strncpy(HELLO_APP_Data.RecVote2PipeName, "HELLO_APP_20_PIPE", sizeof(HELLO_APP_Data.RecVote2PipeName));
    HELLO_APP_Data.RecVote2PipeName[sizeof(HELLO_APP_Data.RecVote2PipeName) - 1] = 0;



    /*Two pipes to send data to siblings for majority vote*/
    /*
    ** Pipeline1
    */
    HELLO_APP_Data.Pipe1Depth = 32;
    strncpy(HELLO_APP_Data.Pipe1Name, "HELLO_APP_01_PIPE", sizeof(HELLO_APP_Data.Pipe1Name));
    HELLO_APP_Data.Pipe1Name[sizeof(HELLO_APP_Data.Pipe1Name) - 1] = 0;
    status = CFE_SB_CreatePipe(&HELLO_APP_Data.Data1Pipe, HELLO_APP_Data.Pipe1Depth, HELLO_APP_Data.Pipe1Name);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                            "hello App: Error creating SB HELLO_APP_01_PIPE, RC = 0x%08lX", (unsigned long)status);
    }

    /*
    ** Pipeline2
    */
    HELLO_APP_Data.Pipe2Depth = 32;
    strncpy(HELLO_APP_Data.Pipe2Name, "HELLO_APP_02_PIPE", sizeof(HELLO_APP_Data.Pipe2Name));
    HELLO_APP_Data.Pipe2Name[sizeof(HELLO_APP_Data.Pipe2Name) - 1] = 0;
    status = CFE_SB_CreatePipe(&HELLO_APP_Data.Data2Pipe, HELLO_APP_Data.Pipe2Depth, HELLO_APP_Data.Pipe2Name);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_ERROR,
                            "hello App: Error creating SB HELLO_APP_02_PIPE, RC = 0x%08lX", (unsigned long)status);
    }

    

    CFE_EVS_SendEvent(HELLO_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, "hello App 0 Initialized.%s",
                          VersionString);
    return status;
}

void computing(void)
{
    CFE_ES_WriteToSysLog("computing now for 3s seperate thread");
    OS_TaskDelay(2000);
    CFE_ES_WriteToSysLog("exiting computing thread");
    OS_TaskExit();
}