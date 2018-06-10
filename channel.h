#pragma once

#include <qvector2d.h>
#include <qpainter.h>

class Channel
{
    double m_radius;
    QVector2D m_pos;

public:
    /**
     * @brief Channel - balls can go through the channel and show up at other side
     * @param radius
     * @param pos
     */
    Channel(double radius, QVector2D pos) : m_radius(radius), m_pos(pos) {}

    /**
     * @brief render - draw the Channel to the screen with the provided brush and offset
     * @param painter - the brush to paint with
     * @param offset - the offset from the window
     */
    void render(QPainter& painter, const QVector2D& offset);

    /// whether this Channel contains the circle defined by the arguments
    bool contains(const QVector2D& center, const double& radius) {
        return ((center-m_pos).length() < fabs(radius - m_radius));
    }
    /**
     * @brief getPositon - return the position of channel
     * @return
     */
    QVector2D getPositon(){
        return m_pos;
    }
    /**
     * @brief getRadius - return the radius of channel
     * @return
     */
    double getRadius(){
        return m_radius;
    }

};
