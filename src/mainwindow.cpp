#include "mainwindow.h"
#include "songinfo.h"
#include "ui_mainwindow.h"
#include <QLabel>

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

    // Connect album art display
    main_album_art_display = ui->MainAlbumArt;

    // Initialize current MPD song info if existing song, queue, etc.?
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_playback_toggled() { }

void MainWindow::handle_song_changed(const SongInfo& song_info) { }

void MainWindow::handle_main_art_changed(const QPixmap& art)
{
    if(art.isNull())
        return main_album_art_display->setPixmap(art);
}