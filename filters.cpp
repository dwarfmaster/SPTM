
#include "filters.hpp"
#include <algorithm>

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

    TagFilter::TagFilter()
    {}

    void TagFilter::set(const std::string& tag, bool has)
    {
        m_tag = tag;
        m_has = has;
    }

    bool TagFilter::operator()(Task* tk)
    {
        return tk->hasTag(m_tag) == m_has;
    }

    ParentFilter::ParentFilter()
    {}

    void ParentFilter::set(Task* par, bool has)
    {
        m_par = par;
        m_has = has;
    }

    bool ParentFilter::operator()(Task* tk)
    {
        std::vector<Task*> pars = tk->parents();
        bool result = std::find(pars.begin(), pars.end(), m_par) != pars.end();
        return result == m_has;
    }

    ChildFilter::ChildFilter()
    {}

    void ChildFilter::set(Task* child, bool has)
    {
        m_child = child;
        m_has = has;
    }

    bool ChildFilter::operator()(Task* tk)
    {
        std::vector<Task*> chs = tk->children();
        bool result = std::find(chs.begin(), chs.end(), m_child) != chs.end();
        return result == m_has;
    }

    IDFilter::IDFilter()
    {}

    void IDFilter::set(const std::string& id, bool valid)
    {
        m_id = id;
        m_valid = valid;
    }

    bool IDFilter::operator()(Task* tk)
    {
        std::string id = tk->stored_id();
        if(id.size() < m_id.size()) /* Shouldn't happen */
            return false;

        for(size_t i = 0; i < m_id.size(); ++i) {
            if(id[i] != m_id[i])
                return !m_valid;
        }
        return m_valid;
    }

    SrcFilter::SrcFilter()
    {}

    void SrcFilter::set(const std::string& src, bool has)
    {
        m_src = src;
        m_has = has;
    }

    bool SrcFilter::operator()(Task* tk)
    {
        if(tk->src() == m_src)
            return m_has;
        else
            return !m_has;
    }

    DstFilter::DstFilter()
    {}

    void DstFilter::set(const std::string& dst, bool has)
    {
        m_dst = dst;
        m_has = has;
    }

    bool DstFilter::operator()(Task* tk)
    {
        std::vector<std::string> dsts = tk->dst();
        bool result = std::find(dsts.begin(), dsts.end(), m_dst) != dsts.end();
        return result == m_has;
    }

    ExeFilter::ExeFilter()
    {}

    void ExeFilter::set(const std::string& exe, bool has)
    {
        m_exe = exe;
        m_has = has;
    }

    bool ExeFilter::operator()(Task* tk)
    {
        std::vector<std::string> exes = tk->exe();
        bool result = std::find(exes.begin(), exes.end(), m_exe) != exes.end();
        return result == m_has;
    }

}

