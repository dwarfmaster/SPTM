
#include "tasksystem.hpp"
#include "filters.hpp"
#include <algorithm>
#include <fstream>

namespace sptm
{
    TaskSystem::TaskSystem()
    {}

    TaskSystem::~TaskSystem()
    {
        for(Task* tk : m_tasks)
            delete tk;
    }

    void TaskSystem::config(Config cfg, const std::string& value)
    {
        m_config[(int)cfg] = value;
    }

    std::string TaskSystem::config(Config cfg) const
    {
        return m_config[(int)cfg];
    }

    bool TaskSystem::load()
    {
        /* TODO */
    }

    bool TaskSystem::save()
    {
        /* The save function is quite rudimentary, and will
         * overwrite the previously saved configuration.
         */
        std::string path = m_config[(int)DONEPATH];
        if(path.empty())
            return false;
        DoneFilter filter(true);
        clearFilters();
        addFilter(&filter);
        if(!saveResults(path))
            return false;

        path = m_config[(int)UNDONEPATH];
        if(path.empty())
            return false;
        filter.set(false);
        clearFilters();
        addFilter(&filter);
        return saveResults(path);
    }
            
    bool TaskSystem::saveResults(const std::string& path)
    {
        std::ofstream ofs(path, std::ios::trunc);
        if(!ofs)
            return false;

        /* First loop to make sure the id are up to date. */
        for(Task *tk : m_tasks)
            tk->id();

        for(Task *tk : m_tasks) {
            /* First line : declaring the task. */
            ofs << tk->stored_id() << " TO ";
            for(std::string dst : tk->dst())
                ofs << dst << " ";
            ofs << "TASK " << tk->action() << std::endl;

            /* Storing the src and exes. */
            ofs << "EXE ";
            for(std::string exe : tk->exe())
                ofs << exe << " ";
            ofs << "\nSRC " << tk->src() << std::endl;

            /* Storing the parents and children. */
            ofs << "PARENTS ";
            for(Task* par : tk->parents())
                ofs << par->stored_id() << " ";
            ofs << "\nCHILDREN ";
            for(Task* child : tk->children())
                ofs << child->stored_id() << " ";
            ofs << std::endl;

            /* Storing the tasks. */
            ofs << "TAGS ";
            for(std::string tag : tk->tags())
                ofs << tag << " ";
            ofs << "\n" << std::endl;
        }

        return true;
    }

    void TaskSystem::clearFilters()
    {
        m_results = m_tasks;
    }

    void TaskSystem::addFilter(Filter* f)
    {
        std::vector<Task*> temp = m_results;
        m_results.clear();

        for(Task* tk : temp) {
            if( (*f)(tk) )
                m_results.push_back(tk);
        }
    }

    std::vector<Task*> TaskSystem::applyFilters()
    {
        return m_results;
    }

    Task* TaskSystem::newTask(const std::string& action, const std::vector<std::string>& dst)
    {
        Task* tk = new Task;
        tk->action(action);
        tk->dst(dst);
        tk->id();

        if(!m_config[(int)DEFEXE].empty())
            tk->addExe(m_config[(int)DEFEXE]);
        m_tasks.push_back(tk);
        return tk;
    }

    void TaskSystem::rmTask(Task** t)
    {
        auto it = std::find(m_tasks.begin(), m_tasks.end(), *t);
        if(it == m_tasks.end())
            return;

        for(Task* parent : (*it)->parents())
            parent->rmChild(*t);
        for(Task* child : (*it)->children())
            (*t)->rmChild(child);

        delete *t;
        *t = NULL;
    }

}

