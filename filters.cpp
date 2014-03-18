
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
}

