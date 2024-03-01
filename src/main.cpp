#include "sparkle.hpp"

#include "singleton_instanciator.hpp"
#include "main_widget.hpp"

int main()
{ 
    spk::Application app = spk::Application("Labi", spk::Vector2UInt(800, 800), spk::Application::Mode::Multithread);

    MainWidget mainWidget = MainWidget("MainWidget");
    mainWidget.setGeometry(0, app.size());
    mainWidget.activate();

    return (app.run());  
}