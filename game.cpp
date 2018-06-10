#include "game.h"
#include "utils.h"

#include <QJsonArray>
#include <stdexcept>
#include <cmath>
#include <exception>
#include <iostream>

extern bool count;
extern bool isShot;
extern bool mode;
extern bool stage3;

Game::~Game() {
    // cleanup ya boi
    for (auto b : *m_balls) delete b;
    delete m_balls;
    delete m_table;
}

void Game::render(QPainter &painter) const {
    if(mode == true && stage3 == true){
        // table is rendered first, as its the lowest
        m_table->render(painter, m_screenshake);
        // then render all the balls
        for (Ball* b : *m_balls) b->render(painter, m_screenshake);

        // the method to show the scene near the cue ball
        QPainterPath path;
        path.addRect(m_screenshake.x(),m_screenshake.y(),1000,500);
        path.addEllipse(m_balls->front()->getPosition().toPointF(),350,350);
        painter.setBrush(QColor(0,0,0));
        painter.drawPath(path);
    }
    else{
        // table is rendered first, as its the lowest
        m_table->render(painter, m_screenshake);
        // then render all the balls
        for (Ball* b : *m_balls) b->render(painter, m_screenshake);
    }
}

void Game::animate(double dt) {
    // keep track of the removed balls (they're set to nullptr during loop)
    // clean up afterwards
    std::vector<Ball*> toBeRemoved;
    // add these balls to the list after we finish
    std::vector<Ball*> toBeAdded;
    // (test) collide the ball with each other ball exactly once
    // to achieve this, balls only check collisions with balls "after them"
    for (auto it = m_balls->begin(); it != m_balls->end(); ++it) {
        Ball* ballA = *it;
        if (ballA == nullptr) continue;

        // correct ball velocity if colliding with table
        QVector2D tableBallDeltaV = resolveCollision(m_table, ballA);
        // test and resolve breakages with balls bouncing off table
        if (ballA->applyBreak(tableBallDeltaV, toBeAdded)) {
            // mark this ball to be deleted
            toBeRemoved.push_back(ballA);
            incrementShake();
            // nullify this ball
            *it = nullptr;

            continue;
        }

        // check whether ball should be swallowed
        if (m_table->sinks(ballA)) {
            // defer swallowing until later (messes iterators otherwise)
            toBeRemoved.push_back(ballA);
            // nullify this ball
            *it = nullptr;
            continue;
        }

        // check collision with all later balls
        for (auto nestedIt = it + 1; nestedIt != m_balls->end(); ++nestedIt) {
            Ball* ballB = *nestedIt;
            if (ballB == nullptr) continue;
            if (isColliding(ballA, ballB)) {
                // retrieve the changes in velocities for each ball and resolve collision
                QVector2D ballADeltaV,ballBDeltaV;
                std::tie(ballADeltaV, ballBDeltaV) = resolveCollision(ballA, ballB);

                // add screenshake, remove ball, and add children to table vector if breaking
                if (ballA->applyBreak(ballADeltaV, toBeAdded)) {
                    toBeRemoved.push_back(ballA);
                    incrementShake();
                    // nullify this ball
                    *it = nullptr;
                    break;
                }
                // add screenshake, remove ball, and add children to table vector if breaking
                if (ballB->applyBreak(ballBDeltaV, toBeAdded)) {
                    toBeRemoved.push_back(ballB);
                    incrementShake();
                    // nullify this ball
                    *nestedIt = nullptr;
                    continue;
                }
            }
        }
        // we marked this ball as deleted, so skip
        if (*it == nullptr) continue;

        // move ball due to speed
        ballA->translate(ballA->getVelocity() * dt);
        // apply friction
        if(ballA->getPosition().y()>100 && ballA->getPosition().y()<150){
            pos = 1;
        }
        else if(ballA->getPosition().y()>250 && ballA->getPosition().y()<300){
            pos = 2;
        }
        else if(ballA->getPosition().y()>400 && ballA->getPosition().y()<450){
            pos = 3;
        }
        else {
            pos = 0;
        }

        // provide different accelerated speed for balls at colorful band
        if(mode == true && stage3 == true){
            this->getIntoChannel(ballA,m_table->getChannel());
            switch (pos) {
            case 1:{
                QVector2D vel;
                vel.setX(0);
                vel.setY(5);
                ballA->changeVelocity(-ballA->getVelocity() * m_table->getFriction() * dt+vel);
            }
            case 2:{
                QVector2D vel;
                vel.setX(5);
                vel.setY(-5);
                ballA->changeVelocity(-ballA->getVelocity() * m_table->getFriction() * dt+vel);
            }
            case 3:{
                QVector2D vel;
                vel.setX(5);
                vel.setY(-5);
                ballA->changeVelocity(-ballA->getVelocity() * m_table->getFriction() * dt+vel);
            }
            case 0:ballA->changeVelocity(-ballA->getVelocity() * m_table->getFriction() * dt);
            }

        }
        else{
            ballA->changeVelocity(-ballA->getVelocity() * m_table->getFriction() * dt);
        }

    }

    if(isShot == true || originalState == true){
        delete CareTaker;
        CareTaker = new StateCareTaker();
        //save the ball state
        CareTaker->SetStateMemento(this->SaveState());
        std::cout<<"Now, saving the game"<<std::endl;
        originalState=false;
    }

    // clean up them trash-balls
    for (Ball* b : toBeRemoved) {
        delete b;
        // delete all balls marked with nullptr
        m_balls->erase(std::find(m_balls->begin(), m_balls->end(), nullptr));
    }
    for (Ball* b: toBeAdded) m_balls->push_back(b);

    updateShake(dt);

    // if press 'r' the game will reset to previous state
    if(count == true){
        RestoreToMemento(CareTaker->GetStateMemento());
    }
    count = false;
}

