//============================================================================
// Name        : RushHour.cpp
// Author      : Ahmed Raza
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <iostream>
#include <string>
#include <cmath> // for basic math functions such as cos, sin, sqrt
#include <fstream>
//libraries for adding music
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
using namespace std;

// -------------------- Global Variables ---------------------
float Time_Left = 180;
string playerName = "";
bool enteringName = true;
bool game_ended = false;

// Variables for Role selection
bool selectingRole = true;
char selectedRole = '0'; // 'T' for Taxi, 'D' for Delivery

// -------------------- Global Variables ---------------------

class Station
{
private:
    int x, y;

public:
    Station(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
    // function for drawing the role changing station
    void draw()
    {
        DrawSquare(x, y, 50, colors[ORANGE]);
    }
    int getX() const
    {
        return x;
    }
    int getY() const
    {
        return y;
    }
};

class PickupPoints
{
protected:
    int x, y;

public:
    int typeId; // this is for the type of role i.e(passenger or package delivery)
    PickupPoints(int x = 0, int y = 0)
    {
        this->x = x;
        this->y = y;
        typeId = 0;
    }
    virtual void draw() = 0;
    virtual int getX() const
    {
        return x;
    }
    virtual int getY() const
    {
        return y;
    }
    virtual ~PickupPoints() {}
};

class Passenger : public PickupPoints
{
public:
    Passenger(int a = 0, int b = 0) : PickupPoints(a, b)
    {
        typeId = 1; // setting 1 for passenger
    }
    void draw() override
    {
       DrawCircle(x + 25, y + 45, 6, colors[BLACK]);
       DrawRoundRect(x + 20, y + 15, 10, 30, colors[BLACK], 4);
       DrawLine(x + 20, y + 35, x + 10, y + 30, 6, colors[BLACK]);
       DrawLine(x + 30, y + 35, x + 40, y + 30, 6, colors[BLACK]); 
       DrawLine(x + 20, y + 15, x + 15, y + 5, 6, colors[BLACK]);
       DrawLine(x + 30, y + 15, x + 35, y + 5, 6, colors[BLACK]); 
    }
};

class DeliveryPackage : public PickupPoints
{
public:
    DeliveryPackage(int a = 0, int b = 0) : PickupPoints(a, b)
    {
        typeId = 2; // setting 2 for package
    }
    void draw() override
    {
        DrawSquare(x + 5, y + 5, 40, colors[BROWN]);
    }
};

class Destination
{
protected:
    int x, y;

public:
    Destination(int x = 0, int y = 0)
    {
        this->x = x;
        this->y = y;
    }
    virtual void draw() = 0;
    virtual int getX() const
    {
        return x;
    }
    virtual int getY() const
    {
        return y;
    }
    virtual ~Destination() {}
};

class PassengerDestination : public Destination
{
public:
    PassengerDestination(int x = 0, int y = 0) : Destination(x, y) {}
    void draw() override
    {
        DrawTriangle(x + 10, y + 10, x + 40, y + 10, x + 25, y + 40, colors[GREEN]);
    }
};

class PackageDestination : public Destination
{
public:
    PackageDestination(int x = 0, int y = 0) : Destination(x, y) {}
    void draw() override
    {
        DrawRectangle(x + 10, y + 10, 30, 30, colors[PURPLE]);
    }
};

class FuelStation
{
private:
    int x, y; // coordinates where it is to be drawn
public:
    FuelStation(int x = 0, int y = 0)
    {
        this->x = x;
        this->y = y;
    }

    void draw()
    {
        DrawSquare(x, y, 50, colors[RED]);
    }

    int getX() const
    {
        return x;
    }
    int getY() const
    {
        return y;
    }
};

class OtherCars
{
    int x, y;   // current coordinates
    int dx, dy; // coordinates to be added in current ones
public:
    OtherCars() {}
    OtherCars(int startX, int startY, int dirX, int dirY) : x(startX), y(startY), dx(dirX), dy(dirY) {}
    void draw()
    {
        DrawRectangle(x + 10, y + 15, 30, 15, colors[GRAY]); // Body
        DrawRectangle(x + 15, y + 30, 20, 8, colors[GRAY]);  // Roof
        DrawCircle(x + 13, y + 10, 6, colors[BLACK]);        // Left wheel
        DrawCircle(x + 37, y + 10, 6, colors[BLACK]);        // Right wheel
        DrawCircle(x + 13, y + 10, 2, colors[DARK_GRAY]);    // Left rim
        DrawCircle(x + 37, y + 10, 2, colors[DARK_GRAY]);    // Right rim
    }

    void move(int arr2d[15][15])
    {
        // final coordinates
        int nextX = x + dx;
        int nextY = y + dy;

        // for searching the array and bounds
        int gx = nextX / 50;
        int gy = nextY / 50;

        // for checking bounds and the cell to be empty
        if (gx < 0 || gx >= 15 || gy < 0 || gy >= 15 || arr2d[gx][gy] != 0)
        {
            int dir = rand() % 4; // any direction (up, down, left, right)
            switch (dir)
            {
            case 0:
                dx = 50;
                dy = 0;
                break; // right
            case 1:
                dx = -50;
                dy = 0;
                break; // left
            case 2:
                dx = 0;
                dy = 50;
                break; // up
            case 3:
                dx = 0;
                dy = -50;
                break; // down
            }
            nextX = x + dx;
            nextY = y + dy;
            gx = nextX / 50;
            gy = nextY / 50;
            if (gx < 0 || gx >= 15 || gy < 0 || gy >= 15 || arr2d[gx][gy] != 0)
            {
                return; // Try again next update
            }
        }
        // update current location
        x = nextX;
        y = nextY;
    }

