#include "labi.hpp"

int main() 
{
    spk::Application app = spk::Application("Labi", spk::Vector2UInt(800, 800), spk::Application::Mode::Monothread);

    MainWidget mainWidget = MainWidget("MainWidget"); 
    mainWidget.setGeometry(0, app.size());
    mainWidget.activate();

    return (app.run());  
}