#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenuBar>
#include <QSlider>
#include <QWidgetAction>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Image Channels");
    resize(800, 600);

    // Создаем меню бар
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    menuBar->setStyleSheet("QMenuBar { background-color: #f5f5f5; color: #963D5A; font-size: 14px; }");

    // Создаем меню "File"
    QMenu *fileMenu = menuBar->addMenu("File");
    fileMenu->setStyleSheet("QMenu { background-color: #f5f5f5; color: #963D5A; font-size: 14px; }");

    // Создаем действия для меню "File"
    QAction *loadAction = new QAction("Load Image", this);
    fileMenu->addAction(loadAction);
    QAction *exitAction = new QAction("Exit", this);
    fileMenu->addAction(exitAction);

    // Создаем слайдер для яркости
    brightnessSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setRange(-100, 100);
    brightnessSlider->setValue(0);
    brightnessSlider->setStyleSheet("QSlider::handle { background-color: #963D5A; }");
     brightnessSlider->setTickInterval(1);

    // Создаем действие с виджетом слайдера яркости
        QWidgetAction *sliderAction = new QWidgetAction(this);
        sliderAction->setDefaultWidget(brightnessSlider);

        // Добавляем действие в меню
        fileMenu->addAction(sliderAction);

        // Подключаем сигнал слайдера к слоту для изменения яркости
        connect(brightnessSlider, &QSlider::valueChanged, this, &MainWindow::adjustBrightness);

    // Создаем меню "Channels"
    QMenu *channelsMenu = menuBar->addMenu("Channels");
    channelsMenu->setStyleSheet("QMenu { background-color: #f5f5f5; color: #963D5A; font-size: 14px; }");

    // Создаем действия для меню "Channels"
    QAction *showOriginalAction = new QAction("Show Original", this);
    channelsMenu->addAction(showOriginalAction);
    QAction *showRChannelAction = new QAction("Show R Channel", this);
    channelsMenu->addAction(showRChannelAction);
    QAction *showGChannelAction = new QAction("Show G Channel", this);
    channelsMenu->addAction(showGChannelAction);
    QAction *showBChannelAction = new QAction("Show B Channel", this);
    channelsMenu->addAction(showBChannelAction);
    QAction *saveRChannelAction = new QAction("Save R Channel", this);
    channelsMenu->addAction(saveRChannelAction);
    QAction *saveGChannelAction = new QAction("Save G Channel", this);
    channelsMenu->addAction(saveGChannelAction);
    QAction *saveBChannelAction = new QAction("Save B Channel", this);
    channelsMenu->addAction(saveBChannelAction);

    // Создаем метку для отображения изображения
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
     imageLabel->setStyleSheet("QLabel { font-size: 18px; }");

    // Создаем вертикальный макет для размещения метки изображения
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(imageLabel);

    // Создаем виджет и устанавливаем главный макет
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);

    // Устанавливаем центральный виджет
    setCentralWidget(centralWidget);
    setStyleSheet("QMainWindow { background-color: white; }");

    // Подключаем сигналы к слотам
    connect(loadAction, &QAction::triggered, this, &MainWindow::openImage);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(showOriginalAction, &QAction::triggered, this, &MainWindow::showOriginal);
    connect(showRChannelAction, &QAction::triggered, this, &MainWindow::showRChannel);
    connect(showGChannelAction, &QAction::triggered, this, &MainWindow::showGChannel);
    connect(showBChannelAction, &QAction::triggered, this, &MainWindow::showBChannel);
    connect(saveRChannelAction, &QAction::triggered, this, &MainWindow::saveRChannel);
    connect(saveGChannelAction, &QAction::triggered, this, &MainWindow::saveGChannel);
    connect(saveBChannelAction, &QAction::triggered, this, &MainWindow::saveBChannel);
}


