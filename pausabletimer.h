#ifndef PAUSABLETIMER_H
#define PAUSABLETIMER_H

#include <QTimer>
#include <QDebug>

class PausableTimer : public QObject
{
    Q_OBJECT
public:
    explicit PausableTimer(int maxSeconds, QObject *parent = nullptr);

    void start();
    void pause();
    void stop();

    bool isActive();

    int maxSeconds() const { return maxSeconds_; }
    void setMaxSeconds(int newMaxSeconds) { maxSeconds_ = newMaxSeconds; }

signals:
    void update(int, int);
    void finish();

private slots:
    //TODO: PRIVATE SLOTS?
    void tick();

private:
    QTimer* timer;

    void debugStatus(QString event);

    // max seconds is the absolute maximum, can only be changed by external input
    // total accumulated seconds, used to resume absolute progress
    int maxSeconds_;
    int tempSecondsElapsed_;

    // total seconds and seconds elapsed in current countdown
    int countdownTime_;
    int secondsElapsed_;
};

#endif // PAUSABLETIMER_H
