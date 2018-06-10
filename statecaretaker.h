#pragma once

#include <memento.h>


class StateCareTaker
{
public:
    /**
     * @brief SetStateMemento - set the memento
     * @param pMemento
     */
    void SetStateMemento(Memento* pMemento){
        m_memento=pMemento;
    }
    /**
     * @brief GetStateMemento - return the memento
     * @return
     */
    Memento* GetStateMemento(){
        return m_memento;
    }
private:
   Memento* m_memento;

};
