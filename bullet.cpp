#include "bullet.h"

// Default constructor.
Bullet::Bullet()
{
    // draw the rect

    // size and position
    setRect((10*qCos(angle*(M_PI/180))),(12.55*qSin(angle*(M_PI/180))),5,1);
    setPen(QPen(Qt::green, 1));

    QTransform itTransf = transform();
    QPointF dp = this->boundingRect().center();
    itTransf.translate( dp.x(), dp.y() );
    itTransf.rotate( angle, Qt::ZAxis );
    itTransf *= QTransform::fromScale( scale(), scale() );
    itTransf.translate( -dp.x(), -dp.y() );
    setTransform(itTransf);

    // updates at 60FPS
    timer.start(16.67, this);
}


// Constructs with an angle and speed to enable realistic bullet firing.
Bullet::Bullet(float angle, float speedX, float speedY)
{
    // spawn bullet in middle of ship, then draw
    setRect((qCos(angle*(M_PI/180))+30), ((qSin(angle*(M_PI/180)))+20),7,1);
    setPen(QPen(Qt::green, 1));

    // rotates bullet relative to ship's position
    this->angle = angle;
    QTransform itTransf = transform();
    QPointF dp = this->boundingRect().center();
    itTransf.translate( dp.x(), dp.y() );
    itTransf.rotate( angle, Qt::ZAxis );
    itTransf *= QTransform::fromScale( scale(), scale() );
    itTransf.translate( -dp.x(), -dp.y() );
    setTransform(itTransf);

    // set initial speeds
    this->initSpeedX = speedX;
    this->initSpeedY = speedY;
    // put bullet under all objects
    setZValue(-1);

    // start the timer. 16.67ms = 60FPS
    timer.start(16.67, this);
    // start the despawn timer (0.7 seconds).
    timeout.start(700, this);
}


// Fires whenever the timers fire.
void Bullet::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId())
    {
        update();
    }
    if (event->timerId() == timeout.timerId())
    {
        despawn();
    }
}


// Handles what the bullet every frame.
void Bullet::update()
{
    // move bullet, adding ship velocity
    setPos( x() + initSpeedX/2 + (20*qCos(angle*(M_PI/180) )) ,
            y() + initSpeedY/2 + (20*qSin(angle*(M_PI/180) )) );

    // screen looping
    if(x() > 960)
        setPos( x() - 960, y());
    else if(x() < 0)
        setPos( x() + 960, y());

    if(y() > 720)
        setPos( x(),  y() - 720);
    else if(y() < 0)
        setPos( x(), y() + 720);

    // collision detection
    QList<QGraphicsItem *> colliding_items = collidingItems();
    for (int i=0, n=colliding_items.size(); i<n; ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(Asteroid))
        {
            // remove bullet from scene
            this->scene()->removeItem(this);
            // reduce asteroid health
            //static_cast<Asteroid *>(colliding_items[i])->setHealth(
                        //static_cast<Asteroid *>(colliding_items[i])->getHealth() - 2);
            static_cast<Asteroid *>(colliding_items[i])->setHealth(0);
            // delete bullet
            delete this;
        }
    }

}


// Despawns the bullet when its time has come.
void Bullet::despawn()
{
    delete this;
}
