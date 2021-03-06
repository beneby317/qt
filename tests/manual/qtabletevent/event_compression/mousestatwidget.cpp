/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mousestatwidget.h"

#include <QTabletEvent>
#include <QPainter>
#include <QTextOption>
#include <QTest>

MouseStatWidget::MouseStatWidget(bool acceptTabletEvent):acceptTabletEvent(acceptTabletEvent),
    receivedMouseEventCount(0),
    receivedMouseEventCountToPaint(0),
    receivedTabletEventCount(0),
    receivedTabletEventCountToPaint(0)
{
    startTimer(1000);
}


void MouseStatWidget::tabletEvent(QTabletEvent *event)
{
    ++receivedTabletEventCount;
    if (acceptTabletEvent)
        event->accept();
    else
        event->ignore();
    // make sure the event loop is slow
    QTest::qSleep(15);
}

void MouseStatWidget::mouseMoveEvent(QMouseEvent *)
{
    ++receivedMouseEventCount;
}

void MouseStatWidget::timerEvent(QTimerEvent *)
{
    receivedMouseEventCountToPaint = receivedMouseEventCount;
    receivedTabletEventCountToPaint = receivedTabletEventCount;
    receivedMouseEventCount = 0;
    receivedTabletEventCount = 0;
    update();
}

void MouseStatWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.drawRect(rect());
    QStringList text;
    text << ((acceptTabletEvent) ? " - tablet events accepted - " : " - tablet events ignored - ");
    text << QString("Number of tablet events received in the last second: %1").arg(QString::number(receivedTabletEventCountToPaint));
    text << QString("Number of mouse events received in the last second: %1").arg(QString::number(receivedMouseEventCountToPaint));

    QTextOption textOption(Qt::AlignCenter);
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    painter.drawText(rect(),
                     text.join("\n"),
                     textOption);
}
