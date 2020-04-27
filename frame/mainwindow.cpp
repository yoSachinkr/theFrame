/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2020 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <elements/timelineelement.h>
#include <elements/rectangleelement.h>
#include <QFileDialog>
#include <QMessageBox>
#include <taboutdialog.h>
#include "prerenderer.h"

struct MainWindowPrivate {
    QTimer* playTimer;
    qint64 playStartTime;
    quint64 startFrame;

    QString currentFile;
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    d = new MainWindowPrivate();

    Prerenderer* prerenderer = new Prerenderer();
    ViewportElement* root = ui->viewport->rootElement();
    prerenderer->setViewportElement(root);
    prerenderer->setTimeline(ui->timeline);

    ui->viewport->setPrerenderer(prerenderer);

    ui->timeline->setPrerenderer(prerenderer);
    ui->timeline->setViewportElement(root);
    ui->propertiesWidget->setTimeline(ui->timeline);

    ui->viewport->setFrame(0);
    prerenderer->tryPrerenderAll();

    d->playTimer = new QTimer(this);
    connect(d->playTimer, &QTimer::timeout, this, &MainWindow::updatePlayFrame);
}

MainWindow::~MainWindow() {
    delete d;
    delete ui;
}


void MainWindow::on_timeline_currentFrameChanged(quint64 frame) {
    ui->viewport->setFrame(frame);
}

void MainWindow::on_actionNextFrame_triggered() {
    ui->timeline->setCurrentFrame(ui->timeline->currentFrame() + 1);
}

void MainWindow::on_actionPreviousFrame_triggered() {
    quint64 frame = ui->timeline->currentFrame();
    if (frame != 0) frame--;
    ui->timeline->setCurrentFrame(frame);
}

void MainWindow::on_actionDeleteTransition_triggered() {
    ui->timeline->deleteSelected();
}

void MainWindow::on_actionZoomIn_triggered() {
    double spacing = ui->timeline->frameSpacing();
    spacing *= 1.25;
    ui->timeline->setFrameSpacing(spacing);
}

void MainWindow::on_actionZoomOut_triggered() {
    double spacing = ui->timeline->frameSpacing();
    spacing *= 0.8;
    ui->timeline->setFrameSpacing(spacing);
}

void MainWindow::on_playButton_toggled(bool checked) {
    if (checked) {
        if (ui->timeline->currentFrame() == ui->timeline->frameCount() - 1) {
            d->startFrame = 0;
        } else {
            d->startFrame = ui->timeline->currentFrame();
        }
        d->playStartTime = QDateTime::currentMSecsSinceEpoch();
        d->playTimer->start();
    } else {
        d->playTimer->stop();
    }
}

void MainWindow::updatePlayFrame() {
    qint64 msecsAfterFrame = QDateTime::currentMSecsSinceEpoch() - d->playStartTime;
    qint64 msecsPerFrame = 1000 / ui->timeline->framerate();

    qint64 advancedFrames = msecsAfterFrame / msecsPerFrame;
    quint64 newFrame = d->startFrame + static_cast<quint64>(advancedFrames);

    if (newFrame > ui->timeline->frameCount()) {
        ui->timeline->setCurrentFrame(ui->timeline->frameCount() - 1);
        ui->playButton->setChecked(false);
    } else {
        ui->timeline->setCurrentFrame(newFrame);
    }
}

void MainWindow::on_actionPlay_triggered() {
    ui->playButton->toggle();
}

void MainWindow::on_actionFirstFrame_triggered() {
    ui->playButton->setChecked(false);
    ui->timeline->setCurrentFrame(0);
}

void MainWindow::on_actionLastFrame_triggered() {
    ui->playButton->setChecked(false);
    ui->timeline->setCurrentFrame(ui->timeline->frameCount() - 1);
}

void MainWindow::on_firstFrameButton_clicked() {
    ui->actionFirstFrame->trigger();
}

void MainWindow::on_lastFrameButton_clicked() {
    ui->actionLastFrame->trigger();
}

void MainWindow::on_actionSave_triggered() {
    if (d->currentFile.isEmpty()) {
        ui->actionSaveAs->trigger();
        return;
    }

    QSaveFile saveFile(d->currentFile);
    saveFile.open(QSaveFile::WriteOnly);

    QJsonDocument doc(ui->timeline->save());
    saveFile.write(doc.toJson(QJsonDocument::Compact));
    if (!saveFile.commit()) {
        //Error Error!
        QMessageBox::critical(this, tr("Error"), tr("Sorry, we couldn't save the file. Check that there is enough disk space and that you have permission to write to the file.\n\nDon't close the window until you've managed to save your changes, otherwise you may lose data."));
    }
}

void MainWindow::on_actionSaveAs_triggered() {
    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setNameFilters({tr("theFrame Project Files (*.tfrproj)")});
    fileDialog->setWindowFlag(Qt::Sheet);
    connect(fileDialog, &QFileDialog::finished, this, [ = ](int result) {
        if (result == QFileDialog::Accepted) {
            d->currentFile = fileDialog->selectedFiles().first();
            ui->actionSave->trigger();
        }
        fileDialog->deleteLater();
    });
    fileDialog->open();
}

void MainWindow::on_actionOpen_triggered() {
    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setNameFilters({tr("theFrame Project Files (*.tfrproj)")});
    fileDialog->setWindowFlag(Qt::Sheet);
    connect(fileDialog, &QFileDialog::finished, this, [ = ](int result) {
        if (result == QFileDialog::Accepted) {
            //Attempt to load this file
            QFile file(fileDialog->selectedFiles().first());
            file.open(QSaveFile::ReadOnly);

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);

            if (error.error != QJsonParseError::NoError || !doc.isObject()) {
                //Error Error!
                return;
            }

            if (!ui->timeline->load(doc.object())) {
                //Error Error!
                return;
            }

            d->currentFile = fileDialog->selectedFiles().first();
        }
        fileDialog->deleteLater();
    });
    fileDialog->open();
}

void MainWindow::on_actionAbout_triggered() {
    tAboutDialog dialog;
    dialog.exec();
}
