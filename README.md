
## Known issues

The internal structure consists of four Apps:

1. hello_app0
2. hello_app1
3. hello_app2
4. sensor_sim_app


sensor_sim_app delivers sensor data through a pipe to all hello_app*. Each five seconds one "faulty" value is generated. The faulty value will be randomly distributed to one of the hello_app*. All hello_app* run in parallel and communicate with each other over pipes. They will internally compare the values, vote in comparison with each other and try to consent on one app to deliver the result.

A correct output will look like:
```
  EVS Port1 1980-012-14:03:21.70680 66/1/HELLO_APP0 1: hello App 0 Votind ident
  EVS Port1 1980-012-14:03:21.70686 66/1/HELLO_APP2 1: hello App 2 Votind ident
  EVS Port1 1980-012-14:03:21.70703 66/1/HELLO_APP1 1: hello App 1 Votind ident
  EVS Port1 1980-012-14:03:21.70866 66/1/HELLO_APP0 1: Use APP0 result
  EVS Port1 1980-012-14:03:21.70878 66/1/HELLO_APP2 1: Use APP0 result
  EVS Port1 1980-012-14:03:21.70903 66/1/HELLO_APP1 1: Use APP0 result
```

As soon as one of the applications receives an incorrect value, the Instances might consent to use other apps' results. The faulty app will through the voting process recognize its incorrect behavior and will initiate a restart. While restarting the Program will switch to error mode and the values will be handed through. After the faulty app is restarted, it will communicate its recovery and the Programm should continue as normal:

A correct output will look like:  
```
  EVS Port1 1980-012-14:03:24.70875 66/1/HELLO_APP1 1: hello App 1 Votind APP0 down27.120000
  EVS Port1 1980-012-14:03:24.70878 66/1/HELLO_APP0 1: hello App 0 Votind APP0 down27.120000
  EVS Port1 1980-012-14:03:24.70878 66/1/HELLO_APP2 1: hello App 2 Votind APP0 down27.120000
  EVS Port1 1980-012-14:03:24.70886 66/1/HELLO_APP0 1: Use APP1 result
  EVS Port1 1980-012-14:03:24.70887 66/1/HELLO_APP1 1: Use APP1 result
  1980-012-14:03:24.70890 CFE_ES_RestartApp: Restart Application HELLO_APP0 Initiated
  EVS Port1 1980-012-14:03:24.70895 66/1/HELLO_APP2 1: Use APP1 result
  1980-012-14:03:24.70910 CFE_ES_ExitApp: Application HELLO_APP0 called CFE_ES_ExitApp
  EVS Port1 1980-012-14:03:24.70919 66/1/HELLO_APP1 1: data received 26.120000
  EVS Port1 1980-012-14:03:24.70922 66/1/HELLO_APP2 1: data received 26.120000
  EVS Port1 1980-012-14:03:24.80934 66/1/HELLO_APP2 1: We are in error mode
  EVS Port1 1980-012-14:03:24.80935 66/1/HELLO_APP1 1: We are in error mode
  EVS Port1 1980-012-14:03:25.46072 66/1/CFE_ES 10: Restart Application HELLO_APP0 Completed, AppID=1114123
  EVS Port1 1980-012-14:03:25.51100 66/1/HELLO_APP0 1: hello App 0 Initialized. V0.000.0001
  EVS Port1 1980-012-14:03:25.61128 66/1/HELLO_APP0 1: up message 0xB5 received
  EVS Port1 1980-012-14:03:25.61131 66/1/HELLO_APP0 1: up message 0xC5 received
  EVS Port1 1980-012-14:03:25.70877 66/1/HELLO_APP2 1: data received 26.120000
  EVS Port1 1980-012-14:03:25.70880 66/1/HELLO_APP1 1: data received 26.120000
  EVS Port1 1980-012-14:03:25.70884 66/1/HELLO_APP0 1: data received 26.120000
  EVS Port1 1980-012-14:03:25.70895 66/1/HELLO_APP0 1: task created 0
  EVS Port1 1980-012-14:03:25.80903 66/1/HELLO_APP1 1: We are in error mode
  EVS Port1 1980-012-14:03:25.80903 66/1/HELLO_APP2 1: We are in error mode
  EVS Port1 1980-012-14:03:25.80912 66/1/HELLO_APP2 1: up message 0xA6 received
  EVS Port1 1980-012-14:03:25.80912 66/1/HELLO_APP1 1: up message 0xA5 received
  EVS Port1 1980-012-14:03:26.70896 66/1/HELLO_APP2 1: data received 26.120000
  EVS Port1 1980-012-14:03:26.70903 66/1/HELLO_APP1 1: data received 26.120000
  EVS Port1 1980-012-14:03:26.80929 66/1/HELLO_APP0 1: hello App 0 Votind ident
  EVS Port1 1980-012-14:03:26.80931 66/1/HELLO_APP2 1: hello App 2 Votind ident
  EVS Port1 1980-012-14:03:26.80937 66/1/HELLO_APP1 1: hello App 1 Votind ident
  EVS Port1 1980-012-14:03:26.80944 66/1/HELLO_APP2 1: Use APP0 result
  EVS Port1 1980-012-14:03:26.80944 66/1/HELLO_APP1 1: Use APP0 result
  EVS Port1 1980-012-14:03:26.80946 66/1/HELLO_APP2 1: data received 26.120000
  EVS Port1 1980-012-14:03:26.80950 66/1/HELLO_APP0 1: Use APP0 result
```

