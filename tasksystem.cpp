
#include "tasksystem.hpp"
#include "filters.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>

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
        std::string path = m_config[(int)DONEPATH];
        if(path.empty())
            return false;
        if(!loadSome(path, true))
            return false;

        path = m_config[(int)UNDONEPATH];
        if(path.empty())
            return false;
        return loadSome(path, false);
    }
            
    bool TaskSystem::loadSome(const std::string& path, bool done)
    {
        std::ifstream ifs(path);
        if(!ifs)
            return false;
        std::string line;
        Task* act = NULL;
        std::map<std::string, Task*> ids;
        std::map<Task*, std::vector<std::string>> children;
        std::vector<std::string> act_children;

        while(std::getline(ifs, line)) {
            std::string word;
            std::istringstream iss(line);
            std::getline(iss, word, ' ');

            if(word == "TASK") {
                if(act) {
                    m_tasks.push_back(act);
                    children[act] = act_children;
                }
                act = NULL;
                bool action = false;
                std::vector<std::string> dsts;
                std::string action_line;
                std::getline(iss, word, ' ');
                std::string id = word;
                while(std::getline(iss, word, ' ')) {
                    if(word == "TO")
                        action = false;
                    else if(word == "ACTION")
                        action = true;
                    else {
                        if(action) {
                            if(!action_line.empty())
                                action_line += ' ';
                            action_line += word;
                        }
                        else
                            dsts.push_back(word);
                    }
                }

                if(action_line.empty() || dsts.empty())
                    continue;
                act_children.clear();
                act = new Task;
                act->action(action_line);
                act->dst(dsts);
                act->id();
                act->done(done);
                ids[id] = act;
            }

            else if(word == "CHILDREN") {
                std::string word;
                while(std::getline(iss, word, ' '))
                    act_children.push_back(word);
            }

            else if(word == "TAGS") {
                if(!act)
                    continue;
                std::string word;
                while(std::getline(iss, word, ' '))
                    act->addTag(word);
            }

            else if(word == "FROM") {
                if(!act)
                    continue;
                std::string word;
                while(std::getline(iss, word, ' '))
                    act->src(word);
            }

            else if(word == "EXE") {
                if(!act)
                    continue;
                std::string word;
                while(std::getline(iss, word, ' '))
                    act->addExe(word);
            }
        }
        if(act) {
            m_tasks.push_back(act);
            children[act] = act_children;
        }

        /* Heredity. */
        for(Task* tk : m_tasks) {
            for(std::string child_id : children[tk])
                tk->addChild(ids[child_id]);
        }

        if(m_tasks.empty())
            return false;
        return true;
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
        for(Task *tk : m_results)
            tk->id();

        for(Task *tk : m_results) {
            /* First line : declaring the task. */
            ofs << "TASK " << tk->stored_id() << " TO ";
            for(std::string dst : tk->dst())
                ofs << dst << " ";
            ofs << "ACTION " << tk->action() << std::endl;

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

