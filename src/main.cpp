#include "App.h"

#include <iostream>

int main(){

    App app;

    //app.load_directories();
    app.init();
    app.create_window(720, 1280, "File Explorer | Pablo");
    app.create_graphical_context();
    app.event_loop();

    return 0;
}