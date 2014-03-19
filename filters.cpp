
#include "filters.hpp"

namespace sptm
{
    DoneFilter::DoneFilter(bool done)
        : m_done(done)
    {}

    void DoneFilter::set(bool done)
    {
        m_done = done;
    }

    bool DoneFilter::operator()(Task* tk)
    {
        return tk->done() == m_done;
    }

    TagFilter::TagFilter(std::string tag, bool has)
        : m_tag(tag), m_has(has)
    {}

    void TagFilter::set(std::string tag, bool has)
    {
        m_tag = tag;
        m_has = has;
    }

    bool TagFilter::operator()(Task* tk)
    {
        return tk->hasTag(m_tag) == m_has;
    }

}

