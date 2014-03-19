
#include "tasksystem.hpp"
#include "filters.hpp"
#include <iostream>
#include <cstring>

void tag(int argc, char *argv[], sptm::TaskSystem* ts)
{
    if(argc != 4) {
        std::cerr << "Invalid use of tag command." << std::endl;
        return;
    }

    ts->clearFilters();
    sptm::TagFilter filter(argv[3], true);
    ts->addFilter(&filter);
    std::vector<sptm::Task*> tasks = ts->applyFilters();

    for(sptm::Task* tk : tasks)
        tk->addTag(argv[2]);
}

void search(int argc, char *argv[], sptm::TaskSystem* ts)
{
    if(argc <= 2) {
        std::cerr << "Missing arguments to search command." << std::endl;
        return;
    }

    ts->clearFilters();
    for(int i = 2; i < argc; ++i) {
        sptm::TagFilter filter(argv[i], true);
        if(argv[i][0] == '-')
            filter.set(argv[i] + 1, false);
        ts->addFilter(&filter);
    }
    std::vector<sptm::Task*> tasks = ts->applyFilters();

    for(sptm::Task* tk : tasks)
        std::cout << "Task " << tk->stored_id() << " : \"" << tk->action() << "\"." << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc <= 1) {
        std::cerr << "Argument required." << std::endl;
        return 1;
    }

    sptm::TaskSystem ts;
    ts.config(sptm::TaskSystem::DEFSRC, "Lucas8");
    ts.config(sptm::TaskSystem::DEFDST, "Lucas8");
    ts.config(sptm::TaskSystem::DEFEXE, "Lucas8");

    std::string home = std::getenv("HOME");
    ts.config(sptm::TaskSystem::DONEPATH,   home + "/.sptm/done");
    ts.config(sptm::TaskSystem::UNDONEPATH, home + "/.sptm/undone");

    if(!ts.load()) {
        std::cerr << "Couldn't load SPTM files." << std::endl;
        return 1;
    }

    if(strcmp(argv[1], "tag") == 0)
        tag(argc, argv, &ts);
    else if(strcmp(argv[1], "search") == 0)
        tag(argc, argv, &ts);
    else {
        std::cerr << "Invalid command." << std::endl;
        return 1;
    }

    return 0;
}