void Game::updateShake(double dt) {
    // <3 code lovingly taken from here: <3
    // https://gamedev.stackexchange.com/a/47565
    // slightly modified..

    // update the screen shake per time step
    m_shakeRadius *= (1-dt)*0.9;
    m_shakeAngle += (150 + rand()%60);
    m_screenshake = QVector2D(sin(m_shakeAngle)*m_shakeRadius, cos(m_shakeAngle)*m_shakeRadius);
}

QVector2D Game::resolveCollision(const Table* table, Ball* ball) {
    QVector2D bPos = ball->getPosition();

    QVector2D startingVel = ball->getVelocity();

    // resulting multiplicity of direction. If a component is set to -1, it
    // will flip the velocity's corresponding component
    QVector2D vChange(1,1);

    // ball is beyond left side of table's bounds
    if (bPos.x() - ball->getRadius() <= 0) {
        // flip velocity if wrong dir
        if (ball->getVelocity().x() <= 0) vChange.setX(-1);
    // ball is beyond right side of table's bounds
    } else if (bPos.x() + ball->getRadius() >= 0 + table->getWidth()) {
        // flip velocity if wrong dir
        if (ball->getVelocity().x() >= 0) vChange.setX(-1);
    }
    // ball is above top of the table's bounds
    if (bPos.y() - ball->getRadius() <= 0) {
        // flip iff we're travelling in the wrong dir
        if (ball->getVelocity().y() <= 0) vChange.setY(-1);
    // ball is beyond bottom of table's bounds
    } else if (bPos.y() + ball->getRadius() >= 0 + table->getHeight()) {
        // if we're moving down (we want to let the ball bounce up if its heading back)
        if (ball->getVelocity().y() >= 0) vChange.setY(-1);
    }

    ball->multiplyVelocity(vChange);

    // return the change in velocity
    return ball->getVelocity() - startingVel;
}

std::pair<QVector2D, QVector2D> Game::resolveCollision(Ball* ballA, Ball* ballB) {
    // SOURCE : ASSIGNMENT SPEC

    // if not colliding (distance is larger than radii)
    QVector2D collisionVector = ballB->getPosition() - ballA->getPosition();
    if (collisionVector.length() > ballA->getRadius() + ballB->getRadius()) {
       throw std::logic_error("attempting to resolve collision of balls that do not touch");
    }
    collisionVector.normalize();

    QVector2D ballAStartingVelocity = ballA->getVelocity();
    QVector2D ballBStartingVelocity = ballB->getVelocity();

    float mr = ballB->getMass() / ballA->getMass();
    double pa = QVector2D::dotProduct(collisionVector, ballA->getVelocity());
    double pb = QVector2D::dotProduct(collisionVector, ballB->getVelocity());

    if (pa <= 0 && pb >= 0) return std::make_pair(QVector2D(0,0), QVector2D(0,0));

    double a = -(mr + 1);
    double b = 2*(mr * pb + pa);
    double c = -((mr - 1)*pb*pb + 2*pa*pb);
    double disc = sqrt(b*b - 4*a*c);
    double root = (-b + disc)/(2*a);
    if (root - pb < 0.01) {
        root = (-b - disc)/(2*a);
    }

    ballA->changeVelocity(mr * (pb - root) * collisionVector);
    ballB->changeVelocity((root-pb) * collisionVector);

    // return the change in velocities for the two balls
    return std::make_pair(ballA->getVelocity() - ballAStartingVelocity, ballB->getVelocity() - ballBStartingVelocity);
}

Memento* Game::SaveState(){
    std::vector<Ball*>* balls = new std::vector<Ball*>();
    // deep copy each ball and save them in memento
    for(auto it = m_balls->begin(); it!=m_balls->end();++it){
        Ball* ballA = *it;
        balls->push_back(ballA->clone());
    }
    return new Memento(balls);
}

void Game::RestoreToMemento(Memento *pMemento){
    // reset the game state
    m_balls = pMemento->m_ballState;
    Ball* ball = this->m_balls->front();
    CueBall* cb = new CueBall(ball);
    this->m_balls->front() = static_cast<Ball*>(cb);
    this->addMouseFunctions(cb->getEvents());
}

void Game::getIntoChannel(Ball* ball, std::vector<Channel*> channel){
    //methods to determine whether the balls are getting into the channel
     if((ball->getPosition()-channel.front()->getPositon()).length()<
                channel.front()->getRadius()){
         ball->setPosition(channel.back()->getPositon()+QVector2D(30,-20));
        }
     else if((ball->getPosition()-channel.back()->getPositon()).length()<
                channel.back()->getRadius()){
         ball->setPosition(channel.front()->getPositon()+QVector2D(-30,20));
        }
}
