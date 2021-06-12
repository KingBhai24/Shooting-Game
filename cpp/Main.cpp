#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;
//--------------------------------------//
void ItoA(int x, char* y)
{
	if (x == 0)
	{
		y[0] = '0';
		y[1] = 0;
		return;
	}
	char a[10];
	int b;
	int i = 0;
	int j = 0;

	while (x > 0)
	{
		b = x % 10;
		x = x / 10;
		a[i] = b + 48;
		i++;
	}
	i--;
	for (; i >= 0; j++, i--)
		y[j] = a[i];
	y[j] = 0;
}
//--------------------------------------//
struct WindowValues {
	sf::RenderWindow object;
	sf::Uint32 style;
	string header;
	sf::Vector2i fullSize;
	sf::Vector2i playSize;
	int lanes;
	float laneGroundMargin;
};
struct BackgroundValues {
	sf::Sprite object;
	sf::Texture texture;
};
struct ShapeValues {
	sf::RectangleShape* objects = NULL;
	int arraySize;
};
struct SampleShapeValues {
	sf::Vector2f size;
	sf::Color borderColor;
	float rotation;
	float borderThickness;
};
struct PlayerValues {
	sf::RectangleShape object;
	sf::Vector2f size;
	sf::Vector2f position;
	sf::Color color;
	sf::Color borderColor;
	float borderThickness;
};
struct BulletValues {
	sf::CircleShape object;
	sf::Color color;
	sf::Color borderColor;
	float radius;
	float borderThickness;
};
struct RunningShapeValues {
	sf::RectangleShape* objects = NULL;
	int arraySize;
	float fallingSpeed;
};
struct RunningBulletValues {
	sf::CircleShape* objects = NULL;
	int arraySize;
	int allowedBullets;
	float speed;
};
struct TextValues {
	sf::Text object;
	sf::Font font;
	sf::Color color1;
	sf::Color color2;
	sf::Uint32 style;
	sf::Vector2f position;
	sf::Vector2f space;
	int size;
};
struct RunningStatsValues {
	int life;
	int score;
	int high_score;
	int max_bullets;
	int bullets;
	int level;
	int ScoreIncrement;
};
struct AutoShootValues {
	bool shoot;
	float interval;
};
struct AutoMoveValues {
	int move;
	float interval;
};
struct ExtraValues {
	float updateTimer;
	float createTimer;
	bool display;	//set to true when any changes in screen is done and has to be shown.
	int lastInput;	//stores last input of user.
	int BulletarrayIndex;
	int ShapeArrayIndex;
};

