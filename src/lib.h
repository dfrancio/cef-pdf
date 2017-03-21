#ifndef LIB_H_
#define LIB_H_

#include <string>
#include <memory>

namespace cefpdf {

struct App;
struct MainArgs;
struct CommandLine;
struct PageSizeIterator;

typedef std::shared_ptr<App> AppPtr;
typedef std::shared_ptr<MainArgs> MainArgsPtr;
typedef std::shared_ptr<CommandLine> CommandLinePtr;
typedef std::shared_ptr<PageSizeIterator> PageSizeIteratorPtr;

const std::string &GetLibVersion();

const std::string &GetDefaultPageSizeName();

const std::string &GetDefaultServerHost();

const std::string &GetDefaultServerPort();

PageSizeIteratorPtr CreatePageSizeIterator();

bool GetNextPageSize(PageSizeIteratorPtr iterator, std::string &name, int &width, int &height);

AppPtr CreateApp();

MainArgsPtr CreateMainArgs(int argc, char **argv);

CommandLinePtr CreateCommandLine(int argc, char **argv);

std::string GetExecutableName(CommandLinePtr commandLine);

bool HasSwitch(CommandLinePtr commandLine, const std::string &name);

int ExecuteSubProcess(AppPtr app, MainArgsPtr mainArgs);

void Initialize(AppPtr app, MainArgsPtr mainArgs, CommandLinePtr commandLine);

void EnableJavaScript(AppPtr app);

void DisableJavaScript(AppPtr app);

int RunJob(AppPtr app);

int RunServer(AppPtr app);

} // namespace cefpdf

#endif // LIB_H_
