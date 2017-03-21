#ifndef JOB_LOADER_H_
#define JOB_LOADER_H_

#include "../Common.h"
#include "Visitor.h"
#include "Local.h"
#include "Remote.h"
#include "StdInput.h"

#include "include/cef_frame.h"

namespace cefpdf {
namespace job {

class Loader : public Visitor
{

public:
    Loader(CefRefPtr<CefFrame> frame) :
        m_frame(frame) {}

    void visit(CefRefPtr<Local> job) override {
        m_frame->LoadURL(cefpdf::constants::scheme + "://local");
    }

    void visit(CefRefPtr<Remote> job) override {
        m_frame->LoadURL(job->GetUrl());
    }

    void visit(CefRefPtr<StdInput> job) override {
        m_frame->LoadURL(cefpdf::constants::scheme + "://stdin");
    }

private:
    CefRefPtr<CefFrame> m_frame;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(Loader)
};

} // namespace job
} // namespace cefpdf

#endif // JOB_LOADER_H_
