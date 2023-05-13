#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QSlider>

class QLabel;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void openImage();
    void showOriginal();
    void showRChannel();
    void showGChannel();
    void showBChannel();
    void saveRChannel();
    void saveGChannel();
    void saveBChannel();
    void adjustBrightness(int value);

private:
    QLabel *imageLabel;
    QImage *image;
    QSlider *brightnessSlider;

    void setModernStyle();
    void setModernWindowStyle();
    void displayImage(const QImage& img);
    void saveChannel(const QImage& channel, const QString& fileName);
};

#endif // MAINWINDOW_H
