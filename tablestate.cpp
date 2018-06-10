#include "tablestate.h"
#include <QVector2D>
#include <QPainter>
#include <table.h>
#include <iostream>


void NormalState::render(QPainter &painter, const QVector2D &offset){
    painter.setBrush(m_table->getBrush());
    painter.drawRect(offset.x(),offset.y(),m_table->getWidth(),m_table->getHeight());

}

void AdventureState::render(QPainter &painter, const QVector2D &offset){
    painter.setBrush(QColor(240,230,140));
    painter.drawRect(offset.x(),offset.y(),m_table->getWidth(),m_table->getHeight());
    painter.setRenderHint(QPainter::Antialiasing, true );
    QLinearGradient linearGradient( 0, 0, 1000, 500);
    linearGradient.setColorAt( 0.2, Qt::white );
    linearGradient.setColorAt( 0.4, Qt::red );
    linearGradient.setColorAt( 0.6, Qt::green );
    linearGradient.setColorAt( 0.8, Qt::blue );
    painter.setBrush( QBrush(linearGradient ));
    painter.fillRect(QRect(0,100,m_table->getWidth(),50),QBrush(linearGradient ));
    painter.fillRect(QRect(0,250,m_table->getWidth(),50),QBrush(linearGradient ));
    painter.fillRect(QRect(0,400,m_table->getWidth(),50),QBrush(linearGradient ));


    for(Channel* p : m_channel){
        p->render(painter,offset);
    }

//    const int r = 20;
//    QConicalGradient conicalGradient( 400, 50, 0);
//    conicalGradient.setColorAt( 0.0, Qt::red );
//    conicalGradient.setColorAt( 60.0/360.0, Qt::yellow );
//    conicalGradient.setColorAt( 120.0/360.0, Qt::green );
//    conicalGradient.setColorAt( 180.0/360.0, Qt::cyan );
//    conicalGradient.setColorAt( 240.0/360.0, Qt::blue );
//    conicalGradient.setColorAt( 300.0/360.0, Qt::magenta );
//    conicalGradient.setColorAt( 1.0, Qt::red);
//    QBrush brush( conicalGradient );
//    painter.setBrush( brush );
//    painter.drawEllipse( QPoint(400, 50), r, r);

//    QConicalGradient conicalGradient2( 100, 470, 0);
//    conicalGradient2.setColorAt( 0.0, Qt::red );
//    conicalGradient2.setColorAt( 60.0/360.0, Qt::yellow );
//    conicalGradient2.setColorAt( 120.0/360.0, Qt::green );
//    conicalGradient2.setColorAt( 180.0/360.0, Qt::cyan );
//    conicalGradient2.setColorAt( 240.0/360.0, Qt::blue );
//    conicalGradient2.setColorAt( 300.0/360.0, Qt::magenta );
//    conicalGradient2.setColorAt( 1.0, Qt::red);
//    QBrush brush2( conicalGradient2 );
//    painter.setBrush( brush2 );
//    painter.drawEllipse( QPoint(100, 470), r, r);

}
