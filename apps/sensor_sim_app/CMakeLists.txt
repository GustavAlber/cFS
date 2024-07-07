project(CFE_SENSOR_SIM_APP C)

set(APP_SRC_FILES
  fsw/src/sensor_sim_app.c
)

# Create the app module
add_cfe_app(sensor_sim_app ${APP_SRC_FILES})

target_include_directories(sensor_sim_app PUBLIC fsw/inc)
