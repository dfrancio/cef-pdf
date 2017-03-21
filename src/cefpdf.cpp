#include "cefpdf.h"

#include "Client.h"
#include "Job/Remote.h"
#include "Server/Server.h"

struct cefpdf::App
{
    CefRefPtr<cefpdf::Client> client;
    CefRefPtr<CefCommandLine> commandLine;
    App() : client(new cefpdf::Client)
    {
    }
};

struct cefpdf::MainArgs
{
    CefMainArgs args;
#if defined(OS_WIN)
    MainArgs(HINSTANCE hInstance) : args(hInstance)
    {
    }
#else
    MainArgs(int argc, char **argv) : args(argc, argv)
    {
    }
#endif // OS_WIN
};

struct cefpdf::CommandLine
{
    CefRefPtr<CefCommandLine> commandLine;
    CommandLine() : commandLine(CefCommandLine::CreateCommandLine())
    {
    }
};

struct cefpdf::PageSizeIterator
{
    cefpdf::PageSizes::const_iterator it;
    PageSizeIterator(cefpdf::PageSizes::const_iterator iterator) : it(iterator)
    {
    }
};

const std::string &cefpdf::GetLibVersion()
{
    return cefpdf::constants::version;
}

const std::string &cefpdf::GetDefaultPageSizeName()
{
    return cefpdf::constants::pageSize;
}

const std::string &cefpdf::GetDefaultServerHost()
{
    return cefpdf::constants::serverHost;
}

const std::string &cefpdf::GetDefaultServerPort()
{
    return cefpdf::constants::serverPort;
}

cefpdf::AppPtr cefpdf::CreateApp()
{
    return std::make_shared<App>();
}

cefpdf::MainArgsPtr cefpdf::CreateMainArgs(int argc, char **argv)
{
#if defined(OS_WIN)
    return std::make_shared<MainArgs>(::GetModuleHandle(NULL));
#else
    return std::make_shared<MainArgs>(argc, argv);
#endif // OS_WIN
}

cefpdf::CommandLinePtr cefpdf::CreateCommandLine(int argc, char **argv)
{
    CommandLinePtr result = std::make_shared<CommandLine>();
#if defined(OS_WIN)
    result->commandLine->InitFromString(::GetCommandLine());
#else
    result->commandLine->InitFromArgv(argc, argv);
#endif // OS_WIN
    return result;
}

cefpdf::PageSizeIteratorPtr cefpdf::CreatePageSizeIterator()
{
    return std::make_shared<cefpdf::PageSizeIterator>(cefpdf::pageSizes.begin());
}

std::string cefpdf::GetExecutableName(CommandLinePtr commandLine)
{
    std::string program = commandLine->commandLine->GetProgram().ToString();

    // Remove directory if present.
    // Do this before extension removal in case directory has a period character.
    const std::size_t s = program.find_last_of("\\/");
    if (std::string::npos != s)
    {
        program.erase(0, s + 1);
    }

    // Remove extension if present.
    const std::size_t e = program.rfind('.');
    if (std::string::npos != e)
    {
        program.erase(e);
    }

    return program;
}

bool cefpdf::GetNextPageSize(
    PageSizeIteratorPtr iterator, std::string &name, int &width, int &height)
{
    if (iterator->it != cefpdf::pageSizes.end())
    {
        name = iterator->it->name;
        width = iterator->it->width;
        height = iterator->it->height;
        ++iterator->it;
        return true;
    }
    return false;
}

bool cefpdf::HasSwitch(CommandLinePtr commandLine, const std::string &name)
{
    return commandLine->commandLine->HasSwitch(name);
}

int cefpdf::ExecuteSubProcess(AppPtr app, MainArgsPtr mainArgs)
{
#if !defined(OS_MACOSX)
    // Execute the sub-process logic, if any. This will either return immediately for the browser
    // process or block until the sub-process should exit.
    return app->client->ExecuteSubProcess(mainArgs->args);
#else
    return 0;
#endif // !OS_MACOSX
}

void cefpdf::Initialize(AppPtr app, MainArgsPtr mainArgs, CommandLinePtr commandLine)
{
    app->commandLine = commandLine->commandLine;
    app->client->Initialize(mainArgs->args);

    if (commandLine->commandLine->HasSwitch("javascript"))
        app->client->SetDisableJavaScript(false);
    else
        app->client->SetDisableJavaScript(true);
}

int cefpdf::RunJob(AppPtr app)
{
    CefRefPtr<cefpdf::job::Job> job;

    if (app->commandLine->HasSwitch("url"))
    {
        job = new cefpdf::job::Remote(app->commandLine->GetSwitchValue("url"));
    }
    else if (app->commandLine->HasSwitch("file"))
    {
        job = new cefpdf::job::Remote(
            cefpdf::pathToUri(app->commandLine->GetSwitchValue("file").ToString()));
    }
    else
    {
        app->client->Shutdown();
        throw std::string("no input specified");
    }

    // Set output file
    CefCommandLine::ArgumentList args;
    app->commandLine->GetArguments(args);
    job->SetOutputPath(args.empty() ? "output.pdf" : args[0]);

    if (app->commandLine->HasSwitch("size"))
        job->SetPageSize(app->commandLine->GetSwitchValue("size"));

    if (app->commandLine->HasSwitch("margin"))
        job->SetPageMargin(app->commandLine->GetSwitchValue("margin"));

    if (app->commandLine->HasSwitch("landscape"))
        job->SetLandscape();

    if (app->commandLine->HasSwitch("backgrounds"))
        job->SetBackgrounds();

    app->client->SetStopAfterLastJob(true);
    app->client->PostJob(job);
    app->client->Run();

    return 0;
}

int cefpdf::RunServer(AppPtr app)
{
    std::string port = cefpdf::constants::serverPort;
    if (app->commandLine->HasSwitch("port"))
        port = app->commandLine->GetSwitchValue("port").ToString();

    std::string host = cefpdf::constants::serverHost;
    if (app->commandLine->HasSwitch("host"))
        host = app->commandLine->GetSwitchValue("host").ToString();

    CefRefPtr<cefpdf::server::Server> server = new cefpdf::server::Server(app->client, host, port);
    server->Start();

    return 0;
}
