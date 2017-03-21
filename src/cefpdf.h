#ifndef CEFPDF_H_
#define CEFPDF_H_

#include <memory>
#include <string>

#include "export.h"

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
CEFPDF_EXPORT const std::string &GetLibVersion();
CEFPDF_EXPORT const std::string &GetDefaultPageSizeName();
CEFPDF_EXPORT const std::string &GetDefaultServerHost();
CEFPDF_EXPORT const std::string &GetDefaultServerPort();

//-----------------------------------------------------------------------------
// Opaque pointer creation
//-----------------------------------------------------------------------------
CEFPDF_EXPORT AppPtr CreateApp();
CEFPDF_EXPORT MainArgsPtr CreateMainArgs(int argc, char **argv);
CEFPDF_EXPORT CommandLinePtr CreateCommandLine(int argc, char **argv);
CEFPDF_EXPORT PageSizeIteratorPtr CreatePageSizeIterator();

//-----------------------------------------------------------------------------
// Utility functions
//-----------------------------------------------------------------------------
CEFPDF_EXPORT std::string GetExecutableName(CommandLinePtr commandLine);
CEFPDF_EXPORT bool GetNextPageSize(
    PageSizeIteratorPtr iterator, std::string &name, int &width, int &height);
CEFPDF_EXPORT bool HasSwitch(CommandLinePtr commandLine, const std::string &name);

//-----------------------------------------------------------------------------
// Main application procedures
//-----------------------------------------------------------------------------
CEFPDF_EXPORT int ExecuteSubProcess(AppPtr app, MainArgsPtr mainArgs);
CEFPDF_EXPORT void Initialize(AppPtr app, MainArgsPtr mainArgs, CommandLinePtr commandLine);
CEFPDF_EXPORT int RunJob(AppPtr app);
CEFPDF_EXPORT int RunServer(AppPtr app);

} // namespace cefpdf

#endif // CEFPDF_H_