    int getX() const
    {
        return x;
    }
    int getY() const
    {
        return y;
    }
};
// class for buildings, trees
class Obstacle
{
private:
    int x, y; // current coordinates
public:
    Obstacle(int x = 0, int y = 0)
    {
        this->x = x;
        this->y = y;
    }
    void drawbuilding()
    {
        DrawSquare(x, y, 50, colors[BLACK]);
    }
    void drawtree()
    {
        DrawRectangle(x + (50 - 10) / 2, y, 10, 15, colors[BROWN]);
        DrawCircle(x + 25, y + 15 + 16 / 2, 14, colors[GREEN]);
    }
    int getX() const { return x; }
    int getY() const { return y; }
};

class Car
{
private:
    int x, y;     // current coordinates
    float *color; // color of car
    float fuel;     // variable for tracking fuel
public:
    Car() {}
    Car(int startX, int startY, float *col = colors[WHITE])
    {
        x = startX;
        y = startY;
        this->color = col;
        fuel = 100; // setting fuel initially full
    }
    void move(int dx, int dy)
    {
        int nx = x + dx;
        int ny = y + dy;
        nx = (nx / 50) * 50; // ensuring that the value is exact to avoid mismovements
        ny = (ny / 50) * 50;
        if (nx >= 0 && nx < 750 && ny >= 0 && ny < 750) // if in bounds
        {
            x = nx;
            y = ny;
            fuel -= 0.25; // decreasing fuel while moving
        }
    }
    void draw()
    {
        DrawRectangle(x + 10, y + 15, 30, 15, color);             // Body
        DrawRectangle(x + 15, y + 30, 20, 8, colors[LIGHT_BLUE]); // Roof
        DrawCircle(x + 13, y + 10, 6, colors[BLACK]);             // Left wheel
        DrawCircle(x + 37, y + 10, 6, colors[BLACK]);             // Right wheel
        DrawCircle(x + 13, y + 10, 2, colors[DARK_GRAY]);         // Left rim
        DrawCircle(x + 37, y + 10, 2, colors[DARK_GRAY]);         // Right rim
    }
    int getX() const
    {
        return x;
    }
    int getY() const
    {
        return y;
    }
    int getfuel()
    {
        return fuel;
    }
    bool refillfuel(int &money) // for refueling
    {
        if (fuel <= 98 && money >= 5) // if fuel is needed and, if player has enough money
        {
            fuel += 2;  // adding fuel
            money -= 5; // taking money
            return true;
        }
        return false;
    }

    ~Car() {}
};

class Game; // declaring it so that if used before its declaration

class Player
{
protected:
    Car car;
    int score, money; // variables for tracking money and score
    bool carrying;    // to know if the player is carrying something
    PickupPoints *currentPickup;
    Destination *destination;

public:
    Player() {}
    Player(int startX, int startY, float *color) : car(startX, startY, color)
    {
        money = 500; // initializing it
        score = 0; // initializing it
        this->carrying = false;
        this->currentPickup = nullptr;
        this->destination = nullptr;
    }
    virtual void pickup(PickupPoints *point, Game &game) = 0;
    virtual void dropoff(Game &game) = 0;    // passing game reference
    virtual void switchRole(Game &game) = 0; // for switching role
    virtual bool isTaxiDriver() const = 0;   // to check role
    void move(int dx, int dy)
    {
        car.move(dx, dy);
    }
    void draw()
    {
        car.draw();
    }
    int getcarposition_x()
    {
        return car.getX();
    }
    int getcarposition_y()
    {
        return car.getY();
    }
    int getscore()
    {
        return score;
    }
    int getmoney()
    {
        return money;
    }
    int getfuel()
    {
        return car.getfuel();
    }
    bool refillfuel()
    {
        return car.refillfuel(money);
    }
    bool isCarrying() const
    {
        return carrying;
    }
    void setCarrying(bool c)
    {
        carrying = c;
    }
    void setPickup(PickupPoints *p)
    {
        currentPickup = p; // setting pickup location
    }
    void setDestination(Destination *d)
    {
        destination = d; // setting destination location
    }
    void setScore(int newScore)
    {
        score = newScore;
    }
    void setMoney(int newMoney)
    {
        money = newMoney;
    }
    PickupPoints *getPickup()
    {
        return currentPickup;
    }
    Destination *getDestination()
    {
        return destination;
    }
    virtual void hitPerson() {}   // if the car hits a person in any role
    virtual void hitObstacle() {} // if the car hits an obstacle in any role
    virtual void hitOtherCar() {} // if the car hits another car in any role
    virtual ~Player()             // destructor
    {
        delete destination;
    }
};

class TaxiDriver : public Player
{
public:
    TaxiDriver(int sx, int sy) : Player(sx, sy, colors[BROWN]) {}

    void pickup(PickupPoints *point, Game &game) override; // declaration of pickup function
    void dropoff(Game &game) override;                     // declaration of dropoff function
    void switchRole(Game &game) override;
    bool isTaxiDriver() const override { return true; } // Implement role check
    void hitPerson() override
    {
        cout << "Taxi hit a person.\n";
        score -= 5; // decreasing 5 scores if taxi hits person
    }
    void hitObstacle() override
    {
        cout << "Taxi hit an obstacle.\n";
        score -= 2; // decreasing 2 scores if taxi hits obstacle
    }
    void hitOtherCar() override
    {
        cout << "Taxi hit another car.\n";
        score -= 3; // decreasing 3 scores if taxi hits another car
    }
};

class DeliveryDriver : public Player
{
public:
    DeliveryDriver(int sx, int sy) : Player(sx, sy, colors[BLUE]) {} // NEW: Blue color

