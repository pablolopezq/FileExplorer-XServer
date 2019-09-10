#include "App.h"

int main(){

    App app;

    app.create_window(720, 1280, "File Explorer | Pablo");
    app.create_graphical_context();
    app.draw_loop();

    return 0;
}