#include <vector>

#include <SFML/Graphics.hpp>

#include "CSegWay.hpp"

#ifdef SFML_STATIC 
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "winmm.lib")
	#pragma comment(lib, "jpeg.lib")

	#ifdef NDEBUG
	#pragma comment(lib, "sfml-system-s.lib")
	#pragma comment(lib, "sfml-window-s.lib")
	#pragma comment(lib, "sfml-graphics-s.lib")
	#else
	#pragma comment(lib, "sfml-system-s-d.lib")
	#pragma comment(lib, "sfml-window-s-d.lib")
	#pragma comment(lib, "sfml-graphics-s-d.lib")
	#endif

#else
	#ifdef NDEBUG
	#pragma comment(lib, "sfml-system.lib")
	#pragma comment(lib, "sfml-window.lib")
	#pragma comment(lib, "sfml-graphics.lib")
	#else
	#pragma comment(lib, "sfml-system-d.lib")
	#pragma comment(lib, "sfml-window-d.lib")
	#pragma comment(lib, "sfml-graphics-d.lib")
	#endif

#endif

#define GRID_CELL_SIZE 8.f

void fillCell(sf::RenderWindow &window, sf::Uint32 x, sf::Uint32 y, const sf::Color &color)
{
	sf::RectangleShape rshp;
	rshp.setFillColor(color);
	rshp.setPosition(sf::Vector2f((x * GRID_CELL_SIZE)-1, (y * GRID_CELL_SIZE)-1));
	rshp.setSize(sf::Vector2f(GRID_CELL_SIZE, GRID_CELL_SIZE));

	window.draw(rshp);
}