    void pickup(PickupPoints *point, Game &game) override;
    void dropoff(Game &game) override;
    void switchRole(Game &game) override;
    bool isTaxiDriver() const override { return false; } // Implement role check
    void hitPerson() override
    {
        cout << "Delivery driver hit a person.\n";
        score -= 8; // decreasing 8 scores if car hits person
    }
    void hitObstacle() override
    {
        cout << "Delivery driver hit an obstacle.\n";
        score -= 4; // decreasing 4 scores if car hits obstacle
    }
    void hitOtherCar() override
    {
        cout << "Delivery driver hit another car.\n";
        score -= 5; // decreasing 5 scores if car hits car
    }
};

class Menu
{
public:
    void displayMainMenu()
    {
        DrawString(100, 700, "Enter your name to start playing: ", colors[BLUE_VIOLET]);
        DrawString(430, 700, playerName, colors[DARK_GREEN]);
        
        
        DrawString(200, 600, "INSTRUCTIONS: ", colors[INDIGO]);
        DrawString(70, 560, "-> Use arrow keys for moving up, down, right, and left.", colors[DARK_GREEN]);
        DrawString(70, 520, "-> Press (spacebar) to pick package/passenger.", colors[DARK_GREEN]);
        DrawString(70, 480, "-> Press 'D' to drop package/passenger.", colors[DARK_GREEN]);
        DrawString(70, 440, "-> Press 'F' while standing at fuel station to refill.", colors[DARK_GREEN]);
        DrawString(70, 400, "-> Press 'P' while standing at the station to switch your role.", colors[DARK_GREEN]);
        
        DrawString(200, 300, "GAME RULES: ", colors[INDIGO]);
        DrawString(70, 260, "-> The game will be played for 3 minutes.", colors[DARK_GREEN]);
        DrawString(70, 220, "-> You will lose score if you collide with any obstacle or other cars.", colors[DARK_GREEN]);
        DrawString(70, 180, "-> You win the game if your score is more than 100 at the end.", colors[DARK_GREEN]);
        DrawString(70, 140, "-> Game ends if you run out of fuel, or your score becomes 0.", colors[DARK_GREEN]);
        
        
    }
    void displayRoleSelection()
    {
        DrawString(150, 650, "Welcome to Rush Hour Game.", colors[BLUE_VIOLET]);
        DrawString(150, 600, "Press T for Taxi Driver or D for Delivery Driver.", colors[BLUE_VIOLET]);
    }
};

class GameBoard
{
private:
    Obstacle buildings[48]; // Fixed array of buildings
    Obstacle trees[21];
    FuelStation fuelStations[2];
    Passenger passengers[2];
    DeliveryPackage package[2];
    Station station;
    OtherCars *cars[2];
    int fuelstationscount;
    int tree_count;
    int buildings_count;
    int passengers_count;
    int packages_count;
    int othercars_count;
    int car_x, car_y;
    int arr2d[15][15];
    bool isTaxiDriver;
    // to Check if cell has at least two empty adjacent cells
    bool hasTwoEmptyNeighbors(int gx, int gy) const
    {
        int adjacentEmpty = 0;
        if (gx > 0 && arr2d[gx - 1][gy] == 0)
            adjacentEmpty++;
        if (gx < 14 && arr2d[gx + 1][gy] == 0)
            adjacentEmpty++;
        if (gy > 0 && arr2d[gx][gy - 1] == 0)
            adjacentEmpty++;
        if (gy < 14 && arr2d[gx][gy + 1] == 0)
            adjacentEmpty++;
        return adjacentEmpty >= 2;
    }

public:
    GameBoard(bool taxiDriver = true) : buildings_count(0), tree_count(0), fuelstationscount(0), passengers_count(0), packages_count(0), othercars_count(0), car_x(0), car_y(700), isTaxiDriver(taxiDriver)
    {
        initializeBoard();
    }
    void initializeBoard()
    {
        // Initialize grid
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                arr2d[i][j] = 0;
            }
        }
        // Clearing all counts, in case for resetting board
        buildings_count = tree_count = fuelstationscount = passengers_count = packages_count = othercars_count = 0;
        srand(time(0));

        // Placed fixed station at (0, 0) bottom left
        station = Station(0, 0);
        arr2d[0][0] = 6;
        arr2d[0][1] = 0;
        arr2d[1][0] = 0;
        arr2d[1][1] = 0;

