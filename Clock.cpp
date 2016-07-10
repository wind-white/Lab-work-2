#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>

int main()
{
	// Определение некоторых переменных и констант
	const int screenWidth = 800;
	const int screenHeight = 600;
	const float PI = 3.1415927;
	const int clockCircleSize = 250;
	const int clockCircleThickness = 2;
	int x, y;
	float angle = 0.0;

	// Установка урованю мультисэмплинга
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Создание окна приложения
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML Analog Clock", sf::Style::Close, settings);

	// Определение центра окна
	sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	// Создание списка для точек для часов
	sf::CircleShape dot[60];

	// Создание точек и помещение их в самые правые позиции
	for (int i = 0; i<60; i++)
	{
		x = (clockCircleSize - 10) * cos(angle);
		y = (clockCircleSize - 10) * sin(angle);

		if (i % 5 == 0)
			dot[i] = sf::CircleShape(3);
		else
			dot[i] = sf::CircleShape(1);
		dot[i].setFillColor(sf::Color::Black);
		dot[i].setOrigin(dot[i].getGlobalBounds().width / 2, dot[i].getGlobalBounds().height / 2);
		dot[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);

		angle = angle + ((2 * PI) / 60);
	}

	// Создание контура часов
	sf::CircleShape clockCircle(clockCircleSize);

	clockCircle.setPointCount(100);
	clockCircle.setOutlineThickness(clockCircleThickness);
	clockCircle.setOutlineColor(sf::Color::Black);
	clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2, clockCircle.getGlobalBounds().height / 2);
	clockCircle.setPosition(window.getSize().x / 2 + clockCircleThickness, window.getSize().y / 2 + clockCircleThickness);

	// Создание другого круга для центра
	sf::CircleShape centerCircle(10);

	centerCircle.setPointCount(100);
	centerCircle.setFillColor(sf::Color::Red);
	centerCircle.setOrigin(centerCircle.getGlobalBounds().width / 2, centerCircle.getGlobalBounds().height / 2);
	centerCircle.setPosition(windowCenter);

	// Создание часов, минут и секунд
	sf::RectangleShape hourHand(sf::Vector2f(5, 180));
	sf::RectangleShape minuteHand(sf::Vector2f(3, 240));
	sf::RectangleShape secondsHand(sf::Vector2f(2, 250));

	hourHand.setFillColor(sf::Color::Black);
	minuteHand.setFillColor(sf::Color::Black);
	secondsHand.setFillColor(sf::Color::Red);

	hourHand.setOrigin(hourHand.getGlobalBounds().width / 2, hourHand.getGlobalBounds().height - 25);
	minuteHand.setOrigin(minuteHand.getGlobalBounds().width / 2, minuteHand.getGlobalBounds().height - 25);
	secondsHand.setOrigin(secondsHand.getGlobalBounds().width / 2, secondsHand.getGlobalBounds().height - 25);

	hourHand.setPosition(windowCenter);
	minuteHand.setPosition(windowCenter);
	secondsHand.setPosition(windowCenter);

	// Создание звукового эффекта
	sf::Music clockTick;
	if (!clockTick.openFromFile("resources/clock-1.wav"))
		return EXIT_FAILURE;
	clockTick.setLoop(true);
	clockTick.play();

	// Создание фона для часов
	sf::Texture clockImage;
	if (!clockImage.loadFromFile("resources/clock-image.jpg"))
	{
		return EXIT_FAILURE;
	}

	clockCircle.setTexture(&clockImage);
	clockCircle.setTextureRect(sf::IntRect(40, 0, 500, 500));

	while (window.isOpen())
	{
		// Обработка событий
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Окно закрыто: выход
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Получить системное время
		std::time_t currentTime = std::time(NULL);

		struct tm * ptm = localtime(&currentTime);

		hourHand.setRotation(ptm->tm_hour * 30 + (ptm->tm_min / 2));
		minuteHand.setRotation(ptm->tm_min * 6 + (ptm->tm_sec / 12));
		secondsHand.setRotation(ptm->tm_sec * 6);

		// Очистить окно
		window.clear(sf::Color::White);

		// Нарисовать все части часов
		window.draw(clockCircle);

		for (int i = 0; i<60; i++)
		{
			window.draw(dot[i]);
		}

		window.draw(hourHand);
		window.draw(minuteHand);
		window.draw(secondsHand);
		window.draw(centerCircle);

		// Вывести всё на экран
		window.display();
	}

	return EXIT_SUCCESS;
}