int main()
{
	sf::ContextSettings settings;
	//settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1024, 768), "Simulation: Real Map", sf::Style::Default, settings);
	sf::RenderWindow segWayMapWindow(sf::VideoMode(1024, 768), "Simulation: SegWay Map (read only)", sf::Style::Default, settings);
	sf::View mainView = window.getDefaultView();

	bool isSimulating = false;
	bool isMouseLeftDown = false;
	bool drawGrid = false;

	sf::Vertex pendingLine[2];
	pendingLine[0].color = sf::Color::Black;
	pendingLine[1].color = sf::Color::Black;

	sf::VertexArray walls(sf::Lines);
	std::vector<sf::RectangleShape> obstacles;
	sf::Vector2i lastObstaclePos;

	CSegWay segWay;

	int cellX = 0;
	int cellY = 0;

	sf::Clock timeCounter;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					isSimulating = !isSimulating;
				}
				else if (event.key.code == sf::Keyboard::G)
				{
					drawGrid = !drawGrid;
				}
				else if (event.key.code == sf::Keyboard::T)
				{
					segWay.setTarget(sf::Vector2f(sf::Mouse::getPosition(window)));
				}
				else if (event.key.code == sf::Keyboard::R)
				{
					segWay = CSegWay();
					obstacles.clear();
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				const auto &mb = event.mouseButton;
				if (mb.button == sf::Mouse::Left)
				{
					isMouseLeftDown = true;
					if (obstacles.size() == 0)
					{
						lastObstaclePos = sf::Vector2i(mb.x, mb.y);
						sf::RectangleShape obstacle(sf::Vector2f(GRID_CELL_SIZE, GRID_CELL_SIZE)*2.f);
						const float x = static_cast<float>(mb.x);
						const float y = static_cast<float>(mb.y);
						obstacle.setPosition(x, y);
						obstacle.setFillColor(sf::Color::Black);
						obstacles.push_back(obstacle);

						const float GS2 = GRID_CELL_SIZE * 2.f;
						auto &allBlocks = segWay.mBlocked;
						const auto &map = segWay.mNavi.mMap;
						for (float i = x;i < (x + GS2);++i)
							for (float j = y;j < (y + GS2);++j)
								allBlocks.insert(map.getIdxFromPos(sf::Vector2f(i, j)));
					}
					//pendingLine[0].position.x = static_cast<float>(event.mouseButton.x);
					//pendingLine[0].position.y = static_cast<float>(event.mouseButton.y);
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				const auto &mb = event.mouseButton;
				if (mb.button == sf::Mouse::Left)
				{
					isMouseLeftDown = false;
					//walls.append(pendingLine[0]);
					//walls.append(pendingLine[1]);
					/*sf::RectangleShape obstacle(sf::Vector2f(GRID_CELL_SIZE, GRID_CELL_SIZE)*2.f);
					const float x = static_cast<float>(event.mouseButton.x);
					const float y = static_cast<float>(event.mouseButton.y);
					obstacle.setPosition(x, y);
					obstacle.setFillColor(sf::Color::Black);
					obstacles.push_back(obstacle);

					const float GS2 = GRID_CELL_SIZE * 2.f;
					auto &allBlocks = segWay.mBlocked;
					const auto &map = segWay.mNavi.mMap;
					for (float i = x;i < (x + GS2);++i)
						for (float j = y;j < (y + GS2);++j)
							allBlocks.insert(map.getIdxFromPos(sf::Vector2f(i, j)));*/

				}
				else if (mb.button == sf::Mouse::Right)
				{
					/*const auto vertexWallCnt = walls.getVertexCount();
					if(!isMouseLeftDown && (vertexWallCnt > 1))
						walls.resize(vertexWallCnt - 2);*/
				}
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				const auto &mm = event.mouseMove;

				const int x = mm.x;
				const int y = mm.y;

				/*pendingLine[1].position.x = x;
				pendingLine[1].position.y = y;

				cellX = static_cast<int>(x / GRID_CELL_SIZE);
				cellY = static_cast<int>(y / GRID_CELL_SIZE);*/

				if (isMouseLeftDown)
				{
					int dxy = abs(lastObstaclePos.x - x) + abs(lastObstaclePos.y - y);
					if (dxy > (GRID_CELL_SIZE*2))
					{
						sf::RectangleShape obstacle(sf::Vector2f(GRID_CELL_SIZE, GRID_CELL_SIZE)*2.f);
						lastObstaclePos = sf::Vector2i(x, y);
						obstacle.setPosition(x, y);
						obstacle.setFillColor(sf::Color::Black);
						obstacles.push_back(obstacle);

						const int GS2 = static_cast<int>(GRID_CELL_SIZE * 2.f);
						auto &allBlocks = segWay.mBlocked;
						const auto &map = segWay.mNavi.mMap;
						for (int i = x;i < (x + GS2);++i)
							for (int j = y;j < (y + GS2);++j)
								allBlocks.insert(map.getIdxFromPos(sf::Vector2f(i, j)));
					}
				}

			}
			else if (event.type == sf::Event::Resized)
				window.setView( mainView = sf::View(
					sf::FloatRect(
							0.f,
							0.f,
							static_cast<float>(event.size.width),
							static_cast<float>(event.size.height)
						)
				));
			else if (event.type == sf::Event::Closed)
				window.close();
		}

		const auto timeDelta = timeCounter.getElapsedTime().asSeconds();
		timeCounter.restart();

		window.clear(sf::Color::White);

		
		// update path/segway
		if (isSimulating)
		{
			segWay.update(timeCounter.getElapsedTime().asSeconds());
		}


		// draw everything here

		// first draw grid (as background)
		if (drawGrid)
		{
			const auto sizeX = static_cast<float>(window.getSize().x);
			const auto sizeY = static_cast<float>(window.getSize().y);
			
			const auto gridCellSize = GRID_CELL_SIZE;
			const auto gridColor = sf::Color::Black;

			sf::VertexArray lines(sf::Lines);
			for (float x = gridCellSize-1; x < sizeX; x += gridCellSize)
			{
				lines.append(sf::Vertex(sf::Vector2f(x, 0.f), gridColor));
				lines.append(sf::Vertex(sf::Vector2f(x, sizeY), gridColor));
			}
			for (float y = gridCellSize-1; y < sizeY; y += gridCellSize)
			{
				lines.append(sf::Vertex(sf::Vector2f(0, y), gridColor));
				lines.append(sf::Vertex(sf::Vector2f(sizeX, y), gridColor));
			}
			window.draw(lines);
		}

		fillCell(window, cellX, cellY, sf::Color::Green);

		// then draw walls
		//window.draw(walls);

		// then draw pending line
		if (isMouseLeftDown)
			window.draw(pendingLine, 2, sf::Lines);

		for (const auto &obstacle : obstacles)
			window.draw(obstacle);

		// last draw segway
		segWay.draw(window);

		window.display();


		if (segWayMapWindow.isOpen())
		{
			while (segWayMapWindow.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					segWayMapWindow.close();
			}
			segWayMapWindow.clear(sf::Color::White);
			segWay.draw(segWayMapWindow);
			segWay.mNavi.drawMap(segWayMapWindow);
			segWayMapWindow.display();
		}
	}

	return 0;
}