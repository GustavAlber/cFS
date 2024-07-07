project(CFE_HELLO_APP2 C)

set(APP_SRC_FILES
  fsw/src/hello_app.c
)

# Create the app module
add_cfe_app(hello_app2 ${APP_SRC_FILES})

target_include_directories(hello_app2 PUBLIC fsw/inc)
