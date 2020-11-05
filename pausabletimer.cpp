#include "pausabletimer.h"

PausableTimer::PausableTimer(int maxSeconds, QObject *parent) :
    QObject(parent),
    timer(new QTimer{}),
    maxSeconds_(maxSeconds),
    tempSecondsElapsed_(0),
    countdownTime_(maxSeconds),
    secondsElapsed_(0)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
}

void PausableTimer::tick()
{
    secondsElapsed_  += 1;
    debugStatus("--tick--");
    if(secondsElapsed_ >= countdownTime_) {
        emit finish();
        timer->stop();
        return;
    }
    else {
        // this is absolute progress
        emit update(tempSecondsElapsed_ + secondsElapsed_, maxSeconds_);
    }
}

void PausableTimer::start()
{
    secondsElapsed_ = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    // if paused
    if(tempSecondsElapsed_ > 0) {
        countdownTime_ = maxSeconds_ - tempSecondsElapsed_;
        debugStatus("resumed end");
    }
    // if new
    else {
        countdownTime_ = maxSeconds_;
        debugStatus("started end");
    }
    timer->start(1000);
}

void PausableTimer::pause()
{
    debugStatus("paused start");
    // only pause and accumulate if timer still active
    if(isActive()) {
        timer->stop();
        tempSecondsElapsed_ += secondsElapsed_;
    }
    debugStatus("paused end");
}

void PausableTimer::stop()
{
    debugStatus("stopped start");
    timer->stop();
    tempSecondsElapsed_ = 0;
    debugStatus("stopped end");
}

bool PausableTimer::isActive()
{
    return timer->isActive();
}

void PausableTimer::debugStatus(QString event)
{
    qDebug() << event << "\t" << isActive()
             << " progress: " << tempSecondsElapsed_ << " of " << maxSeconds_
             << " countdown: " << secondsElapsed_ << " of " << countdownTime_;
}
