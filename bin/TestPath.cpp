#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Minions.h"
#include "XMLParser.h"
#include "Larry.h"
#include "TextureManager.h"
#include "TerrainManager.h"
#include "util.h"
#include "math.h"
#include "PathUtil.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	// Resource Managers
	TextureManager textures;
	XMLParser parser;
	TerrainManager terrain;

	// Fish
	sf::Vertex line[2];
	sf::Vertex line2[2];
	Minions minion(sf::Vector2f(700,400));
	Larry larry;

	// create main window
	sf::RenderWindow App(sf::VideoMode(1024,768,32), "Path - SFML",sf::Style::Titlebar|sf::Style::Close);

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
	
	// expanded terrain
	std::vector<std::vector<sf::Vector2f>> expand_test;

	// polygon shapes
	std::vector<sf::Vector2f> shape1;

	shape1.push_back(sf::Vector2f(500,250));
	shape1.push_back(sf::Vector2f(200,120));
	shape1.push_back(sf::Vector2f(150,350));
	test.push_back(shape1);
	expand_test.push_back(PathUtil::expandPolygon(shape1,minion.getHeadDistance()+40));


	std::vector<sf::Vector2f> shape2;
	shape2.push_back(sf::Vector2f(750,700));
	shape2.push_back(sf::Vector2f(650,500));
	shape2.push_back(sf::Vector2f(900,600));
	test.push_back(shape2);
	expand_test.push_back(PathUtil::expandPolygon(shape2,minion.getHeadDistance()+40));
	
	std::vector<sf::Vector2f> shape3;
	shape3.push_back(sf::Vector2f(850,100));
	shape3.push_back(sf::Vector2f(650,110));
	shape3.push_back(sf::Vector2f(630,200));
	shape3.push_back(sf::Vector2f(850,210));
	test.push_back(shape3);
	expand_test.push_back(PathUtil::expandPolygon(shape3,minion.getHeadDistance()+40));
	
	
	terrain.load(test);


	// test without shapes
	// expand_test.clear();

    // start main loop
	sf::Clock clock;
	float deltaTime;
	sf::Vector2f travelDirection, newPos, travelDistance, currPos, headVector;
	sf::Vector2f targetPoint, normalizedDirection;

	while(App.isOpen())
	{
    // process events
		sf::Event event;
		while (App.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
				App.close();
				break;
				default:
				break;
			}
		}
    	// clear screen and fill with blue
		App.clear(sf::Color::Blue);
		
		sf::Vector2i mousePos=sf::Mouse::getPosition(App);
		

		deltaTime = clock.restart().asSeconds();
		currPos = minion.getKnot(0).getPosition();


		targetPoint = PathUtil::getPath(larry.getKnot(0).getPosition(),currPos,expand_test, (minion.getSpeed()*deltaTime));
	
		travelDirection = targetPoint - currPos;
		
		normalizedDirection = normalize(travelDirection);

		travelDistance=(((minion.getSpeed()*deltaTime + minion.getHeadDistance())*normalizedDirection));

		newPos = currPos + travelDistance;

		if(isnan(newPos.x)) {
			cout << currPos.x << "," << currPos.y << endl;
			cout <<  travelDirection.x << "," << travelDirection.y << endl;
			cout << normalize(travelDirection).x << endl;
			// break;
		}

		//show path to take between fish
		line[0] = currPos;
		line[1] = targetPoint;

		larry.swimTo(sf::Vector2f(mousePos.x,mousePos.y));

		// minion.swimTo(sf::Vector2f(mousePos.x,mousePos.y));
		minion.swimTo(newPos);

		larry.update(0);
		minion.update(0);

		App.draw(terrain);
		App.draw(larry);
		App.draw(minion);
		App.draw(line, 2, sf::Lines);
		App.display();
	}

  // Done.
	return 0;

}

