#ifndef FACESDIFFERENCESDATABASE_H
#define FACESDIFFERENCESDATABASE_H

#include <Emotion.h>
#include <list>
#include <map>
#include <vector>
#include <set>

class FacesDifferencesDatabase
{
    public:
        FacesDifferencesDatabase(Emotion basicEmotion);
        void add(Emotion emotion, std::vector<float> vec);
        std::list<std::vector<float> > get(Emotion emotion);
        std::set<Emotion> getEmotions();
        Emotion getBasicEmotion();
        virtual ~FacesDifferencesDatabase();
    protected:
    private:
        Emotion basicEmotion;
        std::set<Emotion> emotions;
        std::map<Emotion, std::list<std::vector<float> > > data;
};

#endif // FACESDIFFERENCESDATABASE_H