<span style="color:red; font-size:2em">
However, recovery is a little unstable. I think it's mostly through synchronisation issues.
A good indicator for a catastrophe is a broken consens:
</span>

```
  EVS Port1 1980-012-14:03:29.81019 66/1/HELLO_APP2 1: Consens BROKE!!
  EVS Port1 1980-012-14:03:29.81021 66/1/HELLO_APP2 1: Consens BROKE!!
  EVS Port1 1980-012-14:03:29.81022 66/1/HELLO_APP2 1: Use APP0 result
  EVS Port1 1980-012-14:03:29.81024 66/1/HELLO_APP0 1: Consens BROKE!!
  EVS Port1 1980-012-14:03:29.81039 66/1/HELLO_APP1 1: Consens BROKE!!
  EVS Port1 1980-012-14:03:29.81069 66/1/HELLO_APP0 1: Consens BROKE!!
  EVS Port1 1980-012-14:03:29.81095 66/1/HELLO_APP1 1: Consens BROKE!!
  EVS Port1 1980-012-14:03:29.81112 66/1/HELLO_APP2 1: data received 26.120000
  EVS Port1 1980-012-14:03:29.81117 66/1/HELLO_APP0 1: Use APP0 result
  EVS Port1 1980-012-14:03:29.81121 66/1/HELLO_APP1 1: Use APP0 result
  1980-012-14:03:29.81146 CFE_ES_RestartApp: Restart Application HELLO_APP1 Initiated
  1980-012-14:03:29.81147 CFE_ES_ExitApp: Application HELLO_APP1 called CFE_ES_ExitApp
  EVS Port1 1980-012-14:03:29.91156 66/1/HELLO_APP2 1: We are in error mode
  EVS Port1 1980-012-14:03:30.70999 66/1/HELLO_APP2 1: data received 26.120000
  EVS Port1 1980-012-14:03:30.71002 66/1/HELLO_APP0 1: data received 26.120000
  EVS Port1 1980-012-14:03:30.71008 66/1/HELLO_APP0 1: task created 0
  EVS Port1 1980-012-14:03:30.71008 66/1/HELLO_APP0 1: We are in error mode
  EVS Port1 1980-012-14:03:30.71009 66/1/HELLO_APP0 1: data received 26.120000
  EVS Port1 1980-012-14:03:30.71009 66/1/HELLO_APP0 1: task created -15
  EVS Port1 1980-012-14:03:30.71011 66/1/HELLO_APP0 1: We are in error mode
  1980-012-14:03:30.71013 computing now for 3s seperate thread
  EVS Port1 1980-012-14:03:30.71224 66/1/CFE_ES 10: Restart Application HELLO_APP1 Completed, AppID=1114124
  EVS Port1 1980-012-14:03:30.76255 66/1/HELLO_APP1 1: hello App 0 Initialized. V0.000.0001
  EVS Port1 1980-012-14:03:30.81024 66/1/HELLO_APP2 1: We are in error mode
  EVS Port1 1980-012-14:03:30.86279 66/1/HELLO_APP1 1: up message 0xC5 received
  EVS Port1 1980-012-14:03:31.71020 66/1/HELLO_APP2 1: data received 26.120000
  EVS Port1 1980-012-14:03:31.71025 66/1/HELLO_APP0 1: data received 26.120000
  EVS Port1 1980-012-14:03:31.71029 66/1/HELLO_APP0 1: task created -15
  EVS Port1 1980-012-14:03:31.71031 66/1/HELLO_APP0 1: We are in error mode
```
At that point we will stay in error mode forever.






## Setup

Ensure the following software are installed: Make, CMake, GCC, and Git.  To setup the cFS BUNDLE directly from the latest set of interoperable repositories:

    git clone https://github.com/GustavAlber/cFS.git
    cd cFS
    git submodule init
    git submodule update

Copy in the default makefile and definitions:

    cp cfe/cmake/Makefile.sample Makefile
    cp -r cfe/cmake/sample_defs sample_defs

## Build and Run

    make SIMULATION=native prep
    make
    make install
    cd build/exe/cpu1/
    ./core-cpu1



