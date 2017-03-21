#ifndef JOB_CONTENT_PROVIDER_H_
#define JOB_CONTENT_PROVIDER_H_

#include "Visitor.h"
#include "Local.h"
#include "Remote.h"
#include "StdInput.h"
#include "StdInputStreamReader.h"

namespace cefpdf {
namespace job {

class ContentProvider : public Visitor
{

public:
    ContentProvider() {}

    CefRefPtr<CefStreamReader> GetStreamReader() const {
        return m_reader;
    }

    void visit(CefRefPtr<Local> job) override {
        m_reader = CefStreamReader::CreateForData(
            static_cast<void*>(const_cast<char*>(job->GetContent().c_str())),
            job->GetContent().size()
        );
    }

    void visit(CefRefPtr<Remote> job) override {
        // no implementation
    }

    void visit(CefRefPtr<StdInput> job) override {
        m_reader = CefStreamReader::CreateForHandler(new StdInputStreamReader);
    }

private:
    CefRefPtr<CefStreamReader> m_reader;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(ContentProvider)
};

} // namespace job
} // namespace cefpdf

#endif // JOB_CONTENT_PROVIDER_H_
