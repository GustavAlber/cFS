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
 * @file
 *
 * Main header file for the Sample application
 */

#include "cfe.h"
#include "sample_app_msgids.h"

#ifndef HELLO_APP_H
#define HELLO_APP_H

typedef struct
{
  CFE_MSG_TelemetryHeader_t TelemetryHeader;

  /*
  ** Task command interface counters...
  */
  uint8          CmdCounter;
  uint8          ErrCounter;
  bool           con[3];
} SIM_ConPacket_t;

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

    CFE_SB_PipeId_t RecPipe;
    char   RecPipeName[CFE_MISSION_MAX_API_LEN];

    CFE_SB_PipeId_t RecVote1Pipe;
    char   RecVote1PipeName[CFE_MISSION_MAX_API_LEN];

    CFE_SB_PipeId_t RecVote2Pipe;
    char   RecVote2PipeName[CFE_MISSION_MAX_API_LEN];

    CFE_SB_PipeId_t Data1Pipe;
    char   Pipe1Name[CFE_MISSION_MAX_API_LEN];
    uint16 Pipe1Depth;

    CFE_SB_PipeId_t Data2Pipe;
    char   Pipe2Name[CFE_MISSION_MAX_API_LEN];
    uint16 Pipe2Depth;

    SIM_TempPacket_t VotingMessage1;
    SIM_TempPacket_t VotingMessage2;

    SIM_ConPacket_t ConsentMessage1;
    SIM_ConPacket_t ConsentMessage2;


} HELLO_APP_Data_t;


void         HELLO_APP_Main(void);
CFE_Status_t HELLO_APP_Init(void);

#endif /* SAMPLE_APP_H */
