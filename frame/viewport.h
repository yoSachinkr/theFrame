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
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>
#include <tpromise.h>
#include <elements/viewportelement.h>

namespace Ui {
    class Viewport;
}

struct ViewportPrivate;
class Prerenderer;
class Timeline;
class Viewport : public QWidget {
        Q_OBJECT

    public:
        explicit Viewport(QWidget* parent = nullptr);
        ~Viewport();

        void setFrame(quint64 frame);
        void setPrerenderer(Prerenderer* prerenderer);

        void setTimeline(Timeline* timeline);
        Timeline* timeline();

        ViewportElement* rootElement();

        QRect viewportRect();
        double viewportScale();

        QRect viewportToCanvas(QRect viewportCoordinates);
        QRect canvasToViewport(QRect canvasCoordinates);
        QPoint viewportToCanvas(QPoint viewportCoordinates);
        QPoint canvasToViewport(QPoint canvasCoordinates);

    signals:
        void viewportRectChanged();

    private:
        Ui::Viewport* ui;
        ViewportPrivate* d;

        void updateTimelineSelection();
        void updateViewportRect();

        void paintEvent(QPaintEvent* event);
        void resizeEvent(QResizeEvent *event);
};

#endif // VIEWPORT_H