        // Place 24 pairs of buildings (48 buildings total)
        for (int i = 0; i < 21;)
        {
            int gx = (rand() % 14) + 1;
            int gy = (rand() % 14) + 1;
            int n1 = gx * 50; // Pixel x
            int n2 = gy * 50; // Pixel y
            // Checking if primary cell can be occupied
            if (n1 >= 50 && n1 <= 700 && n2 >= 50 && n2 <= 700 && arr2d[gx][gy] == 0 &&
                !(gx == 0 && gy == 0) && !(gx == 0 && gy == 1) && !(gx == 1 && gy == 0) && !(gx == 1 && gy == 1))
            {
                bool isHorizontal = rand() % 2; // Randomly choose horizontal or vertical pair
                int gx2, gy2, n3, n4;
                if (!isHorizontal)
                {
                    gx2 = gx;
                    gy2 = gy + 1;
                    n3 = n1;
                    n4 = n2 + 50;
                }
                else
                {
                    gx2 = gx + 1;
                    gy2 = gy;
                    n3 = n1 + 50;
                    n4 = n2;
                }
                // Checking if secondary cell can be occupied
                if (gx2 <= 14 && gy2 <= 14 && n3 >= 50 && n3 <= 700 && n4 >= 50 && n4 <= 700 && arr2d[gx2][gy2] == 0 &&
                    !(gx2 == 0 && gy2 == 0) && !(gx2 == 0 && gy2 == 1) && !(gx2 == 1 && gy2 == 0) && !(gx2 == 1 && gy2 == 1))
                {
                    buildings[buildings_count++] = Obstacle(n1, n2);
                    arr2d[gx][gy] = 1; // storing 1 for buildings
                    buildings[buildings_count++] = Obstacle(n3, n4);
                    arr2d[gx2][gy2] = 1; // storing 1 for buildings
                    i++;
                }
            }
        }
        // Placing 21 trees, each next to a building and not next to another tree
        for (int i = 0; i < 21;)
        {
            int gx = (rand() % 14) + 1;
            int gy = (rand() % 14) + 1;
            int m1 = gx * 50; // Pixel x
            int m2 = gy * 50; // Pixel y
            // Check if cell is empty, not near station, and has at least one building neighbor
            if (m1 >= 50 && m1 <= 700 && m2 >= 50 && m2 <= 700 && arr2d[gx][gy] == 0 &&
                !(gx == 0 && gy == 0) && !(gx == 0 && gy == 1) && !(gx == 1 && gy == 0) && !(gx == 1 && gy == 1))
            {
                bool hasBuildingNeighbor = (gx > 0 && arr2d[gx - 1][gy] == 1) || (gx < 14 && arr2d[gx + 1][gy] == 1) || // condition for checking adjacent building
                                           (gy > 0 && arr2d[gx][gy - 1] == 1) || (gy < 14 && arr2d[gx][gy + 1] == 1);
                bool noTreeNeighbor = !(gx > 0 && arr2d[gx - 1][gy] == 2) && !(gx < 14 && arr2d[gx + 1][gy] == 2) && // condition for checking adjacent tree
                                      !(gy > 0 && arr2d[gx][gy - 1] == 2) && !(gy < 14 && arr2d[gx][gy + 1] == 2);
                if (hasBuildingNeighbor && noTreeNeighbor) // if both are true
                {
                    trees[tree_count++] = Obstacle(m1, m2);
                    arr2d[gx][gy] = 2; // storing 2 for trees
                    i++;
                }
            }
        }
        // Placing fuel stations with at least two adjacent empty cells
        for (int i = 0; i < 2;)
        {
            int f1 = ((rand() % 13) + 1) * 50;
            int f2 = ((rand() % 13) + 1) * 50;
            int gx = f1 / 50;
            int gy = f2 / 50;
            if (f1 >= 50 && f1 <= 700 && f2 >= 50 && f2 <= 700 && arr2d[gx][gy] == 0 && !(gx == 0 && gy == 0) && !(gx == 0 && gy == 1) && !(gx == 1 && gy == 0) && !(gx == 1 && gy == 1) && hasTwoEmptyNeighbors(gx, gy))
            {
                fuelStations[fuelstationscount++] = FuelStation(f1, f2);
                arr2d[gx][gy] = 3; // storing 3 for fuel stations
                i++;
            }
        }

        // Placing passengers with at least two adjacent empty cells
        if (isTaxiDriver)
        {
            for (int i = 0; i < 2;)
            {
                int p1 = ((rand() % 13) + 1) * 50;
                int p2 = ((rand() % 13) + 1) * 50;
                int gx = p1 / 50;
                int gy = p2 / 50;
                if (p1 >= 50 && p1 <= 700 && p2 >= 50 && p2 <= 700 && arr2d[gx][gy] == 0 &&
                    !(gx == 0 && gy == 0) && !(gx == 0 && gy == 1) && !(gx == 1 && gy == 0) && !(gx == 1 && gy == 1) &&
                    hasTwoEmptyNeighbors(gx, gy))
                {
                    passengers[passengers_count++] = Passenger(p1, p2);
                    arr2d[gx][gy] = 4; // storing 4 for passengers
                    i++;
                }
            }
        }
        else
        {
            passengers_count = 0; // Clearing passengers in delivery mode
        }

        // Placing packages with at least two adjacent empty cells
        if (!isTaxiDriver)
        {
            for (int i = 0; i < 2;)
            {
                int q1 = ((rand() % 13) + 1) * 50;
                int q2 = ((rand() % 13) + 1) * 50;
                int gx = q1 / 50;
                int gy = q2 / 50;
                if (q1 >= 50 && q1 <= 700 && q2 >= 50 && q2 <= 700 && arr2d[gx][gy] == 0 &&
                    !(gx == 0 && gy == 0) && !(gx == 0 && gy == 1) && !(gx == 1 && gy == 0) && !(gx == 1 && gy == 1) &&
                    hasTwoEmptyNeighbors(gx, gy))
                {
                    package[packages_count++] = DeliveryPackage(q1, q2);
                    arr2d[gx][gy] = 5; // storing 5 for packages
                    i++;
                }
            }
        }
        else
        {
            packages_count = 0; // Clearing packages in taxi mode
        }

