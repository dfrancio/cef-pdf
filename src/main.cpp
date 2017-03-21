#include <iostream>
#include <string>

#include "cefpdf.h"

#if defined(_MSC_VER)
#pragma comment(linker, "/entry:mainCRTStartup /subsystem:console")
#endif // _MSC_VER

void printSizes()
{
    cefpdf::PageSizeIteratorPtr iterator = cefpdf::CreatePageSizeIterator();
    std::string name;
    int width, height;
    while (cefpdf::GetNextPageSize(iterator, name, width, height))
        std::cout << name << " " << width << "x" << height << std::endl;
}

void printHelp(std::string name)
{
    std::cout << name << " v" << cefpdf::GetLibVersion() << std::endl;
    std::cout << "  Creates PDF files from HTML pages" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  cef-pdf [options] --url=<url>|--file=<path> [output]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --help -h        This help screen." << std::endl;
    std::cout << "  --url=<url>      URL to load, may be http, file, data, anything supported by "
                 "Chromium."
              << std::endl;
    std::cout << "  --file=<path>    File path to load using file:// scheme. May be relative to "
                 "current directory."
              << std::endl;
    std::cout << "  --size=<spec>    Size (format) of the paper: A3, B2.. or custom "
                 "<width>x<height> in mm."
              << std::endl;
    std::cout << "                   " << cefpdf::GetDefaultPageSizeName() << " is the default."
              << std::endl;
    std::cout << "  --list-sizes     Show all defined page sizes." << std::endl;
    std::cout << "  --landscape      Wheather to print with a landscape page orientation."
              << std::endl;
    std::cout << "                   Default is portrait" << std::endl;
    std::cout << "  --margin=<spec>  Paper margins in mm (much like CSS margin but without units)"
              << std::endl;
    std::cout << "                   If omitted some default margin is applied." << std::endl;
    std::cout << "  --javascript     Enable JavaScript." << std::endl;
    std::cout << "  --backgrounds    Print with backgrounds." << std::endl;
    std::cout << std::endl;
    std::cout << "Server options:" << std::endl;
    std::cout << "  --server         Start HTTP server" << std::endl;
    std::cout << "  --host=<host>    If starting server, specify ip address to bind to."
              << std::endl;
    std::cout << "                   Default is " << cefpdf::GetDefaultServerHost() << std::endl;
    std::cout << "  --port=<port>    Specify server port number. Default is "
              << cefpdf::GetDefaultServerPort() << std::endl;
    std::cout << std::endl;
    std::cout << "Output:" << std::endl;
    std::cout << "  PDF file name to create. Default is output.pdf" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    cefpdf::AppPtr app = cefpdf::CreateApp();
    cefpdf::MainArgsPtr mainArgs = cefpdf::CreateMainArgs(argc, argv);

    int exitCode = cefpdf::ExecuteSubProcess(app, mainArgs);
    if (exitCode >= 0)
        return exitCode;

    cefpdf::CommandLinePtr commandLine = cefpdf::CreateCommandLine(argc, argv);

    if (cefpdf::HasSwitch(commandLine, "help") || cefpdf::HasSwitch(commandLine, "h"))
    {
        printHelp(cefpdf::GetExecutableName(commandLine));
        return 0;
    }

    if (cefpdf::HasSwitch(commandLine, "list-sizes"))
    {
        printSizes();
        return 0;
    }

    cefpdf::Initialize(app, mainArgs, commandLine);
    try
    {
        if (cefpdf::HasSwitch(commandLine, "server"))
            return cefpdf::RunServer(app);
        else
            return cefpdf::RunJob(app);
    }
    catch (const std::string &error)
    {
        std::cerr << "ERROR: " << error << std::endl;
        return 1;
    }
}
