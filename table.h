#pragma once

#include <QColor>
#include <QPainter>

#include "pocket.h"
#include <tablestate.h>
#include <channel.h>

class Ball;

class Table {
protected:
    int m_width;
    int m_height;
    QBrush m_brush;
    double m_friction;
    std::vector<Channel*> m_channel;

public:
    bool isAdventure = false;
    virtual ~Table() {}
    Table(int width, int height, QColor colour, double friction) :
        m_width(width), m_height(height),
        m_brush(colour), m_friction(friction) {}
    /**
     * @brief render - draw the table to screen using the specified painter
     * @param painter - painter to use
     */
    virtual void render(QPainter& painter, const QVector2D& offset) = 0;
    virtual void enterNormalState(){}
    virtual void enterAdventureState(){}
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    double getFriction() const { return m_friction; }
    virtual std::vector<Channel*> getChannel(){return m_channel;}
    virtual bool sinks(Ball*) { return false; }
};

class StageOneTable : public Table
{
public:
    StageOneTable(int width, int height, QColor colour, double friction) :
        Table(width, height, colour, friction) {}
    /**
     * @brief render - draw the stageonetable to screen using the specified painter
     * @param painter - painter to use
     */
    void render(QPainter &painter, const QVector2D& offset) override;

//    std::vector<Channel*> getChannel() override{}
};

class StageTwoTable : public Table {
protected:
    std::vector<Pocket*> m_pockets;

public:
    StageTwoTable(int width, int height, QColor colour, double friction) :
        Table(width, height, colour, friction) {}

    ~StageTwoTable();

    /**
     * @brief render - draw the stageonetable to screen using the specified painter
     * @param painter - painter to use
     */
    void render(QPainter &painter, const QVector2D& offset) override;

    // sinky winky ball
    virtual bool sinks(Ball* b) override;

    /* self explanatory */
    void addPocket(Pocket* p) { m_pockets.push_back(p); }

//    std::vector<Channel*> getChannel() override{}
};

class StageThreeTable : public Table {
protected:
    std::vector<Pocket*> m_pockets;
    std::vector<Channel*> m_channel;
    TableState* m_tableState;
    NormalState* m_normalState;
    AdventureState* m_adventureState;

public:
    /**
     * @brief StageThreeTable - stage three table which can change the mode and has new functions
     * @param width
     * @param height
     * @param colour
     * @param friction
     */
    StageThreeTable(int width, int height, QColor colour, double friction) :
        Table(width, height, colour, friction) {
        m_normalState = new NormalState(this);
        m_adventureState = new AdventureState(this);
        m_tableState = m_normalState;
    }

    ~StageThreeTable();

    /**
     * @brief render - draw the stageonetable to screen using the specified painter
     * @param painter - painter to use
     */
    void render(QPainter &painter, const QVector2D& offset) override;
    /**
     * @brief setChannel - offer a method the pass the parameter of channel for table state class
     */
    void setChannel(){ m_tableState->m_channel = m_channel;}
    // sinky winky ball
    virtual bool sinks(Ball* b) override;

    /* self explanatory */
    void addPocket(Pocket* p) { m_pockets.push_back(p); }
    void addChannel(Channel* p){ m_channel.push_back(p);}
    /**
     * @brief enterNormalState - change the mode
     */
    void enterNormalState() override{
        m_tableState = this->getNormalState();
        this->isAdventure = false;
    }
    /**
     * @brief enterAdventureState - change the mode
     */
    void enterAdventureState() override{
        m_tableState = this->getAdventureState();
        this->isAdventure = true;
    }

    NormalState* getNormalState(){
        return m_normalState;
    }
    AdventureState* getAdventureState(){
        return m_adventureState;
    }


    QBrush getBrush(){
        return m_brush;
    }

    std::vector<Channel*> getChannel() override{return m_channel;}
};
