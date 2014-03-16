
#ifndef DEF_SPTM_TASK
#define DEF_SPTM_TASK

#include <string>
#include <vector>

namespace sptm
{
    class Task
    {
        public:
            Task();
            ~Task();
            bool done() const;
            bool done(bool d);

            /* Contacts */
            std::string src() const;
            void src(const std::string& str);
            std::vector<std::string> dst() const;
            void dst(const std::vector<std::string>& ds);
            void addDst(const std::string& d);
            std::vector<std::string> exe() const;
            void exe(const std::vector<std::string>& es);
            void addExe(const std::string& e);

            /* Action line. */
            std::string action() const;
            void action(const std::string& a);

            /* Description. */
            std::string desc() const;
            void desc(const std::string& d);

            /* Tags. */
            std::vector<std::string> tags() const;
            void addTag(const std::string& str);
            void rmTag(const std::string& str);
            bool hasTag(const std::string& str) const;

            /* Tree. */
            std::vector<Task*> parents() const;
            std::vector<Task*> children() const;
            void addChild(Task* t);
            void rmChild(Task* t);

            /* Compute the id (SHA-512) */
            void id();
            std::string stored_id() const;

        private:
            bool m_done;
            std::string m_action;
            std::string m_desc;
            std::string m_src;
            std::vector<std::string> m_dst;
            std::vector<std::string> m_exe;
            std::vector<std::string> m_tags;
            std::vector<Task*> m_parents;
            std::vector<Task*> m_children;
            std::string m_id;
    };
}

#endif

