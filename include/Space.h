#ifndef SPACE_H
#define SPACE_H
#include <iostream>
#include <list>
#include <vector>


class Space
{
    public:
        Space(std::string fileName);
        Space(std::list<std::vector<float> > vecs, std::list<void*> classes);
        void add(std::vector<float> vec, void* attribute);
        void save(std::string fileName);
        void* knn(std::vector<float> vec, int k);
        virtual ~Space();
    protected:
    private:
};

#endif // SPACE_H
