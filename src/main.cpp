#include "App.h"

#include <iostream>

int main(){

    App app;

    app.create_window(720, 1280, "File Explorer | Pablo");
    app.create_graphical_context();
    app.init();
    app.event_loop();

    return 0;
}