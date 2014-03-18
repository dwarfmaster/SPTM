
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
}

#endif

