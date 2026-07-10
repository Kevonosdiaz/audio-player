#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <iostream>

MainWindow::MainWindow(MpdHandler* mpd_handler, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mpd_handler(mpd_handler)
{
    ui->setupUi(this);
    ui->VolumeSlider->setMinimum(0);
    ui->VolumeSlider->setMaximum(100);
    ui->PlaybackSlider->setMinimum(0);
    // Allow maximum to be set for each individual song's duration (sec)

    // Setup media playback buttons
    connect(ui->PlaybackButton,
            &QPushButton::clicked,
            mpd_handler,
            &MpdHandler::handle_toggle_playback);
    connect(ui->NextButton, &QPushButton::clicked, mpd_handler, &MpdHandler::handle_next_song);
    connect(ui->PrevButton, &QPushButton::clicked, mpd_handler, &MpdHandler::handle_prev_song);
    connect(ui->VolumeSlider, &QSlider::valueChanged, mpd_handler, &MpdHandler::handle_volume);
    connect(ui->PlaybackSlider, &QSlider::valueChanged, mpd_handler, &MpdHandler::handle_playback_seeking);
    connect(ui->RepeatButton, &QPushButton::clicked, mpd_handler, &MpdHandler::handle_repeat);
    connect(ui->RandomButton, &QPushButton::clicked, mpd_handler, &MpdHandler::handle_random);
    connect(ui->SingleButton, &QPushButton::clicked, mpd_handler, &MpdHandler::handle_single);
    connect(ui->ConsumeButton, &QPushButton::clicked, mpd_handler, &MpdHandler::handle_consume);

    // Connect album art display
    main_album_art_display = ui->MainAlbumArt;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_playback_toggled() { }

void MainWindow::handle_song_changed(int duration) {
    std::cout << "Just set a maximum of " << duration << " seconds for PlaybackSlider" << std::endl;
    ui->PlaybackSlider->setMaximum(duration);
}

void MainWindow::handle_main_art_changed(const QPixmap& art)
{
    if(art.isNull())
        return;
    main_album_art_display->set_pixmap(art);
}

void MainWindow::handle_volume_changed(int volume)
{
    ui->VolumeSlider->setValue(volume);
}

void MainWindow::handle_playback_seeking_changed(int seconds)
{
    ui->PlaybackSlider->setValue(seconds);
}

void MainWindow::handle_repeat_mode_changed(bool repeat_on) { }

void MainWindow::handle_random_mode_changed(bool random_on) { }

void MainWindow::handle_single_mode_changed(mpd_single_state state) { }

void MainWindow::handle_consume_mode_changed(mpd_consume_state state) { }
