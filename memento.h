#pragma once

#include <ball.h>
#include <vector>
#include <utils.h>

class Memento
{
public:

private:
    friend class Game;
    /**
     * @brief Memento - save the balls' state as vector
     * @param ball
     */
    Memento(std::vector<Ball*>* ball):m_ballState(ball){}
    virtual ~Memento(){}
    /**
     * @brief getState - return the ballstate which has been saved
     * @return
     */
    std::vector<Ball*>* getState() const{
        return m_ballState;
    }

    std::vector<Ball*>* m_ballState;
};