WindowValues WV1;
BackgroundValues BV1;
ShapeValues SV1;
SampleShapeValues SSV1;
PlayerValues PV1;
BulletValues BuV1;
RunningShapeValues RSV1;
RunningBulletValues RBV1;
RunningStatsValues RuSV1;
AutoShootValues ASV1;
AutoMoveValues AMV1;
TextValues TV1;
ExtraValues EV1;
//--------------------------------------//
void DeallocateMemory()
{
	if (SV1.objects != NULL)
	{
		delete[] SV1.objects;
		SV1.objects = NULL;
	}
	if (RSV1.objects != NULL)
	{
		delete[] RSV1.objects;
		RSV1.objects = NULL;
	}
	if (RBV1.objects != NULL)
	{
		delete[] RBV1.objects;
		RBV1.objects = NULL;
	}
}
//--------------------------------------//
void LoadFromMemory()
{
	if (!BV1.texture.loadFromFile("Images/background.png"))
		cout << "ERROR! Cannot Load Background Texture\n";
	if (!TV1.font.loadFromFile("Fonts/Normal.otf"))
		cout << "ERROR! Cannot load fonts\n";
}
//--------------------------------------//
void PreInitialize()
{
	if (SV1.objects != NULL)
		cout << "ERROR! Shapes Array Not NULL\n";
	if (RSV1.objects != NULL)
		cout << "ERROR! Running Shapes Array Not NULL\n";
	if (RBV1.objects != NULL)
		cout << "ERROR! Running Bullets Array Not NULL\n";

	EV1.createTimer = 0.5f;
	EV1.updateTimer = 0.01f;		//best responce 0.01f
	ASV1.interval = 0.1f;

	WV1.lanes = 5;
	RSV1.fallingSpeed = 1.0f;
	RBV1.speed = 0.5f;
	AMV1.interval = 0.7f;

	RuSV1.life = 10;
	RBV1.allowedBullets = 30;
	ASV1.shoot = false;
	AMV1.move = 0;

	srand(time(0));
	EV1.display = true;
	EV1.lastInput = -1;
	EV1.BulletarrayIndex = -1;
	EV1.ShapeArrayIndex = -1;

	WV1.fullSize.x = 550;
	WV1.fullSize.y = 600;
	WV1.laneGroundMargin = 20;
	WV1.playSize.x = 350;
	WV1.playSize.y = 600;
	WV1.style = sf::Style::Default;
	WV1.header = "Shooter";

	BV1.object.setTexture(BV1.texture);
	SSV1.size.x = float(2 * WV1.playSize.x / WV1.lanes / 7);
	SSV1.size.y = float(2 * WV1.playSize.x / WV1.lanes / 7);
	SSV1.rotation = 45.0f;
	SSV1.borderThickness = 2;
	SSV1.borderColor = sf::Color::Red;

	SV1.arraySize = 10;
	SV1.objects = new sf::RectangleShape[SV1.arraySize];
	for (int i = 0; i < SV1.arraySize; i++)
	{
		SV1.objects[i].setSize(SSV1.size);
		SV1.objects[i].setOrigin(SV1.objects[i].getSize().x / 2 + SSV1.borderThickness, SV1.objects[i].getSize().y / 2 + SSV1.borderThickness);
		SV1.objects[i].setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		SV1.objects[i].setOutlineThickness((float)SSV1.borderThickness);
		SV1.objects[i].setOutlineColor(SSV1.borderColor);
		SV1.objects[i].setRotation(SSV1.rotation);
	}

	PV1.size.x = float(5 * WV1.playSize.x / WV1.lanes / 7);
	PV1.size.y = float(1 * WV1.playSize.x / WV1.lanes / 7);
	PV1.position.x = float(WV1.playSize.x / WV1.lanes / 2);
	PV1.position.y = float(WV1.fullSize.y - WV1.laneGroundMargin);
	PV1.color = sf::Color::Magenta;
	PV1.borderColor = sf::Color::Red;
	PV1.borderThickness = 2;
	PV1.object.setSize(PV1.size);
	PV1.object.setOrigin(PV1.size.x / 2 + PV1.borderThickness, PV1.size.y / 2 + PV1.borderThickness);
	PV1.object.setFillColor(PV1.color);
	PV1.object.setOutlineThickness((float)PV1.borderThickness);
	PV1.object.setOutlineColor(PV1.borderColor);
	PV1.object.setPosition(PV1.position);

	BuV1.radius = float(0.5 * WV1.playSize.x / WV1.lanes / 7);
	BuV1.borderColor = sf::Color::Red;
	BuV1.borderThickness = 2;
	BuV1.color = sf::Color::Magenta;
	BuV1.object.setRadius((float)BuV1.radius);
	BuV1.object.setOrigin((float)BuV1.radius + BuV1.borderThickness, (float)BuV1.radius + BuV1.borderThickness);
	BuV1.object.setFillColor(BuV1.color);
	BuV1.object.setOutlineThickness((float)BuV1.borderThickness);
	BuV1.object.setOutlineColor(BuV1.borderColor);

	RSV1.arraySize = 100;
	RSV1.objects = new sf::RectangleShape[RSV1.arraySize];
	for (int i = 0; i < RSV1.arraySize; i++)
		RSV1.objects[i].setPosition(-1, -1);

	RBV1.arraySize = 30;
	if (RBV1.allowedBullets > RBV1.arraySize)
		cout << "ERROR! Bullets array size is smaller than Allowed Bullets\n";
	RBV1.objects = new sf::CircleShape[RBV1.arraySize];
	for (int i = 0; i < RBV1.arraySize; i++)
		RBV1.objects[i].setPosition(WV1.fullSize.y + 10, WV1.fullSize.y + 10);

	TV1.size = 14;
	TV1.color1 = sf::Color(255, 255, 0);
	TV1.color2 = sf::Color(255, 128, 0);
	TV1.style = sf::Text::Style::Bold;
	TV1.space.x = 20;
	TV1.space.y = 30;
	TV1.position.x = 350;
	TV1.position.y = 10;
	TV1.object.setFont(TV1.font);
	TV1.object.setFillColor(TV1.color1);
	TV1.object.setCharacterSize(TV1.size);
	TV1.object.setStyle(TV1.style);
	TV1.object.setPosition(TV1.position);

	RuSV1.level = 1;
	RuSV1.score = 0;
	RuSV1.high_score = 0;
	RuSV1.ScoreIncrement = 10;
	RuSV1.max_bullets = RBV1.allowedBullets;
	RuSV1.bullets = RBV1.allowedBullets;
}
//--------------------------------------//
bool CheckSpaceForBullets(int& index)
{
	for (int i = 0; i < RBV1.allowedBullets; i++)
		if (RBV1.objects[i].getPosition().x == WV1.fullSize.y + 10 && RBV1.objects[i].getPosition().y == WV1.fullSize.y + 10)
		{
			index = i;
			return true;
		}
	return false;
}
void CreateBullet(int index)
{
	RBV1.objects[index].setFillColor(BuV1.object.getFillColor());
	RBV1.objects[index].setOrigin(BuV1.object.getOrigin());
	RBV1.objects[index].setOutlineColor(BuV1.object.getOutlineColor());
	RBV1.objects[index].setOutlineThickness(BuV1.object.getOutlineThickness());
	RBV1.objects[index].setRadius(BuV1.object.getRadius());
	RBV1.objects[index].setPosition(PV1.object.getPosition().x, PV1.object.getPosition().y - BuV1.radius - PV1.object.getLocalBounds().height / 2);
}
bool CheckBulletCollisionWithBoundary(int& bulletIndex)
{
	for (int i = 0; i < RBV1.allowedBullets; i++)
		if (RBV1.objects[i].getPosition().y <= 0)
		{
			bulletIndex = i;
			return true;
		}
	return false;
}
bool CheckBulletCollisionWithShapes(int& bulletIndex, int& shapeIndex)
{
	for (int i = 0; i < RSV1.arraySize; i++)
		if (RSV1.objects[i].getPosition().x != -1 && RSV1.objects[i].getPosition().y != -1)
		{
			for (int j = 0; j < RBV1.allowedBullets; j++)
				if (RBV1.objects[j].getPosition().x != WV1.fullSize.y + 10 && RBV1.objects[j].getPosition().y != WV1.fullSize.y + 10)
					if (RBV1.objects[j].getGlobalBounds().intersects(RSV1.objects[i].getGlobalBounds()))
					{
						shapeIndex = i;
						bulletIndex = j;
						return true;
					}
		}
	return false;
}
void RemoveBullet(int bulletIndex)
{
	RBV1.objects[bulletIndex].setPosition(WV1.fullSize.y + 10, WV1.fullSize.y + 10);
}
//--------------------------------------//
bool CheckSpaceForShapes(int& shapeIndex)
{
	for (int i = 0; i < RSV1.arraySize; i++)
		if (RSV1.objects[i].getPosition().x == -1 && RSV1.objects[i].getPosition().y == -1)
		{
			shapeIndex = i;
			return true;
		}
	return false;
}
void CreateShape(int shapeIndex)
{
	int a = rand() % SV1.arraySize;
	int b = rand() % WV1.lanes;
	b = WV1.playSize.x / WV1.lanes / 2 + b * WV1.playSize.x / WV1.lanes;

	RSV1.objects[shapeIndex].setFillColor(SV1.objects[a].getFillColor());
	RSV1.objects[shapeIndex].setOrigin(SV1.objects[a].getOrigin());
	RSV1.objects[shapeIndex].setOutlineColor(SV1.objects[a].getOutlineColor());
	RSV1.objects[shapeIndex].setOutlineThickness(SV1.objects[a].getOutlineThickness());
	RSV1.objects[shapeIndex].setRotation(SV1.objects[a].getRotation());
	RSV1.objects[shapeIndex].setSize(SV1.objects[a].getSize());
	RSV1.objects[shapeIndex].setPosition((float)b, 0);
}
bool CheckShapeCollisionWithBoundary(int& shapeIndex)
{
	for (int i = 0; i < RSV1.arraySize; i++)
		if (RSV1.objects[i].getPosition().y + RSV1.objects[i].getSize().y / 2 >= WV1.playSize.y - WV1.laneGroundMargin)
		{
			shapeIndex = i;
			return true;
		}
	return false;
}
void RemoveShape(int shapeIndex)
{
	RSV1.objects[shapeIndex].setPosition(-1, -1);
}
//--------------------------------------//
void UpdateBullets()
{
	if (CheckBulletCollisionWithShapes(EV1.BulletarrayIndex, EV1.ShapeArrayIndex))
	{
		RemoveBullet(EV1.BulletarrayIndex);
		RemoveShape(EV1.ShapeArrayIndex);
		RuSV1.bullets++;
		RuSV1.score = RuSV1.score + RuSV1.ScoreIncrement;
	}
	if (CheckBulletCollisionWithBoundary(EV1.BulletarrayIndex))
	{
		RemoveBullet(EV1.BulletarrayIndex);
		RuSV1.bullets++;
	}

	for (int i = 0; i < RBV1.allowedBullets; i++)
		if (RBV1.objects[i].getPosition().x != WV1.fullSize.y + 10 && RBV1.objects[i].getPosition().y != WV1.fullSize.y + 10)
			RBV1.objects[i].move(0, (float)-RBV1.speed);
}
void UpdateShapes()
{
	if (CheckShapeCollisionWithBoundary(EV1.ShapeArrayIndex))
	{
		RemoveShape(EV1.ShapeArrayIndex);
		if (RuSV1.life > 0)
			RuSV1.life--;
	}
	for (int i = 0; i < RSV1.arraySize; i++)
		if (RSV1.objects[i].getPosition().x != -1 && RSV1.objects[i].getPosition().y != -1)
		{
			RSV1.objects[i].move(0, (float)RSV1.fallingSpeed);
		}
}
//--------------------------------------//
void MovePlayer(int x)
{
	if (x == 1)
	{
		if (PV1.object.getPosition().x + WV1.playSize.x / WV1.lanes < WV1.playSize.x / WV1.lanes * WV1.lanes)
			PV1.object.move((float)WV1.playSize.x / WV1.lanes, 0);
	}
	else if (x == -1)
	{
		if (PV1.object.getPosition().x - WV1.playSize.x / WV1.lanes > 0)
			PV1.object.move((float)-WV1.playSize.x / WV1.lanes, 0);
	}
}
//--------------------------------------//
void DrawShapes()
{
	for (int i = 0; i < RSV1.arraySize; i++)
		if (RSV1.objects[i].getPosition().x != -1 && RSV1.objects[i].getPosition().y != -1)
			WV1.object.draw(RSV1.objects[i]);
}
void DrawBullets()
{
	for (int i = 0; i < RBV1.allowedBullets; i++)
		if (RBV1.objects[i].getPosition().x != WV1.fullSize.y + 10 && RBV1.objects[i].getPosition().y != WV1.fullSize.y)
			WV1.object.draw(RBV1.objects[i]);
}
void DrawText(char* a, char* b)
{
	sf::Vector2f positionTemp;

	TV1.object.setFillColor(TV1.color1);
	TV1.object.setString(a);
	WV1.object.draw(TV1.object);

	positionTemp.x = (float)TV1.object.findCharacterPos(TV1.space.x).x;		//if number is greater than last index than it will return last index position
	positionTemp.y = (float)TV1.object.findCharacterPos(TV1.space.x).y;

	TV1.object.setFillColor(TV1.color2);
	TV1.object.setPosition(positionTemp);
	TV1.object.setString(b);
	WV1.object.draw(TV1.object);
}
void WrapperDrawText()
{
	char a[30];
	char b[30];

	TV1.object.setPosition(TV1.position);
	strcpy_s(a, "Score = ");
	ItoA(RuSV1.score, b);
	TV1.object.setPosition(TV1.position);
	DrawText(a, b);

	TV1.object.setPosition(TV1.position.x, TV1.object.getPosition().y + TV1.space.y);
	strcpy_s(a, "Life = ");
	ItoA(RuSV1.life, b);
	DrawText(a, b);

	TV1.object.setPosition(TV1.position.x, TV1.object.getPosition().y + TV1.space.y);
	TV1.object.setPosition(TV1.position.x, TV1.object.getPosition().y + TV1.space.y);
	strcpy_s(a, "Max Bullets = ");
	ItoA(RuSV1.max_bullets, b);
	DrawText(a, b);

	TV1.object.setPosition(TV1.position.x, TV1.object.getPosition().y + TV1.space.y);
	strcpy_s(a, "Bullets = ");
	ItoA(RuSV1.bullets, b);
	DrawText(a, b);

	TV1.object.setPosition(TV1.position.x, TV1.object.getPosition().y + TV1.space.y);
	TV1.object.setPosition(TV1.position.x, TV1.object.getPosition().y + TV1.space.y);
	strcpy_s(a, "Level = ");
	ItoA(RuSV1.level, b);
	DrawText(a, b);

	TV1.object.setPosition(TV1.position.x, TV1.object.getPosition().y + TV1.space.y);
	strcpy_s(a, "High Score = ");
	ItoA(RuSV1.high_score, b);
	DrawText(a, b);
}
void Display()
{
	WV1.object.clear();
	WV1.object.draw(BV1.object);
	WrapperDrawText();
	DrawShapes();
	DrawBullets();
	WV1.object.draw(PV1.object);
	WV1.object.display();
	EV1.display = false;
}
//--------------------------------------//
void HandleKeyPressedEvents(sf::Event e)
{
	if (e.key.code == sf::Keyboard::Right)
	{
		MovePlayer(1);
		EV1.display = true;
		AMV1.move = 1;
	}
	else if (e.key.code == sf::Keyboard::Left)
	{
		MovePlayer(-1);
		EV1.display = true;
		AMV1.move = -1;
	}
	else if (e.key.code == sf::Keyboard::Space)
		ASV1.shoot = true;
}
void HandleKeyReleasedEvents(sf::Event e)
{
	if (e.key.code == sf::Keyboard::Space)
		ASV1.shoot = false;
	if (e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right)
		AMV1.move = 0;
}
//--------------------------------------//
void SetTimer(float& timer1, float& timer2, float& timer3, float& timer4)
{
	timer1 = EV1.createTimer;
	timer2 = EV1.updateTimer;
	timer3 = ASV1.interval;
	timer4 = AMV1.interval;
}
//--------------------------------------//
void RunWindow()
{
	sf::Clock timer;
	float time1;
	float time2;
	float time3;
	float time4;
	WV1.object.create(sf::VideoMode(WV1.fullSize.x, WV1.fullSize.y), WV1.header, WV1.style);

	SetTimer(time1, time2, time3, time4);
	timer.restart();
	while (WV1.object.isOpen())
	{
		sf::Event e;
		while (WV1.object.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				WV1.object.close();
				return;
			}
			if (e.type == sf::Event::KeyPressed)
				HandleKeyPressedEvents(e);
			if (e.type == sf::Event::KeyReleased)
				HandleKeyReleasedEvents(e);
		}
		if (timer.getElapsedTime() >= sf::seconds(time1))
		{
			if (CheckSpaceForShapes(EV1.ShapeArrayIndex))
				CreateShape(EV1.ShapeArrayIndex);
			else
				cout << "Shape Array Full\n";
			time1 = time1 + EV1.createTimer;
		}
		if (timer.getElapsedTime() >= sf::seconds(time2))
		{
			UpdateShapes();
			UpdateBullets();
			time2 = time2 + EV1.updateTimer;
			EV1.display = true;
		}
		if (ASV1.shoot)
			if (timer.getElapsedTime() >= sf::seconds(time3))
			{
				if (CheckSpaceForBullets(EV1.BulletarrayIndex))
				{
					CreateBullet(EV1.BulletarrayIndex);
					RuSV1.bullets--;
				}
				time3 = time3 + ASV1.interval;
			}
		if (AMV1.move != 0)
			if (timer.getElapsedTime() >= sf::seconds(time4))
			{
				MovePlayer(AMV1.move);
				time4 = time4 + AMV1.interval;
			}
		if (EV1.display)
			Display();
	}
}
//--------------------------------------//
int main()
{
	LoadFromMemory();
	DeallocateMemory();
	PreInitialize();
	RunWindow();
}
//--------------------------------------//