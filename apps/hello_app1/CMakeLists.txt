project(CFE_HELLO_APP1 C)

set(APP_SRC_FILES
  fsw/src/hello_app.c
)

# Create the app module
add_cfe_app(hello_app1 ${APP_SRC_FILES})

target_include_directories(hello_app1 PUBLIC fsw/inc)
