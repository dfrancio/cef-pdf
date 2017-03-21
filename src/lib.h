#ifndef LIB_H_
#define LIB_H_

#include <memory>
#include <string>

namespace cefpdf
{
//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
struct App;
struct MainArgs;
struct CommandLine;
struct PageSizeIterator;

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef std::shared_ptr<App> AppPtr;
typedef std::shared_ptr<MainArgs> MainArgsPtr;
typedef std::shared_ptr<CommandLine> CommandLinePtr;
typedef std::shared_ptr<PageSizeIterator> PageSizeIteratorPtr;

//-----------------------------------------------------------------------------
// Static constant accessors
//-----------------------------------------------------------------------------
const std::string &GetLibVersion();
const std::string &GetDefaultPageSizeName();
const std::string &GetDefaultServerHost();
const std::string &GetDefaultServerPort();

//-----------------------------------------------------------------------------
// Opaque pointer creation
//-----------------------------------------------------------------------------
AppPtr CreateApp();
MainArgsPtr CreateMainArgs(int argc, char **argv);
CommandLinePtr CreateCommandLine(int argc, char **argv);
PageSizeIteratorPtr CreatePageSizeIterator();

//-----------------------------------------------------------------------------
// Utility functions
//-----------------------------------------------------------------------------
std::string GetExecutableName(CommandLinePtr commandLine);
bool GetNextPageSize(PageSizeIteratorPtr iterator, std::string &name, int &width, int &height);
bool HasSwitch(CommandLinePtr commandLine, const std::string &name);

//-----------------------------------------------------------------------------
// Main application procedures
//-----------------------------------------------------------------------------
int ExecuteSubProcess(AppPtr app, MainArgsPtr mainArgs);
void Initialize(AppPtr app, MainArgsPtr mainArgs, CommandLinePtr commandLine);
int RunJob(AppPtr app);
int RunServer(AppPtr app);

} // namespace cefpdf

#endif // LIB_H_