        // Placing other cars
        for (int i = 0; i < 2;)
        {
            int gx = ((rand() % 13) + 1);
            int gy = ((rand() % 13) + 1);
            if (arr2d[gx][gy] == 0 && !(gx == 0 && gy == 0) && !(gx == 0 && gy == 1) && !(gx == 1 && gy == 0) && !(gx == 1 && gy == 1))
            {
                int x = gx * 50;
                int y = gy * 50;
                int dir = rand() % 4; // getting a random direction from(up, down, left, right)
                int dx = 0, dy = 0;
                if (dir == 0)
                    dx = 50;
                else if (dir == 1)
                    dx = -50;
                else if (dir == 2)
                    dy = 50;
                else if (dir == 3)
                    dy = -50;
                cars[othercars_count++] = new OtherCars(x, y, dx, dy);
                i++;
            }
        }
    }
    ~GameBoard()
    {
        for (int i = 0; i < othercars_count; i++)
        {
            delete cars[i];
        }
    }

    void drawBoard()
    {
        for (int i = 0; i < 15; i++)
        {
            DrawLine(50 * i, 0, 50 * i, 750, 1, colors[BLACK]); //
            // DrawLine(0, 50 * i, 750, 50 * i, 1, colors[BLACK]);
        }
        DrawLine(0, 0, 840, 0, 2, colors[BLACK]);
        DrawLine(1, 0, 1, 750, 10, colors[BLACK]);
        DrawLine(0, 750, 750, 750, 10, colors[BLACK]);

        // calling draw functions for everything on the board
        for (int i = 0; i < buildings_count; i++)
        {
            buildings[i].drawbuilding();
        }
        for (int i = 0; i < tree_count; i++)
        {
            trees[i].drawtree();
        }
        for (int i = 0; i < fuelstationscount; i++)
        {
            fuelStations[i].draw();
        }
        if (isTaxiDriver)
        {
            for (int i = 0; i < passengers_count; i++)
            {
                passengers[i].draw(); // drawing passengers in taxi mode
            }
        }
        else
        {
            for (int i = 0; i < packages_count; i++)
            {
                package[i].draw(); // drawing packages in delivery mode
            }
        }
        station.draw();
        for (int i = 0; i < 2; i++)
        {
            cars[i]->draw();
        }
    }
    // for checking if motion of car is possible
    bool search_arr(int next_x, int next_y)
    {
        int gx = next_x / 50;
        int gy = next_y / 50;

        if (gx < 0 || gx >= 15 || gy < 0 || gy >= 15) // for bounds
        {
            return false;
        }
        if (arr2d[gx][gy] == 1 || arr2d[gx][gy] == 2 || (isTaxiDriver && arr2d[gx][gy] == 5) || (!isTaxiDriver && arr2d[gx][gy] == 4))
        {
            return false;
        }
        return true;
    }
    // function for checking if the car is standing on the fuel station or not
    bool matchFuelStation()
    {
        int current_x = car_x / 50;
        int current_y = car_y / 50;
        return (arr2d[current_x][current_y] == 3);
    }
    // function for checking if the car is standing on the role switching station or not
    bool matchStation()
    {
        int current_x = car_x / 50;
        int current_y = car_y / 50;
        return (arr2d[current_x][current_y] == 6);
    }
    PickupPoints *getPickupPoint(int x, int y)
    {
        int gx = x / 50;
        int gy = y / 50;
        if (gx < 0 || gx >= 15 || gy < 0 || gy >= 15)
            return nullptr;
        if (isTaxiDriver && arr2d[gx][gy] == 4)
        {
            for (int i = 0; i < passengers_count; i++)
            {
                if (passengers[i].getX() == x && passengers[i].getY() == y)
                {
                    return &passengers[i];
                }
            }
        }
        else if (!isTaxiDriver && arr2d[gx][gy] == 5)
        {
            for (int i = 0; i < packages_count; i++)
            {
                if (package[i].getX() == x && package[i].getY() == y)
                {
                    return &package[i];
                }
            }
        }
        return nullptr;
    }
    void removePickupPoint(int x, int y)
    {
        int gx = x / 50;
        int gy = y / 50;
        if (isTaxiDriver && arr2d[gx][gy] == 4)
        {
            for (int i = 0; i < passengers_count; i++)
            {
                if (passengers[i].getX() == x && passengers[i].getY() == y)
                {
                    passengers[i] = passengers[--passengers_count];
                    arr2d[gx][gy] = 0;
                    break;
                }
            }
        }
        else if (!isTaxiDriver && arr2d[gx][gy] == 5)
        {
            for (int i = 0; i < packages_count; i++)
            {
                if (package[i].getX() == x && package[i].getY() == y)
                {
                    package[i] = package[--packages_count];
                    arr2d[gx][gy] = 0;
                    break;
                }
            }
        }
    }
    void setcarposition(int car_x, int car_y)
    {
        this->car_x = car_x;
        this->car_y = car_y;
    }
    void resetBoard(bool taxiDriver, int player_x, int player_y) // for resetting the board after switching role
    {
        for (int i = 0; i < othercars_count; i++)
        {
            delete cars[i]; // deleting cars
        }
        buildings_count = tree_count = fuelstationscount = passengers_count = packages_count = othercars_count = 0; // removing everything
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                arr2d[i][j] = 0;
            }
        }
        car_x = player_x;
        car_y = player_y;
        isTaxiDriver = taxiDriver;
        initializeBoard(); // calling the initialze function again
    }
    bool checkOtherCar(int x, int y) // for checking collision with other car
    {
        for (int i = 0; i < othercars_count; i++)
        {
            if (cars[i]->getX() == x && cars[i]->getY() == y)
                return true;
        }
        return false;
    }
    void update() // for controlling movement of other cars
    {
        static float lastUpdate = Time_Left;
        if (Time_Left <= lastUpdate - 0.5)
        {
            for (int i = 0; i < othercars_count; i++)
            {
                cars[i]->move(arr2d);
            }
            lastUpdate = Time_Left;
        }
    }

    bool isCellEmpty(int x, int y) const // for empty cell
    {
        return arr2d[x / 50][y / 50] == 0;
    }
    void spawnNewPickup(bool isTaxiDriver) // spawning new passenger or package after dropping off previous one
    {
        int gx, gy;
        do //getting random untill a valid cell is found
        {
            gx = (rand() % 13) + 1;
            gy = (rand() % 13) + 1;
        } while (arr2d[gx][gy] != 0 || (gx == 0 && gy == 0) || (gx == 0 && gy == 1) || (gx == 1 && gy == 0) || (gx == 1 && gy == 1) || !hasTwoEmptyNeighbors(gx, gy));
        int x = gx * 50;
        int y = gy * 50;
        if (isTaxiDriver)
        {
            passengers[passengers_count++] = Passenger(x, y); // spawning new passenger
            arr2d[gx][gy] = 4;                                // setting array
        }
        else
        {
            package[packages_count++] = DeliveryPackage(x, y); // spawning new package
            arr2d[gx][gy] = 5;                                 // setting array
        }
    }
};
//base class of the entire game
class Game
{
public:
    GameBoard board;
    Menu menu;
    Player *player;
    Game() : board(true), player(nullptr) {}
    void initializePlayer(bool isTaxiDriver) //allocating a new player according to role
    {
        if (player)
            delete player; //deleting previous player
        if (isTaxiDriver) //checking for role
        {
            player = new TaxiDriver(0, 700); //initializing a taxi driver
            board.resetBoard(true, 0, 700); //resetting board after role switch
        }
        else
        {
            player = new DeliveryDriver(0, 700); //initializing a delivery driver
            board.resetBoard(false, 0, 700); //resetting board after role switch
        }
    }
    void display() //main display function calling board display and car display
    {
        //strings at the top of board
        DrawString(30, 840, playerName, colors[BLACK]);
        DrawString(350, 840, "24I-0026", colors[BLACK]);
        DrawString(620, 840, "AI-B", colors[BLACK]);
        DrawString(30, 780, ("Fuel: " + to_string(player->getfuel()) + "%"), colors[RED]);
        DrawString(210, 780, ("Score: " + to_string(player->getscore())), colors[BLACK]);
        DrawString(400, 780, ("Money: $" + to_string(player->getmoney())), colors[BLACK]);
        DrawString(590, 780, ("Time: " + to_string(int(Time_Left)) + "s"), colors[BLACK]);
        board.drawBoard(); //for drawing board
        player->draw(); //for drawing player's car
        if (player->isCarrying() && player->getDestination())
        {
            player->getDestination()->draw(); //drawing destination of package or passenger
        }
    }
    bool matchFuelStation() //calling board's function of fuel station matching
    {
        return board.matchFuelStation();
    }
    bool matchStation() //calling board's function of role changing station matching
    {
        return board.matchStation();
    }
    bool refillfuel()
    {
        if (player->refillfuel())
        {
            return true;
        }
        return false;
    }
    int getfuel() //getter for fuel
    {
        return player->getfuel();
    }
    void pickup()
    {
        // Check adjacent cells for pickups
        int x = player->getcarposition_x();
        int y = player->getcarposition_y();
        //to check if the player is next to the pickup point(up, down, left, right)
        int adjacentCells[4][2] = {
            {x + 50, y}, // Right
            {x - 50, y}, // Left
            {x, y + 50}, // Up
            {x, y - 50}  // Down
        };
        for (int i = 0; i < 4; i++) //checking for all directions
        {
            int adjX = adjacentCells[i][0];
            int adjY = adjacentCells[i][1];
            PickupPoints *point = board.getPickupPoint(adjX, adjY);
            if (point)
            {
                player->pickup(point, *this); //if player picks the package/pasenger
                if (player->isCarrying())
                { 
                    board.removePickupPoint(adjX, adjY); //remove the pickup point
                }
                break; // Pick up only one point
            }
        }
    }
    void dropoff() //calling dropp off func of player
    {
        player->dropoff(*this);
    }
    void switchRole() //calling role switching func of player
    {
        player->switchRole(*this);
    }
    void displayend() //ending screen
    {
        DrawString(305, 480, "GAME OVER", colors[BLACK]);
        DrawString(305, 400, "Final Score: " + to_string(player->getscore()), colors[BLACK]);
        if(player->getscore() >= 100)
                DrawString(305, 440, "YOU WON.", colors[BLACK]);
        game_ended = true;
    }
    ~Game()
    {
        delete player;
    }
};

