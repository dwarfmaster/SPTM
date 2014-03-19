
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
            TagFilter(std::string tag, bool has = true);
            void set(std::string tag, bool has = true);
            virtual ~TagFilter() = default;
            virtual bool operator()(Task* tk);
        private:
            std::string m_tag;
            bool m_has;
    };
}

#endif

