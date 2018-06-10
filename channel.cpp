#include "channel.h"
#include <QVector2D>
#include <QPainter>
#include <iostream>

void Channel::render(QPainter &painter, const QVector2D &offset){
    QVector2D absolutePos = m_pos + offset;
    QConicalGradient conicalGradient( absolutePos.toPointF(), 0);
    conicalGradient.setColorAt( 0.0, Qt::red );
    conicalGradient.setColorAt( 60.0/360.0, Qt::yellow );
    conicalGradient.setColorAt( 120.0/360.0, Qt::green );
    conicalGradient.setColorAt( 180.0/360.0, Qt::cyan );
    conicalGradient.setColorAt( 240.0/360.0, Qt::blue );
    conicalGradient.setColorAt( 300.0/360.0, Qt::magenta );
    conicalGradient.setColorAt( 1.0, Qt::red);
    QBrush brush( conicalGradient );
    painter.setBrush( brush );
    painter.drawEllipse(absolutePos.toPointF() , this->m_radius, this->m_radius);
}
