#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMediaPlayer>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new PausableTimer{0})
{
    // setup ui first, then load settings, otherwise no widgets to set
    ui->setupUi(this);

    // *no named arguments in connect statements
    connect(timer, SIGNAL(update(int, int)), this, SLOT(on_timerUpdate(int, int)));
    connect(timer, SIGNAL(finish()), this, SLOT(on_timerFinished()));
    settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    QFont ft;
    ft.setPointSize(20);
    ui->label->setFont(ft);
     ui->label->setText("");

     QPalette palette;
     palette.setBrush(QPalette::Background, QBrush(QColor(255,255,255)));//红色
     setPalette(palette);

     Qt::WindowFlags m_flags = windowFlags();

       setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
       setWindowFlags(Qt::FramelessWindowHint);
show();
}

MainWindow::~MainWindow()
{
    // save current settings before deleting the ui
    delete ui;
}





void MainWindow::on_startPushButton_clicked()
{
    int hours = ui->hoursSpinBox->value();
    int minutes = ui->minutesSpinBox->value();
    int seconds = ui->secondsSpinBox->value();

    int totalSeconds = timeInSeconds(hours, minutes, seconds);

    // time spinboxes must be non-zero, no negative input for this widget
    if (totalSeconds == 0) {

         ui->label->setText("Input non-zero time.");
        return;
    }

    if(timer->isActive()) {
         //ui->label->setText("--RESTARTED--");
        timer->stop();
    }
    timer->setMaxSeconds(totalSeconds);
    timer->start();

     ui->label->setText(secondsInTimeString(totalSeconds));
}

void MainWindow::on_stopPushButton_clicked()
{
    timer->stop();
     ui->label->setText("--STOPPED--");

}

void MainWindow::on_pausePushButton_clicked()
{
    timer->pause();
    //ui->label->setText("--PAUSED--");
}

void MainWindow::on_timerUpdate(int currentSeconds, int maxSeconds)
{

     QString time= QString(secondsInTimeString(maxSeconds - currentSeconds));
     ui->label->setText( time);
}

void MainWindow::on_timerFinished()
{
    timer->stop();




    // no cross-platform for now
    //QString os = osName();
    QString command( "powershell.exe" );


    QMediaPlayer* player = new QMediaPlayer();
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    player->setMedia(QUrl::fromLocalFile("./ding-sound-effect_2.mp3"));
    player->setVolume(50);
    player->play();
    ui->label->setText(" Time out!!");

    //  ui->label->setText("--DONE-- : Opening " + filepath + " ...");
    //QStringList params = QStringList() << "Start-Process" << "-FilePath" << filepath;

    //qDebug() << command << params;

    // these don't work
    //bool exists = QFileInfo(filepath).isDir() || QFileInfo(filepath).isFile();
    //bool exists = QFileInfo(QDir::toNativeSeparators(filepath)).isDir();
    //bool exists = QFileInfo::exists(filepath);
    //auto exists = QFileInfo(filepath);
    //qDebug() << "exists: " << exists;

    //QProcess::startDetached(command, params);
}

int MainWindow::timeInSeconds(int hours, int minutes, int seconds)
{
    return hours*3600 + minutes*60 + seconds;
}

QString MainWindow::secondsInTimeString(int totalSeconds)
{
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    QString time_str,str_hour,str_min,str_sec;
    if(hours==0)  str_hour=QString("00:");
    else if (hours<10) str_hour=QString("0%1:").arg(hours);
    else str_hour= QString("%1:").arg(hours);
    if(minutes==0)  str_min=QString("00:");
    else if (minutes<10) str_min=QString("0%1:").arg(minutes);
    else str_min= QString("%1:").arg(minutes);

    if(seconds==0)  str_sec=QString("00");
    else if (seconds<10) str_sec=QString("0%1").arg(seconds);
    else str_sec= QString("%1").arg(seconds);

    time_str=str_hour+str_min+str_sec;
    return time_str;
}

QString MainWindow::osName()
{
    #if defined(Q_OS_WIN)
    return QLatin1String("windows");
    #elif defined(Q_OS_MACOS)
    return QLatin1String("macos");
    #elif defined(Q_OS_LINUX)
    return QLatin1String("linux");
    #elif defined(Q_OS_UNIX)
    return QLatin1String("unix");
    #else
    return QLatin1String("unknown");
    #endif
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();       // 获得部件当前位置
    this->mousePos = event->globalPos(); // 获得鼠标位置
    this->dPos = mousePos - windowPos;   // 移动后部件所在的位置
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}


void MainWindow::on_exitPushButton_clicked()
{
    exit(0);
}
