#include "cli.hpp"

CLI::CLI(QObject* parent) : QObject(parent), m_screenshot(new Screenshot(this)), m_timer(new QTimer(this))
{
    m_screenshot->setScreen(constants::DEFAULT_SCREEN);
    m_timer->setInterval(1000 / constants::DEFAULT_FPS); // 25 fps
    m_timer->setSingleShot(false);

    connect(m_timer, &QTimer::timeout, this, &CLI::takeScreenshot);
}

void CLI::quit()
{
    delete m_screenshot;
    m_screenshot = nullptr;
    emit finished();
}

void CLI::run()
{
    qInfo() << "CLI::run";

    // remove the screenshots dir if it does not exist
    Q_ASSERT(util::createDir(m_dir_screenshots));

    m_timer->start();
    m_elapsed.start();
}

void CLI::takeScreenshot()
{
    m_screenshot->take();
    auto value
        = m_screenshot->getImage().save(QString("%1/%2.jpg").arg(m_dir_screenshots).arg(m_counter, 5, 10, QChar('0')));
    Q_ASSERT(value);
    m_counter++;

    // after run, convert to video
    // ffmpeg -i "%05d.jpg" -c:v libx264 -r 25 -pix_fmt yuv420p out.mp4
    if (m_counter == 100)
    {
        qInfo() << "Elapsed time (seconds): " << m_elapsed.elapsed() / 1000.0;
        m_timer->stop();
        //        qDebug() <<"ScreenShotSize:"<< m_screenshot->getImage().size();
        quit();
    }
}
