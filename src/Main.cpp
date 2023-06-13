#include "include/App.h"
#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/Dbo/Exception.h>

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment &env)
{
	auto app = std::make_unique<Wt::WApplication>(env);
	// Set up application resourses and settings
	app->setTitle("Event Manager Lab");

	// include tailwind css file
	app->useStyleSheet("resources/themes/tailwind/dist/tailwind.css");
	
	// import resources
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/general");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/svgs");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/ProfilePage");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/ProfileSettings");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/EventData");
	app->messageResourceBundle().use(app->appRoot() + "resources/templates/Authentification");

	app->messageResourceBundle().use(app->appRoot() + "resources/xml/Auth");
	app->messageResourceBundle().use(app->appRoot() + "resources/xml/General");
	app->messageResourceBundle().use(app->appRoot() + "resources/xml/Navbar");
	app->messageResourceBundle().use(app->appRoot() + "resources/xml/HomePage");
	
	// app->require("resources/Js/CopyToClipboard.js");
	app->require("resources/Js/Utility.js");
	app->require("https://cdn.jsdelivr.net/npm/alpinejs@3.x.x/dist/cdn.min.js");

	
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