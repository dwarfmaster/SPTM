
#ifndef DEF_SPTM_FILTER
#define DEF_SPTM_FILTER

namespace sptm
{
    class Filter
    {
        public:
            Filter() = default;
            virtual ~Filter() = default;
            virtual bool operator()(Task*) = 0;
    };
}

#endif