void TaxiDriver::pickup(PickupPoints *point, Game &game)
{
    if (point->typeId == 1 && !isCarrying()) // Check for Passenger type
    {
        cout << "Passenger picked up by the Taxi Driver.\n";
        setPickup(point); 
        setCarrying(true); //if carrying then true
        int dx, dy;
        do
        {
            dx = ((rand() % 14) + 1) * 50;
            dy = ((rand() % 14) + 1) * 50;
        } while (dx < 50 || dx > 700 || dy < 50 || dy > 700 || !game.board.isCellEmpty(dx, dy));
        setDestination(new PassengerDestination(dx, dy)); //generating random destination point
    }
}
void TaxiDriver::dropoff(Game &game)
{
    if (isCarrying() && getPickup())
    {
        if (getcarposition_x() == getDestination()->getX() && getcarposition_y() == getDestination()->getY())
        {
            cout << "Passenger dropped off by the Taxi Driver.";
            score += 10;
            int fare = (150 + (rand() % 51)); //random fare every time 
            money += fare; //giving fare to player
            cout << " Fare Earned: " << fare << endl;
            setCarrying(false);
            setPickup(nullptr);
            delete getDestination();
            setDestination(nullptr);
            // Spawn a new passenger
            game.board.spawnNewPickup(true);
        }
    }
}

