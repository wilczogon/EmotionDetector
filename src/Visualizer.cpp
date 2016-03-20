#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Visualizer.h"
#include <Translator.h>

Visualizer::Visualizer()
{
    clearPoints();
}

Visualizer::~Visualizer()
{
    //dtor
}

void Visualizer::visualize(std::string title, /*std::list<std::vector<float> > positions, std::list<sf::Color> colors,*/ int width, int height, sf::Texture* texture){
    sf::RenderWindow window(
        sf::VideoMode(width,height),
        title.c_str()
        );

    sf::Sprite      image;
    image.setTexture(*texture);
    image.setTextureRect( sf::IntRect(0,0,width,height) );

    sf::CircleShape point(1);

    window.setFramerateLimit( 60 );

    bool program_running = true;
    while(program_running)
    {
        sf::Event evt;
        while( window.pollEvent(evt) )
        {
            if(evt.type == sf::Event::Closed)
                program_running = false;
        }

        window.draw(image);

        std::list<sf::Vector2f>::iterator posIt = positions.begin();
        std::list<sf::Color>::iterator colIt = colors.begin();
        while(posIt != positions.end() && colIt != colors.end()){
            point.setPosition(*posIt);
            point.setFillColor(*colIt);
            window.draw(point);

            posIt++;
            colIt++;
        }

        window.display();
    }
}

void Visualizer::visualizeClusters(std::string title, std::list<std::vector<float> > positions, std::list<sf::Color> colors, int width, int height){

    int dim = (*positions.begin()).size();
    float boxWidth = width/(float)dim;
    float boxHeight = height/(float)dim;

    sf::RenderWindow window(
        sf::VideoMode(width,height),
        title.c_str()
        );

    sf::CircleShape point(1);
    sf::RectangleShape line(sf::Vector2f(width, 1));
    line.setFillColor(sf::Color::White);
    sf::RectangleShape line2(sf::Vector2f(1, height));
    line2.setFillColor(sf::Color::White);

    window.setFramerateLimit( 1 );

    bool program_running = true;
    while(program_running)
    {
        sf::Event evt;
        while( window.pollEvent(evt) )
        {
            if(evt.type == sf::Event::Closed)
                program_running = false;
        }

        window.clear(sf::Color::Black);

        //draw boxes
        for(int i = 1; i<dim; ++i){

            line.setPosition(sf::Vector2f(0, i*boxHeight));
            window.draw(line);

            line2.setPosition(sf::Vector2f(i*boxWidth, 0));
            window.draw(line2);
        }

        std::vector<float> max(dim, 0);
        std::vector<float> min(dim, 10000000);
        for(std::vector<float> &vec: positions){
            for(int i = 0; i<dim; ++i){
                if(vec[i]>max[i])
                    max[i] = vec[i];
                if(vec[i]<min[i])
                    min[i] = vec[i];
            }
        }

        int actualZeroX = 0;
        int actualZeroY = 0;
        for(int i = 0; i< dim; ++i){
            for(int j = 0; j< dim; ++j){
                std::list<std::vector<float> >::iterator posIt = positions.begin();
                std::list<sf::Color>::iterator colIt = colors.begin();
                while(posIt != positions.end() && colIt != colors.end()){
                    if(max[i]-min[i] != 0 && max[j]-min[j] != 0){
                        point.setPosition(actualZeroX + boxWidth*((*posIt)[i]-min[i])/(max[i]-min[i]), actualZeroY + boxHeight*((*posIt)[j]-min[j])/(max[j]-min[j]));
                        point.setFillColor(*colIt);
                        window.draw(point);
                    }
                    posIt++;
                    colIt++;
                }
                actualZeroX += boxWidth;
            }
            actualZeroY += boxHeight;
            actualZeroX = 0;
        }

        window.display();
    }
}

void Visualizer::visualize(std::string title, Space* space, sf::Color(*func)(void*), int width, int height){
    visualizeClusters(title, space->getVectors(), Translator::classesToColors(space->getClasses(), func), width, height);
}

void Visualizer::addPoint(sf::Vector2f position, sf::Color color){
    positions.push_back(position);
    colors.push_back(color);
}

void Visualizer::clearPoints(){
    positions = std::list<sf::Vector2f>();
    colors = std::list<sf::Color>();
}
