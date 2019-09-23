#include "App.h"
#include "FilesystemOps.h"

#include <iostream>

int main(){

    // FileOps::copy_dir("/home/pablo/Documents/Clases/GeneracionEmpresas1", "/home/pablo/Documents/Clases/test");

    App app;

    app.create_window(720, 1280, "File Explorer | Pablo");
    app.create_graphical_context();
    app.init();
    app.event_loop();

    return 0;
}