// Defining switchRole outside class
void TaxiDriver::switchRole(Game &game)
{
    if (!isCarrying() && game.matchStation()) //if standing on station and curently not carrying anything
    {
        cout << "Switching from TaxiDriver to DeliveryDriver.\n";
        Player *newPlayer = new DeliveryDriver(0, 0); // Respawn at station
        newPlayer->setScore(game.player->getscore()); //retain score
        newPlayer->setMoney(game.player->getmoney()); //retain money
        game.board.resetBoard(false, 0, 0);
        delete game.player; //deleting old player
        game.player = newPlayer; //generating new player
    }
    else
    {
        cout << "Cannot switch role: Must be at station and not carrying.\n"; 
    }
}
void DeliveryDriver::pickup(PickupPoints *point, Game &game)
{
    if (point->typeId == 2 && !isCarrying()) // Check for DeliveryPackage type
    {
        cout << "Package picked up by the Delivery Driver.\n";
        setPickup(point);
        setCarrying(true); //if carrying then true
        int dx, dy;
        do
        {
            dx = ((rand() % 14) + 1) * 50;
            dy = ((rand() % 14) + 1) * 50;
        } while (dx < 50 || dx > 700 || dy < 50 || dy > 700 || !game.board.isCellEmpty(dx, dy));
        setDestination(new PackageDestination(dx, dy)); //generating random destination point
    }
}
void DeliveryDriver::dropoff(Game &game)
{
    if (isCarrying() && getPickup())
    {
        if (getcarposition_x() == getDestination()->getX() && getcarposition_y() == getDestination()->getY())
        {
            cout << "Package delivered by the Delivery Driver.\n";
            score += 20;
            int fare = (50 + (rand() % 51)); //random fare every time 
            money += fare; //giving fare to player
            cout << " Fare Earned: " << fare << endl;
            setCarrying(false);
            setPickup(nullptr);
            delete getDestination();
            setDestination(nullptr);
            // Spawn a new package
            game.board.spawnNewPickup(false);
        }
    }
}

void DeliveryDriver::switchRole(Game &game)
{
    if (!isCarrying() && game.matchStation()) //if standing on station and curently not carrying anything
    {
        cout << "Switching from DeliveryDriver to TaxiDriver.\n";
        Player *newPlayer = new TaxiDriver(0, 0); // Respawn at station
        newPlayer->setScore(game.player->getscore()); //retain score
        newPlayer->setMoney(game.player->getmoney()); //retain money
        game.board.resetBoard(true, 0, 0); // Reset board with car at (0, 0)
        delete game.player; //deleting old player
        game.player = newPlayer; //generating new player
    }
    else
    {
        cout << "Cannot switch role: Must be at station and not carrying.\n";
    }
}

// -------------------- Global Variables ---------------------

Game game;

// -------------------- Global Variables ---------------------

// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// int xI = 400, yI = 400;

// void drawCar()
// {
//     DrawSquare(xI, yI, 20, colors[WHITE]);
//     glutPostRedisplay();
// }

// bool flag = true;


//not used this function
void moveCar()
{
    /*if (xI > 10 && flag) {
        xI -= 10;
        cout << "going left";
        if(xI < 100)

            flag = false;

    }
    else if (xI < 1010 && !flag) {
        cout << "go right";
        xI += 10;
        if (xI > 900)
            flag = true;
    }*/
}

/*
 * Main Canvas drawing function.
 * */
