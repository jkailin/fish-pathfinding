#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Minions.h"
#include "XMLParser.h"
#include "Larry.h"
#include "TextureManager.h"
#include "TerrainManager.h"
#include "util.h"
#include "math.h"

#include <iostream>

using namespace std;

// check if line intersects polygon. If polygon is far away, doesn't count.
int polygon_intersect_line(sf::Vector2f target_point, sf::Vector2f source_point, std::vector<std::vector<sf::Vector2f>> polygons)
{

    int angleRotation;



}

int main(int argc, char **argv)
    {
        // Resource Managers
        TextureManager textures;
        XMLParser parser;
        TerrainManager terrain;

        // Fish
        sf::Vertex line[2];
        sf::Vertex line2[2];
        Minions minion(sf::Vector2f(700, 200));
        Larry larry;

        // create main window
        sf::RenderWindow App(sf::VideoMode(1024, 768, 32), "Path - SFML", sf::Style::Titlebar | sf::Style::Close);

        // add textures
        parser.loadXML("../data/xml/levels.xml");
        parser.loadTexture(textures);

        minion.setTexture(textures.get().texture);
        minion.setTextureAreas(textures.get().areas);
        larry.setTexture(textures.get().texture);
        larry.setTextureAreas(textures.get().areas);

        minion.init();
        larry.init();

        // create terrain with verticies
        std::vector<std::vector<sf::Vector2f>> test;
        std::vector<std::vector<sf::Vector2f>> expand_test;
        std::vector<sf::Vector2f> shape1;
        shape1.push_back(sf::Vector2f(10, 10));
        shape1.push_back(sf::Vector2f(200, 30));
        shape1.push_back(sf::Vector2f(50, 150));
        test.push_back(shape1);
        expand_test.push_back(expand_polygon(shape1, minion.getHeadDistance() + 40));

        std::vector<sf::Vector2f> shape2;
        shape2.push_back(sf::Vector2f(500, 600));
        shape2.push_back(sf::Vector2f(400, 300));
        shape2.push_back(sf::Vector2f(600, 400));
        test.push_back(shape2);
        expand_test.push_back(expand_polygon(shape2, minion.getHeadDistance() + 40));

        std::vector<sf::Vector2f> shape3;
        shape3.push_back(sf::Vector2f(700, 60));
        shape3.push_back(sf::Vector2f(770, 60));
        shape3.push_back(sf::Vector2f(850, 100));
        shape3.push_back(sf::Vector2f(650, 100));
        test.push_back(shape3);
        expand_test.push_back(expand_polygon(shape3, minion.getHeadDistance() + 40));
        terrain.load(test);

        // test without shapes
        // expand_test.clear();

        // start main loop
        sf::Clock clock;
        float deltaTime;
        // sf::Vector2i currPos;
        sf::Vector2f travelDirection, newPos, travelDistance, currPos, headVector;
        sf::Vector2f targetPoint;
        headVector = sf::Vector2f(7.071, 7.071);

        while (App.isOpen())
        {
            // process events
            sf::Event event;
            while (App.pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                    App.close();
                    break;
                default:
                    break;
                }
            }
            // clear screen and fill with blue
            App.clear(sf::Color::Blue);

            sf::Vector2i mousePos = sf::Mouse::getPosition(App);

            deltaTime = clock.restart().asSeconds();
            currPos = minion.getKnot(0).getPosition();

            // edit this
            targetPoint = getPath(larry.getKnot(0).getPosition(), currPos, expand_test);

            travelDirection = targetPoint - currPos;

            travelDistance = (((minion.getSpeed() * deltaTime + 10.0f) * normalize(travelDirection)));
            newPos = currPos + travelDistance * 1.0f;

            // newPos = newPos;

            if (isnan(newPos.x))
            {
                break;
            }

            cout << "pos x: " << currPos.x << endl;
            cout << "pos y: " << currPos.y << endl;
            // cout<< "direction x: " << travelDirection.x<<endl;
            // cout<< "direction y: " << travelDirection.y<<endl;
            // cout<< "target x: " << travelDirection.x<<endl;
            // cout<< "target y: " << travelDirection.y<<endl;
            cout << "distance x: " << travelDistance.x << endl;
            cout << "distance y: " << travelDistance.y << endl;
            // cout << "new pos x: " << newPos.x<<endl;
            // cout << "new pos y: " << newPos.y<<endl;
            // cout<< "delta time: " << deltaTime<<endl;
            // cout<< "speed: " << minion.getSpeed()<<endl;

            //show path to take between fish
            line[0] = currPos;
            line[1] = targetPoint;
            line2[0] = sf::Vector2f(0, 0);
            line2[1] = travelDirection;

            larry.swimTo(sf::Vector2f(mousePos.x, mousePos.y));

            // minion.swimTo(sf::Vector2f(mousePos.x,mousePos.y));
            minion.swimTo(newPos);

            // if(!isnan(currPos.x) || !isnan(currPos.y) ) {
            // 	minion.swimTo(minion.getKnot(0).getPosition());
            // } else {
            // 	cout << "nan";
            // }
            larry.update(0);
            minion.update(0);

            App.draw(terrain);
            App.draw(larry);
            App.draw(minion);
            App.draw(line, 2, sf::Lines);
            App.draw(line2, 2, sf::Lines);
            App.display();
        }

        // Done.
        return 0;
    }
