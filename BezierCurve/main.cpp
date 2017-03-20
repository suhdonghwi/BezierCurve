#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "hsvrgb.hpp"

void drawBazierCurve(sf::VertexArray points, float t, sf::Color color, std::vector<sf::CircleShape>& footprints, sf::RenderWindow& window)
{
	if (t > 1.0f) t = 1.0f;

	for (std::size_t i = 0; i < points.getVertexCount(); i++)
	{
		points[i].color = color;
	}
	points.setPrimitiveType(sf::LinesStrip);
	window.draw(points);

	sf::VertexArray positions;
	std::vector<sf::CircleShape> circles_points;
	for (std::size_t i = 0; i < points.getVertexCount() - 1; i++)
	{
		sf::Vector2f current_pos = points[i].position, next_pos = points[i+1].position;

		sf::Vector2f position = { (1 - t) * current_pos.x + t * next_pos.x, (1 - t) * current_pos.y + t * next_pos.y };
		sf::CircleShape circle_point;
		circle_point.setRadius(5.0f);
		circle_point.setPosition({ position.x - 5.0f, position.y - 5.0f });
		circle_point.setFillColor(color);

		if (points.getVertexCount() == 2)
		{
			sf::CircleShape footprint;
			footprint.setRadius(2.5f);
			footprint.setPosition({ position.x - 2.5f, position.y - 2.5f });
			footprint.setFillColor(sf::Color::Red);

			footprints.push_back(footprint);
		}

		circles_points.push_back(circle_point);
		positions.append(position);
	}

	for (auto circle : circles_points)
	{
		window.draw(circle);
	}

	if (circles_points.size() != 1)
	{
		rgb rgbcol;
		rgbcol.r = color.r;
		rgbcol.g = color.g;
		rgbcol.b = color.b;

		hsv hsvcol = rgb2hsv(rgbcol);
		hsvcol.s = 100;
		hsvcol.h += 10;

		rgbcol = hsv2rgb(hsvcol);
		drawBazierCurve(positions, t, sf::Color(rgbcol.r, rgbcol.g, rgbcol.b), footprints, window);
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Bezier Curve");
	
	sf::VertexArray points;
	points.setPrimitiveType(sf::LinesStrip);
	points.append(sf::Vertex({ 200, 250 }));
	points.append(sf::Vertex({ 150, 150 }));
	points.append(sf::Vertex({ 300, 150 }));
	points.append(sf::Vertex({ 350, 250 }));
	points.append(sf::Vertex({ 400, 175 }));

	std::vector<sf::CircleShape> footprints;

	float t = 0.0f;
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);

		drawBazierCurve(points, t, sf::Color::White, footprints, window);
		if (clock.getElapsedTime().asMilliseconds() >= 10 && t < 1.0f)
		{	
			t += 0.01;
			clock.restart();
		}

		for (auto circle : footprints)
		{
			window.draw(circle);
		}
		window.display();
	}

	return 0;
}