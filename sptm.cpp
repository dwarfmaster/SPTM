
#include "tasksystem.hpp"
#include "filters.hpp"
#include <iostream>
#include <cstring>
#include <sstream>

void tag(int argc, char *argv[], sptm::TaskSystem* ts);
void search(int argc, char *argv[], sptm::TaskSystem* ts);
std::vector<sptm::Task*> find(int argc, char *argv[], sptm::TaskSystem* ts);
bool positive(std::string& arg);

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
        search(argc, argv, &ts);
    else {
        std::cerr << "Invalid command." << std::endl;
        return 1;
    }

    if(!ts.save()) {
        std::cerr << "Couldn't save changes." << std::endl;
        return 1;
    }

    return 0;
}

void tag(int argc, char *argv[], sptm::TaskSystem* ts)
{
    if(argc != 4) {
        std::cerr << "Invalid use of tag command." << std::endl;
        return;
    }

    ts->clearFilters();
    sptm::TagFilter filter;
    filter.set(argv[3], true);
    ts->addFilter(&filter);
    std::vector<sptm::Task*> tasks = ts->applyFilters();

    for(sptm::Task* tk : tasks)
        tk->addTag(argv[2]);
}

void search(int argc, char *argv[], sptm::TaskSystem* ts)
{
    if(argc <= 2) {
        std::cerr << "Syntax : " << argv[0] << " search tag:value tag:value ..." << std::endl;
        return;
    }

    std::vector<sptm::Task*> tasks = find(argc - 2, argv + 2, ts);
    for(sptm::Task* tk : tasks)
        std::cout << "Task " << tk->stored_id() << " : \"" << tk->action() << "\"." << std::endl;
}

std::vector<sptm::Task*> find(int argc, char *argv[], sptm::TaskSystem* ts)
{
    ts->clearFilters();
    for(int i = 0; i < argc; ++i) {
        std::istringstream iss(argv[i]);
        std::string id;    std::getline(iss, id, ':');
        std::string value; std::getline(iss, value);
        if(id.empty() || value.empty()) {
            std::cout << "invalid description option : \"" << argv[i] << "\"." << std::endl;
            continue;
        }

        sptm::Filter* filter = NULL;
        if(id == "tag") {
            bool pos = positive(value);
            sptm::TagFilter* f = new sptm::TagFilter;
            f->set(value, pos);
            filter = f;
        }

        else if(id == "state") {
            bool pos = positive(value);
            filter = new sptm::DoneFilter(pos);
        }

        else if(id == "id") {
            bool pos = positive(value);
            sptm::IDFilter* f = new sptm::IDFilter;
            f->set(value, pos);
            filter = f;
        }

        else if(id == "dst") {
            bool pos = positive(value);
            sptm::DstFilter* f = new sptm::DstFilter;
            f->set(value, pos);
            filter = f;
        }

        else if(id == "src") {
            bool pos = positive(value);
            sptm::SrcFilter* f = new sptm::SrcFilter;
            f->set(value, pos);
            filter = f;
        }

        else if(id == "exe") {
            bool pos = positive(value);
            sptm::ExeFilter* f = new sptm::ExeFilter;
            f->set(value, pos);
            filter = f;
        }

        else {
            std::cout << "invalid description option : \"" << argv[i] << "\"." << std::endl;
            continue;
        }

        if(filter)
            ts->addFilter(filter);
    }

    return ts->applyFilters();
}

bool positive(std::string& arg)
{
    if(arg.empty())
        return true;

    bool pos = true;
    if(arg[0] == '-') {
        pos = false;
        arg = arg.substr(1);
    }
    else if(arg[0] == '+')
        arg = arg.substr(1);
    return pos;
}

