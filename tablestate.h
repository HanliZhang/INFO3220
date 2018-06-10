#pragma once

#include <iostream>
#include <QVector2D>
#include <QPainter>
#include <channel.h>

class StageThreeTable;

class TableState
{
public:
    /**
     * @brief TableState - abstract class to offer interfaces
     * @param table
     */
    TableState(StageThreeTable* table): m_table(table){}
    virtual ~TableState(){}
    virtual void render(QPainter &painter, const QVector2D& offset) = 0;
protected:
    friend class StageThreeTable;
    StageThreeTable* m_table;
    std::vector<Channel*> m_channel;
};

class NormalState : public TableState
{
public:
    /**
     * @brief NormalState - normal state which is the same as stage 2
     * @param table
     */
    NormalState(StageThreeTable* table): TableState(table) {}
    /**
     * @brief render - draw the normal state to screen using the specified painter
     * @param painter
     * @param offset
     */
    void render(QPainter &painter, const QVector2D& offset) override;
};


class AdventureState : public TableState
{
public:
    /**
     * @brief AdventureState - new mode which has colorful scene and different way to play
     * @param table
     */
    AdventureState(StageThreeTable* table):TableState(table) {}
    /**
      * @brief render - draw the adventure mode to screen using the specified painter
      * @param painter
      * @param offset
      */
     void render(QPainter &painter, const QVector2D& offset) override;
};