void MainWindow::adjustBrightness(int value)
{
    if (image->isNull()) {
        return;
    }

    qreal brightnessFactor = 1.0 + value / 100.0; // Фактор яркости

    // Создаем копию изображения для изменений
    QImage adjustedImage = image->copy();

    // Проход по пикселям изображения
    for (int y = 0; y < adjustedImage.height(); ++y) {
        for (int x = 0; x < adjustedImage.width(); ++x) {
            // Получение пикселя
            QRgb pixel = adjustedImage.pixel(x, y);

            // Изменение яркости компонентов пикселя
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);

            // Применяем фактор яркости к компонентам пикселя
            red = qBound(static_cast<int>(red * brightnessFactor), 0, 255);
            green = qBound(static_cast<int>(green * brightnessFactor), 0, 255);
            blue = qBound(static_cast<int>(blue * brightnessFactor), 0, 255);

            // Обновляем пиксель с измененными компонентами
            adjustedImage.setPixel(x, y, qRgb(red, green, blue));
        }
    }

    // Отображаем измененное изображение
    displayImage(adjustedImage);
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", QDir::homePath(), "Image Files (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty()) {
        image = new QImage(fileName);
        if (!image->isNull()) {
            displayImage(*image);
        }
    }
}

void MainWindow::showOriginal()
{
    if (image != nullptr) {
        displayImage(*image);
    }
}

void MainWindow::showRChannel()
{
    if (image != nullptr) {
        QImage redImage = image->copy();
        for (int i = 0; i < redImage.width(); i++) {
            for (int j = 0; j < redImage.height(); j++) {
                QRgb pixel = redImage.pixel(i, j);
                redImage.setPixel(i, j, qRgb(qRed(pixel), 0, 0));
            }
        }
        displayImage(redImage);
    }
}

void MainWindow::showGChannel()
{
    if (image != nullptr) {
        QImage greenImage = image->copy();
        for (int i = 0; i < greenImage.width(); i++) {
            for (int j = 0; j < greenImage.height(); j++) {
                QRgb pixel = greenImage.pixel(i, j);
                greenImage.setPixel(i, j, qRgb(0, qGreen(pixel), 0));
            }
        }
        displayImage(greenImage);
    }
}
void MainWindow::showBChannel()
{
    if (image != nullptr) {
        QImage blueImage = image->copy();
        for (int i = 0; i < blueImage.width(); i++) {
            for (int j = 0; j < blueImage.height(); j++) {
                QRgb pixel = blueImage.pixel(i, j);
                blueImage.setPixel(i, j, qRgb(0, 0, qBlue(pixel)));
            }
        }
        displayImage(blueImage);
    }
}

void MainWindow::saveRChannel()
{
    if (image != nullptr) {
        QImage redImage = image->copy();
        for (int i = 0; i < redImage.width(); i++) {
            for (int j = 0; j < redImage.height(); j++) {
                QRgb pixel = redImage.pixel(i, j);
                redImage.setPixel(i, j, qRgb(qRed(pixel), 0, 0));
            }
        }
        QString fileName = QFileDialog::getSaveFileName(this, "Save R Channel", QDir::homePath(), "Image Files (*.jpeg)");
        if (!fileName.isEmpty()) {
            redImage.save(fileName);
        }
    }
}

void MainWindow::saveGChannel()
{
    if (image != nullptr) {
        QImage greenImage = image->copy();
        for (int i = 0; i < greenImage.width(); i++) {
            for (int j = 0; j < greenImage.height(); j++) {
                QRgb pixel = greenImage.pixel(i, j);
                greenImage.setPixel(i, j, qRgb(0, qGreen(pixel), 0));
            }
        }
        QString fileName = QFileDialog::getSaveFileName(this, "Save G Channel", QDir::homePath(), "Image Files (*.jpeg)");
        if (!fileName.isEmpty()) {
            greenImage.save(fileName);
        }
    }
}

void MainWindow::saveBChannel()
{
    if (image != nullptr) {
        QImage blueImage = image->copy();
        for (int i = 0; i < blueImage.width(); i++) {
            for (int j = 0; j < blueImage.height(); j++) {
                QRgb pixel = blueImage.pixel(i, j);
                blueImage.setPixel(i, j, qRgb(0, 0, qBlue(pixel)));
            }
        }
        QString fileName = QFileDialog::getSaveFileName(this, "Save B Channel", QDir::homePath(), "Image Files (*.jpeg)");
        if (!fileName.isEmpty()) {
            blueImage.save(fileName);
        }
    }
}

void MainWindow::displayImage(const QImage& img)
{
    QPixmap pixmap(QPixmap::fromImage(img));
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}
