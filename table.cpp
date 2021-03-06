#include "table.h"
#include "ball.h"
#include <iostream>
extern bool mode;

void StageOneTable::render(QPainter &painter, const QVector2D& offset) {
    // our table colour
    painter.setBrush(m_brush);
    // draw table
    painter.drawRect(offset.x(), offset.y(), this->getWidth(), this->getHeight());
}

void StageTwoTable::render(QPainter &painter, const QVector2D& offset) {
    // our table colour
    painter.setBrush(m_brush);
    // draw table
    painter.drawRect(offset.x(), offset.y(), this->getWidth(), this->getHeight());

    // render the pockets relative to this table
    for (Pocket* p : m_pockets) {
        p->render(painter, offset);
    }
}

StageTwoTable::~StageTwoTable() {
    for (Pocket* p : m_pockets) delete p;
}

bool StageTwoTable::sinks(Ball *b) {
    QVector2D absPos = b->getPosition();
    double radius = b->getRadius();
    // check whether any pockets consumes this ball

    for (Pocket* p : m_pockets) {
        // you sunk my scrabbleship
        if (p->contains(absPos, radius)) {
            p->incrementSunk();
            return true;
        }
    }
    return false;
}

void StageThreeTable::render(QPainter &painter, const QVector2D& offset) {
    this->setChannel();
    if(mode == true){
        this->enterAdventureState();

    }
    else {
        this->enterNormalState();
    }
    m_tableState->render(painter,offset);

    for (Pocket* p : m_pockets) {
        p->render(painter, offset);
    }
//    for(Channel* p : m_channel){
//        p->render(painter,offset);
//    }
}

StageThreeTable::~StageThreeTable() {
    for (Pocket* p : m_pockets) delete p;
}

bool StageThreeTable::sinks(Ball *b) {
    QVector2D absPos = b->getPosition();
    double radius = b->getRadius();
    // check whether any pockets consumes this ball
    for (Pocket* p : m_pockets) {
        // you sunk my scrabbleship
        if (p->contains(absPos, radius)) {
            p->incrementSunk();
            return true;
        }
    }
    return false;
}
