#pragma once

#include <string>
#include <SFML/Network.hpp>
#include <memory>
#include <utility>
#include <SFML/Graphics.hpp>


class Player
{
	unsigned int id;
	std::string name;
	sf::TcpSocket* tcpSocket;
	sf::UdpSocket* udpSocket;
	sf::IpAddress playerIp;
	unsigned short udpPortReceive;
	unsigned short udpPortSend;
	sf::Vector2f position;  //player position, in world coordinates (absolute coordinates)
	float speed;     //player speed
	float defRadius;    //default radius set in Player constructor
	float radius;    //player's circle's radius
	float points;				//player's points above 330f.0 to reduce movement lag in client
	sf::Vector2f mapSize;   //player's map size
	sf::Vector2f mapPosition;//player's map's location relative to world coordinates
	sf::Vector2f windowSize;//player's window size
	sf::Color color; //player's color
	bool initReady;

	//need for skills
	sf::Clock foodInvTime;
	sf::Clock invClockReload; 
	sf::Clock invClockUse;
	bool invisibleAvailable; //you can use skill
	bool invisibleActive; //skill is active
	int invisibleTime; //reload time in milisec for skill
	//bool updateInvisble; //if update is available
	bool invisibilityChanged;
	bool invisibleAvailableChanged;
	int invisibleDuration; //inv. duraturion can upgrade

	sf::Clock foodSpeedTime;
	sf::Clock speedClockReload;
	sf::Clock speedClockUse;
	bool speedAvailable; //you can use skill
	bool speedActive;	//skill is active
	int speedTime;		//reload time in milisec for skill
	//bool updateSpeed; //if update is available
	bool speedAvailableChanged;
	int speedDuration; //speed duration
	float speedBoost; //speed boost can upgrade

	int numberOfUpdate; //number of updated skills
	int canUpdateNumber; //number of available updates 0 if no update for client
	bool canUpdateNumberChanged;

public:
	Player(int id, sf::TcpSocket *tcpSocket);
	~Player();

	sf::TcpSocket* getTcpSocket() const;
	sf::UdpSocket* getUdpSocket() const;

	unsigned int getId() const;

	std::string getName() const;
	void setName(std::string _name);

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f _position);

	sf::Vector2f getMapSize() const;
	void setMapSize(sf::Vector2f _mapSize);

	sf::Vector2f getMapPosition() const;
	void setMapPosition(sf::Vector2f _mapPosition);

	sf::Vector2f getWindowSize() const;
	void setWindowSize(sf::Vector2f _windowSize);

	float getPoints() const;

	float getRadius() const;
	void setRadius(float _radius);

	sf::IpAddress getPlayerIp() const;
	void setPlayerIp(sf::IpAddress _playerIp);

	sf::UdpSocket* bindUdpSocket(unsigned short _udpPortReceive, unsigned short _udpPortSend);
	unsigned short getUdpPortSend() const;
	unsigned short getUdpPortReceive() const;

	sf::Vector2f getMousePosition() const;
	void setMousePosition(sf::Vector2f _mousePosition);

	void initPosition();

	sf::Color getColor() const;
	void setColor(sf::Color _color);

	bool getInitReady() const;
	void setInitReady(bool _initReady);

	//skills functions
	void skillChecking(); //ez nezi a skillek kezeleset, aktivalja es elveszi

	bool isInvisible(); //aktiv e a skill
	bool isSpeeding(); //active e a skill
	void invisibleActivate(); //ezt kell meghivni hogy aktivaljuk
	void speedActivate(); // ezt kell meghivni hogy aktivaljuk

	void updateSkill(int key); //ez fejleszti a skilleket
	//bool canUpdataInv(); //van e adott pontszam a fejlesztesre 
	//bool canUpdateSpeed(); //van e adott pontszam a fejlesztesre
	int getUpdateAvailable();

	bool getInvisibilityChanged();
	void setInvisibilityChanged(bool _invisibilityChanged);

	bool getInvisibleAvailableChanged();
	void setInvisibleAvailableChanged(bool _invisibleAvailableChanged);

	bool getSpeedAvailableChanged();
	void setSpeedAvailableChanged(bool _speedAvailableChanged);

	bool getInvisibleAvailable();
	bool getSpeedAvailable();

	float getSpeed();

	void invisibleDeActivate();
	void speedDeActivate();

	bool getCanUpdateNumberChanged();
	void setCanUpdateNumberChanged(bool _canUpdateNumberChanged);
};

