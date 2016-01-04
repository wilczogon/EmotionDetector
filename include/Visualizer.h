#ifndef VISUALIZER_H
#define VISUALIZER_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>


class Visualizer
{
    public:
        Visualizer();
        virtual ~Visualizer();
        void visualize(std::string title, int width, int height, sf::Texture* texture);
        void visualizeClusters(std::string title, std::list<std::vector<float> > positions, std::list<sf::Color> colors, int width, int height/*labels?*/);
        void addPoint(sf::Vector2f position, sf::Color colors);
        void clearPoints();
    protected:
    private:
        std::list<sf::Vector2f> positions;
        std::list<sf::Color> colors;
};

#endif // VISUALIZER_H