void GameDisplay() /**/
{
    // set the background color using function glClearColor.
    // to change the background play with the red, green and blue values below.
    // Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

    glClearColor(1 /*Red Component*/, 1,                       // 148.0/255/*Green Component*/,
                 1 /*Blue Component*/, 1 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
    glClear(GL_COLOR_BUFFER_BIT);                              // Update the colors

    // DrawSquare(400, 20, 40, colors[RED]);
    // DrawString( 50, 800, "Score=0", colors[MISTY_ROSE]);
    // Trianlge Vertices v1(300,50) , v2(500,50) , v3(400,250)
    // DrawTriangle( 300, 450 , 340, 450 , 320 , 490, colors[MISTY_ROSE] );
    // DrawLine(int x1, int y1, int x2, int y2, int lwidth, float *color)
    // DrawCircle(50,670,10,colors[RED]);
    // DrawRoundRect(500,200,50,100,colors[DARK_SEA_GREEN],70);
    // drawCar();

    if (selectingRole) //while selecting role, different screen
        game.menu.displayRoleSelection();
    else if (enteringName) //screen while entering name 
        game.menu.displayMainMenu();
    else if (Time_Left <= 0 || game.player->getscore() < 0 || game.getfuel() < 0)
        game.displayend();//ending screen
    else
        game.display(); //display of game 
    glutSwapBuffers(); // do not modify this line..
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */
void NonPrintableKeys(int key, int x, int y)
{
    int dx = 0, dy = 0;
    if (key == GLUT_KEY_LEFT) //left key
        dx = -50;
    else if (key == GLUT_KEY_RIGHT) //right key
        dx = 50;
    else if (key == GLUT_KEY_UP) //up key
        dy = 50;
    else if (key == GLUT_KEY_DOWN) //down key
        dy = -50;

    if (!selectingRole && !enteringName && !game_ended) //if in the actual game then checking for motion 
    {
        int currentX = game.player->getcarposition_x();
        int currentY = game.player->getcarposition_y();
        int nextX = currentX + dx;
        int nextY = currentY + dy;

        // Checking for collisions
        if (game.board.search_arr(nextX, nextY))
        {
            // Checking for other cars
            if (game.board.checkOtherCar(nextX, nextY))
            {
                game.player->hitOtherCar();
            }
            // Check for passengers (only if player is TaxiDriver)
            else if (game.board.getPickupPoint(nextX, nextY) && game.player->isTaxiDriver())
            {
                game.player->hitPerson();
            }
            else
            {
                game.player->move(dx, dy); //movement of car
                game.board.setcarposition(nextX, nextY); //updating car's location on the board as well
            }
        }
        else
        {
            game.player->hitObstacle();
        }
    }
    glutPostRedisplay();
}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y)
{
    if (key == 27 /* Escape key ASCII*/)
    {
        exit(1); // exit the program when escape key is pressed.
    }

    if (selectingRole) //while selecting role
    {
        if (key == 'T' || key == 't' || key == 'D' || key == 'd')
        {
            selectedRole = (key == 't' || key == 'T') ? 'T' : 'D';
            selectingRole = false;
            enteringName = true;
            bool isTaxiDriver = (key == 'T' || key == 't');
            game.initializePlayer(isTaxiDriver);
        }
    }
    else if (enteringName) //while entering name
    {
        if (key == 13 || key == '\r')
        { // Enter key
            enteringName = false;
        }
        // Allowing only letters (a-z, A-Z) and digits (0-9) and space (ASCII 32)
        else if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') || (key >= '0' && key <= '9') || key == 32)
        {
            playerName += key; //adding characters to the name entered by user
        }
    }

    else
    {
        if (key == 'f' || key == 'F') //for refueling
        {
            if (game.matchFuelStation())
            {
                game.refillfuel();
            }
        }
        else if (key == 32) //for pickup
        {
            game.pickup();
        }
        else if (key == 'd' || key == 'D') //for drop off
        {
            game.dropoff();
        }
        else if (key == 'p' || key == 'P') //for role change
        {
            if (game.matchStation())
            {
                game.switchRole();
            }
        }
    }

    // if (key == 'b' || key == 'B') // Key for placing the bomb
    // {
    //     // do something if b is pressed
    //     cout << "b pressed" << endl;
    // }
    // else if (key == 32)
    // {
    //     if (0)
    //         ;
    // }

    glutPostRedisplay();
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m)
{
    if (!selectingRole && !enteringName)
    {
        Time_Left -= 0.1;
        game.board.update();
    }
    glutPostRedisplay();
    glutTimerFunc(100, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y)
{
    cout << x << " " << y << endl;
    glutPostRedisplay();
}
void MouseMoved(int x, int y)
{
    // cout << x << " " << y << endl;
    glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y)
{

    if (button == GLUT_LEFT_BUTTON) // dealing only with left button
    {
        cout << GLUT_DOWN << " " << GLUT_UP << endl;
    }
    else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
    {
        cout << "Right Button Pressed" << endl;
    }
    glutPostRedisplay();
}
/*
 * our gateway main function
 * */
int main(int argc, char *argv[])
{
    int width = 750, height = 870;                // i have set my window size to be 800 x 600
    InitRandomizer();                             // seed the random number generator...
    glutInit(&argc, argv);                        // initialize the graphics library...
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
    glutInitWindowPosition(50, 50);               // set the initial position of our window
    glutInitWindowSize(width, height);            // set the size of our window
    glutCreateWindow("OOP Project");              // set the title of our game window
    SetCanvasSize(width, height);                 // set the number of pixels...

    //for adding music
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music *bgMusic = Mix_LoadMUS("music.mp3");
    Mix_PlayMusic(bgMusic, -1);

    // Register your functions to the library,
    // you are telling the library names of function to call for different tasks.
    // glutDisplayFunc(display); // tell library which function to call for drawing Canvas.
    glutDisplayFunc(GameDisplay);      // tell library which function to call for drawing Canvas.
    glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
    glutKeyboardFunc(PrintableKeys);   // tell library which function to call for printable ASCII characters
    // This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
    glutTimerFunc(1000.0, Timer, 0);
    glutMouseFunc(MouseClicked);
    glutPassiveMotionFunc(MouseMoved);    // Mouse
    glutMotionFunc(MousePressedAndMoved); // Mouse
    // now handle the control to library and it will call our registered functions when
    // it deems necessary...
    glutMainLoop();
    return 1;
}
#endif /* RushHour_CPP_ */
