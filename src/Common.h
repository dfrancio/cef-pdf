#ifndef COMMON_H_
#define COMMON_H_

#include "include/internal/cef_types.h"

#include <string>
#include <vector>
#include <chrono>

namespace cefpdf {

std::string getCurrentWorkingDirectory();

std::string getTempDirectory();

namespace constants {
    // cef-pdf version number
    const std::string version = "0.2.1";
    // Internal scheme
    const std::string scheme = "cefpdf";
    // Default page size
    const std::string pageSize = "A4";
    // Default character encoding
    const std::string encoding = "utf-8";
    // Max number of concurrent processes (browsers)
    const int maxProcesses = 10;
    // Default host name
    const std::string serverHost = "127.0.0.1";
    // Default TCP server port
    const std::string serverPort = "9288";

    // Current working directory
    const std::string cwd = getCurrentWorkingDirectory();
    // Temp directory including trailing slash
    const std::string temp = getTempDirectory();
}

struct PageSize
{
    std::string name;
    int width, height;
};

typedef cef_pdf_print_margin_type_t PageMarginType;

struct PageMargin
{
    PageMarginType type;
    int top, right, bottom, left;
};

enum struct PageOrientation {
    PORTRAIT, LANDSCAPE
};

typedef std::vector<PageSize> PageSizesMap;

extern PageSizesMap pageSizesMap;

std::string strtolower(std::string s);

PageSize getPageSize(const CefString& str);

PageMargin getPageMargin(const CefString& str);

std::chrono::microseconds::rep microtime();

std::string pathToUri(const std::string&);

std::string reserveTempFile();

std::string loadTempFile(const std::string&, bool remove = true);

bool deleteTempFile(const std::string&);

} // namespace cefpdf

#endif // COMMON_H_
