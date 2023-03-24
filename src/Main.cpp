#include "include/App.h"
#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/Dbo/Exception.h>
#include "include/Theme.h"
// Theme compiles with gulp version 4.0.2

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment &env)
{
	auto app = std::make_unique<Wt::WApplication>(env);
	// Set up application resourses and settings
	app->setTitle("Event Manager Lab");
	app->setTheme(std::make_shared<Theme>());
	// import resources
	// app->useStyleSheet("resources/styles/css/style.css");

	app->messageResourceBundle().use(app->appRoot() + "resources/templates/general");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/svgs");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/ProfilePage");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/ProfileSettings");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/EventData");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/Authentification");

	// app->require("resources/styles/script/script.js");

	// Application content
	app->root()->addWidget(std::make_unique<EventManagerLab>());

	return app;
}

int main(int argc, char **argv)
{
	try
	{
		// Server setup
		Wt::WServer server{argc, argv, WTHTTP_CONFIGURATION};

		server.addEntryPoint(Wt::EntryPointType::Application, createApplication);

		server.run();
	}
	catch (Wt::WServer::Exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (Wt::Dbo::Exception &e)
	{
		std::cerr << "Dbo exception: " << e.what() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << "exception: " << e.what() << std::endl;
	}
}