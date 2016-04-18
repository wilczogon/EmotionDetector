#ifndef FACESIMAGESDATABASE_H
#define FACESIMAGESDATABASE_H

#include <list>
#include <vector>
#include <Emotion.h>
#include <map>
#include <set>


class FacesImagesDatabase
{
    public:
        FacesImagesDatabase();
        void add(std::string personId, Emotion emotion, std::list<std::vector<float> > vec);
        std::list<std::list<std::vector<float> > > get(std::string personId, Emotion emotion);
        std::set<Emotion> getEmotions();
        std::set<std::string> getPersonIds();
        virtual ~FacesImagesDatabase();
    protected:
    private:
        std::map<std::string, std::map<Emotion, std::list<std::list<std::vector<float> > > > > data;
        std::set<Emotion> emotions;
        std::set<std::string> personIds;
};

#endif // FACESIMAGESDATABASE_H
