
#ifndef DEF_SPTM_TASKSYSTEM
#define DEF_SPTM_TASKSYSTEM

#include "task.hpp"
#include "filter.hpp"

namespace sptm
{
    class TaskSystem
    {
        public:
            TaskSystem();
            ~TaskSystem();

            enum Config {
                DEFSRC,
                DEFDST,
                DEFEXE,
                DONEPATH,
                UNDONEPATH,
                LAST
            };
            void config(Config cfg, const std::string& value);
            std::string config(Config cfg) const;

            bool load();
            bool save();

            void clearFilters();
            void addFilter(Filter* f); /* Filters won't be free'd. */
            std::vector<Task*> applyFilters();

            Task* newTask(const std::string& action, const std::vector<std::string>& dst);
            void rmTask(Task** t);

        private:
            std::string m_config[(int)LAST];
            std::vector<Task*> m_results;
            std::vector<Task*> m_tasks;

            bool saveResults(const std::string& path);
    };
}

#endif

