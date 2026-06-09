#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(MpdHandler* mpd_handler, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mpd_handler(mpd_handler)
{
    ui->setupUi(this);
    // Setup media playback buttons
    connect(ui->PlaybackButton,
            &QPushButton::clicked,
            mpd_handler,
            &MpdHandler::handle_toggle_playback);
    connect(ui->NextButton, &QPushButton::clicked, mpd_handler, &MpdHandler::handle_next_song);
    connect(ui->PrevButton, &QPushButton::clicked, mpd_handler, &MpdHandler::handle_prev_song);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playback_toggled() { }

void MainWindow::song_changed() { }