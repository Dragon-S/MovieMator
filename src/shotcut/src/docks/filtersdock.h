/*
 * Copyright (c) 2013-2014 Meltytech, LLC
 * Author: Dan Dennedy <dan@dennedy.org>
 * Author: Brian Matherly <code@brianmatherly.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FILTERSDOCK_H
#define FILTERSDOCK_H

#include <QDockWidget>
#include <QObject>
#include <QQuickView>
#include <QQuickWidget>

class QmlFilter;
class QmlMetadata;
class MetadataModel;
class AttachedFiltersModel;

class FiltersDock : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(QRect dockPosition READ dockPosition NOTIFY dockPositionChanged)
public:
    explicit FiltersDock(MetadataModel* metadataModel, AttachedFiltersModel* attachedModel, QWidget *parent = 0);
    int position() const { return m_position; }

//    Q_INVOKABLE int getCurrentPosition();
#ifdef MOVIEMATOR_PRO
    Q_PROPERTY(int position READ position WRITE setPosition);
    void setPosition(int position);
    void setCurrentClipFrameFromTimeline(int position);
#endif
    Q_PROPERTY(bool proVersion READ proVersion)
    bool proVersion();

    Q_INVOKABLE void promptForUpgrade();

    void setExtraQmlContextProperty(QString name, QObject *object);
    QRect dockPosition();

signals:
    void currentFilterRequested(int attachedIndex);
    void changed(); /// Notifies when a filter parameter changes.
    void positionChanged();
#ifdef MOVIEMATOR_PRO
    void changePosition(int position);
#endif
    void dockPositionChanged();

public slots:
    void clearCurrentFilter();
    void setCurrentFilter(QObject* filter, QmlMetadata* meta, int index);
    void setFadeInDuration(int duration);
    void setFadeOutDuration(int duration);
    void onChangePosition();


protected:
    bool event(QEvent *event);

private slots:
    void resetQview();

private:
    QQuickWidget m_qview;
    int m_position;
    QmlFilter *m_qmlFilter;
};

#endif // FILTERSDOCK_H
