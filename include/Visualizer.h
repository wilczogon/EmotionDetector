#ifndef VISUALIZER_H
#define VISUALIZER_H
#include <iostream>
//#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <Space.h>
#include <opencv2/core.hpp>

class Visualizer
{
    public:
        Visualizer();
        virtual ~Visualizer();
        //void visualize(std::string title, int width, int height, sf::Texture* texture);
        //void visualizeClusters(std::string title, cv::Mat positions, std::list<sf::Color> colors, int width, int height/*labels?*/);
        //void visualizeClusters(std::string title, std::list<std::vector<float> > positions, std::list<sf::Color> colors, int width, int height/*labels?*/);
        //void visualize(std::string title, Space* space, sf::Color(*func)(void*), int width, int height);
        //void addPoint(sf::Vector2f position, sf::Color colors);
        void clearPoints();
    protected:
    private:
        //std::list<sf::Vector2f> positions;
        //std::list<sf::Color> colors;
};

#endif // VISUALIZER_H
