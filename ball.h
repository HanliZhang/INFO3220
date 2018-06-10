#pragma once

#include <QPoint>
#include <cmath>
#include <QPainter>
#include <QVector2D>

class Ball {
protected:
    QBrush m_brush;
    QVector2D m_pos;
    QVector2D m_velocity;
    double m_mass;
    int m_radius;

    // if movement is slower than this, then we're considered at a stand-still
    static constexpr double MovementEpsilon = 1;
public:
    virtual ~Ball() {}
    /**
     * @brief Ball - abstract ball class
     * @param colour
     * @param position
     * @param velocity
     * @param mass
     * @param radius
     */
    Ball(QColor colour, QVector2D position,
         QVector2D velocity, double mass, int radius) :
        m_brush(colour), m_pos(position), m_velocity(velocity),
        m_mass(mass), m_radius(radius) {}
    Ball() {}

    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    virtual void render(QPainter &painter, const QVector2D& offset) = 0;
    /**
     * @brief translate - Move the ball's position by provided vector
     * @param vec - vector
     */
    virtual void translate(QVector2D vec) { m_pos += vec; }

    virtual QVector2D getVelocity() const { return m_velocity; }
    virtual void setVelocity(QVector2D v) { m_velocity = v; }
    /**
     * @brief changeVelocity - modify speed by a constant amount
     * @param delta - change in velocity (x,y)
     */
    virtual void changeVelocity(const QVector2D& delta) { m_velocity += delta; }
    /**
     * @brief multiplyVelocity - apply vector multiplicatively
     * @param vel - vector
     */
    virtual void multiplyVelocity(const QVector2D& vel) { m_velocity *= vel; }

    virtual double getMass() const { return m_mass; }
    virtual QColor getColor() const { return m_brush.color();}
    virtual double getRadius() const { return m_radius; }
    virtual QVector2D getPosition() const { return m_pos; }
    virtual void setPosition(QVector2D p) { m_pos = p; }

    // whether the ball will break, and handle accordingly
    // for base ball, do nothing. insert into rhs if necessary
    virtual bool applyBreak(const QVector2D&, std::vector<Ball*>&) { return false; }

    /**
     * @brief clone - offer the interface of deep copy for balls
     * @return
     */
    virtual Ball* clone() = 0;

    virtual void addChild(Ball* ){}
};

class StageOneBall : public Ball {
public:
    StageOneBall(QColor colour, QVector2D position,
                 QVector2D velocity, double mass, int radius) :
        Ball(colour, position, velocity, mass, radius) {}
    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    void render(QPainter &painter, const QVector2D& offset) override;
    StageOneBall* clone() override {
        return new StageOneBall(*this);
    }
};

class CompositeBall : public Ball {
protected:
    std::vector<Ball*> m_children;
    bool m_renderChildren = true;
    void recursiveRender(QPainter& painter, const QVector2D& offset);
    // default is unbreakable (i.e. inf str)
    double m_strength = std::numeric_limits<double>::max();
public:
    CompositeBall(QColor colour, QVector2D position,
                 QVector2D velocity, double mass, int radius, double strength) :
        Ball(colour, position, velocity, mass, radius), m_strength(strength) { }

    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    void render(QPainter &painter, const QVector2D& offset) override;
    double getStrength(){return m_strength;}
    /* add a child ball to this composite ball */
    void addChild(Ball* b) { m_children.push_back(b); }
    std::vector<Ball*> getChild() {return this->m_children;}
    /**
     * @brief applyBreak - check and resolve breaking balls
     * @param deltaV - the change in velocity
     * @param parentlist - the list of balls that we'll need to add to if we break anything
     * @return whether the ball broke or not
     */
    virtual bool applyBreak(const QVector2D& deltaV, std::vector<Ball*>& parentlist) override;
    /**
     * @brief clone - method to deep copy the composite ball and the inner balls
     * @return
     */
    Ball* clone() override{
        CompositeBall* ball = new CompositeBall(this->getColor(),this->getPosition(),this->getVelocity(),this->getMass(),this->getRadius(),this->getStrength());
        if(!this->m_children.empty()){
            for(auto kid : this->getChild()){
                ball->addChild(kid->clone());
            }
        }
        return ball;
    }
};
