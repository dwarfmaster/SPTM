
#include "task.hpp"
#include "sha2.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>

namespace sptm
{
    Task::Task()
        : m_done(false)
    {}

    Task::~Task()
    {}

    bool Task::done() const
    {
        return m_done;
    }

    bool Task::done(bool d)
    {
        return (m_done = d);
    }

    std::string Task::src() const
    {
        return m_src;
    }

    void Task::src(const std::string& str)
    {
        m_src = str;
    }

    std::vector<std::string> Task::dst() const
    {
        return m_dst;
    }

    void Task::dst(const std::vector<std::string>& ds)
    {
        m_dst = ds;
    }

    void Task::addDst(const std::string& d)
    {
        if(std::find(m_dst.begin(), m_dst.end(), d) == m_dst.end())
            m_dst.push_back(d);
    }

    std::vector<std::string> Task::exe() const
    {
        return m_exe;
    }

    void Task::exe(const std::vector<std::string>& es)
    {
        m_exe = es;
    }

    void Task::addExe(const std::string& e)
    {
        if(std::find(m_exe.begin(), m_exe.end(), e) == m_exe.end())
            m_exe.push_back(e);
    }

    std::string Task::action() const
    {
        return m_action;
    }

    void Task::action(const std::string& a)
    {
        m_action = a;
    }

    std::string Task::desc() const
    {
        return m_desc;
    }

    void Task::desc(const std::string& d)
    {
        m_desc = d;
    }

    std::vector<std::string> Task::tags() const
    {
        return m_tags;
    }

    void Task::addTag(const std::string& str)
    {
        if(std::find(m_tags.begin(), m_tags.end(), str) == m_tags.end())
            m_tags.push_back(str);
    }

    void Task::rmTag(const std::string& str)
    {
        std::vector<std::string>::iterator it = std::find(m_tags.begin(), m_tags.end(), str);
        if(it != m_tags.end())
            m_tags.erase(it);
    }

    bool Task::hasTag(const std::string& str) const
    {
        return std::find(m_tags.begin(), m_tags.end(), str) != m_tags.end();
    }

    std::vector<Task*> Task::parents() const
    {
        return m_parents;
    }

    std::vector<Task*> Task::children() const
    {
        return m_children;
    }

    void Task::addChild(Task* t)
    {
        if(std::find(m_children.begin(), m_children.end(), t) != m_children.end())
            return;
        t->m_parents.push_back(this);
        m_children.push_back(t);
    }

    void Task::rmChild(Task* t)
    {
        std::vector<Task*>::iterator it = std::find(m_children.begin(), m_children.end(), t);
        if(it == m_children.end())
            return;
        m_children.erase(it);
        it = std::find(t->m_parents.begin(), t->m_parents.end(), this);
        if(it != t->m_parents.end())
            t->m_parents.erase(it);
    }
            
    void Task::id()
    {
        std::string tohash(m_action);
        for(std::string dst : m_dst)
            tohash += dst;

        const char* data = tohash.c_str();
        char output[SHA256_DIGEST_STRING_LENGTH];
        char* result = SHA256_Data((const uint8_t*)data, strlen(data), output);
        m_id = result;
    }

    std::string Task::stored_id() const
    {
        return m_id;
    }

}

