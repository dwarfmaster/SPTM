
#ifndef DEF_SPTM_FILTERS
#define DEF_SPTM_FILTERS

#include "filter.hpp"

namespace sptm
{
    class DoneFilter : public Filter
    {
        public:
            DoneFilter(bool done = true);
            void set(bool done);
            virtual ~DoneFilter() = default;
            virtual bool operator()(Task* tk);

        private:
            bool m_done;
    };

    class TagFilter : public Filter
    {
        public:
            TagFilter();
            void set(const std::string& tag, bool has = true);
            virtual ~TagFilter() = default;
            virtual bool operator()(Task* tk);
        private:
            std::string m_tag;
            bool m_has;
    };

    class ParentFilter : public Filter
    {
        public:
            ParentFilter();
            void set(Task* par, bool has = true);
            virtual ~ParentFilter() = default;
            virtual bool operator()(Task* tk);
        private:
            Task* m_par;
            bool m_has;
    };

    class ChildFilter : public Filter
    {
        public:
            ChildFilter();
            void set(Task* child, bool has = true);
            virtual ~ChildFilter() = default;
            virtual bool operator()(Task* tk);
        private:
            Task* m_child;
            bool m_has;
    };

    class IDFilter : public Filter
    {
        public:
            IDFilter();
            void set(const std::string& id, bool valid = true);
            virtual ~IDFilter() = default;
            virtual bool operator()(Task* tk);
        private:
            std::string m_id;
            bool m_valid;
    };

    class SrcFilter : public Filter
    {
        public:
            SrcFilter();
            virtual ~SrcFilter() = default;
            void set(const std::string& src, bool has = true);
            virtual bool operator()(Task* tk);
        private:
            std::string m_src;
            bool m_has;
    };

    class DstFilter : public Filter
    {
        public:
            DstFilter();
            virtual ~DstFilter() = default;
            void set(const std::string& dst, bool has = true);
            virtual bool operator()(Task* tk);
        private:
            std::string m_dst;
            bool m_has;
    };

    class ExeFilter : public Filter
    {
        public:
            ExeFilter();
            virtual ~ExeFilter() = default;
            void set(const std::string& exe, bool has = true);
            virtual bool operator()(Task* tk);
        private:
            std::string m_exe;
            bool m_has;
    };
}

#endif

