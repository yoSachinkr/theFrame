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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tpromise.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

struct MainWindowPrivate;
class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private slots:

        void on_timeline_currentFrameChanged(quint64 frame);

        void on_actionNextFrame_triggered();

        void on_actionPreviousFrame_triggered();

        void on_actionDeleteTransition_triggered();

        void on_actionZoomIn_triggered();

        void on_actionZoomOut_triggered();

        void on_playButton_toggled(bool checked);

        void on_actionPlay_triggered();

        void on_actionFirstFrame_triggered();

        void on_actionLastFrame_triggered();

        void on_firstFrameButton_clicked();

        void on_lastFrameButton_clicked();

        void on_actionSave_triggered();

        void on_actionSaveAs_triggered();

        void on_actionOpen_triggered();

        void on_actionAbout_triggered();

        void on_actionSet_In_Point_triggered();

        void on_actionSet_Out_Point_triggered();

        void on_actionClear_In_Out_Points_triggered();

        void on_actionEnable_Prerendering_toggled(bool arg1);

    private:
        Ui::MainWindow* ui;
        MainWindowPrivate* d;

        void closeEvent(QCloseEvent* event);

        tPromise<void>* save();
        tPromise<void>* saveAs();
        tPromise<void>* ensureDiscardChanges();

        void updatePlayFrame();
};
#endif // MAINWINDOW_H
