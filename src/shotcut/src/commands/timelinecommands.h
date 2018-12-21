/*
 * Copyright (c) 2013-2015 Meltytech, LLC
 * Author: Dan Dennedy <dan@dennedy.org>
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

#ifndef COMMANDS_H
#define COMMANDS_H

#include "models/multitrackmodel.h"
#include "docks/timelinedock.h"
#include "undohelper.h"
#include <QUndoCommand>
#include <QString>
#include <QObject>
#include <QTime>
#include <MltTransition.h>

namespace Timeline
{

enum {
    UndoIdTrimClipIn,
    UndoIdTrimClipOut,
    UndoIdFadeIn,
    UndoIdFadeOut,
    UndoIdTrimTransitionIn,
    UndoIdTrimTransitionOut,
    UndoIdAddTransitionByTrimIn,
    UndoIdAddTransitionByTrimOut,

    UndoIdFilterCommand, //wzq
    UndoIdKeyFrameCommand,
    UndoIdUpdate
};

class AppendCommand : public QUndoCommand
{
public:
    AppendCommand(MultitrackModel& model, int trackIndex, const QString& xml, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    QString m_xml;
    UndoHelper m_undoHelper;
};

class InsertCommand : public QUndoCommand
{
public:
    InsertCommand(MultitrackModel& model, int trackIndex, int position, const QString &xml, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_position;
    QString m_xml;
    QStringList m_oldTracks;
    UndoHelper m_undoHelper;
};

class OverwriteCommand : public QUndoCommand
{
public:
    OverwriteCommand(MultitrackModel& model, int trackIndex, int position, const QString &xml, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    QString m_playlistXml;
    int m_position;
    QString m_xml;
    UndoHelper m_undoHelper;
};

class LiftCommand : public QUndoCommand
{
public:
    LiftCommand(MultitrackModel& model, TimelineDock &timeline, int trackIndex, int clipIndex, const QString &xml, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    QString m_xml;
    UndoHelper m_undoHelper;
    TimelineDock &m_timeline;
};

class RemoveCommand : public QUndoCommand
{
public:
    RemoveCommand(MultitrackModel& model, TimelineDock& timeline, int trackIndex, int clipIndex, const QString &xml, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    QString m_xml;
    UndoHelper m_undoHelper;
    TimelineDock& m_timeline;
};

class NameTrackCommand : public QUndoCommand
{
public:
    NameTrackCommand(MultitrackModel& model, int trackIndex, const QString& name, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    QString m_name;
    QString m_oldName;
};

class MuteTrackCommand : public QUndoCommand
{
public:
    MuteTrackCommand(MultitrackModel& model, int trackIndex, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    bool m_oldValue;
};

class HideTrackCommand : public QUndoCommand
{
public:
    HideTrackCommand(MultitrackModel& model, int trackIndex, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    bool m_oldValue;
};

class CompositeTrackCommand : public QUndoCommand
{
public:
    CompositeTrackCommand(MultitrackModel& model, int trackIndex, Qt::CheckState value, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    Qt::CheckState m_value;
    Qt::CheckState m_oldValue;
};

class LockTrackCommand : public QUndoCommand
{
public:
    LockTrackCommand(MultitrackModel& model, int trackIndex, bool value, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    bool m_value;
    bool m_oldValue;
};

class MoveClipCommand : public QUndoCommand
{
public:
    MoveClipCommand(MultitrackModel& model, int fromTrackIndex, int toTrackIndex, int clipIndex, int position, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_fromTrackIndex;
    int m_toTrackIndex;
    int m_fromClipIndex;
    int m_fromStart;
    int m_toStart;
    UndoHelper m_undoHelper;
};

class TrimClipInCommand : public QUndoCommand
{
public:
    TrimClipInCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, bool ripple, QUndoCommand * parent = 0);
    void redo();
    void undo();
protected:
    int id() const { return UndoIdTrimClipIn; }
    bool mergeWith(const QUndoCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_originalClipIndex;
    int m_delta;
    bool m_ripple;
    UndoHelper m_undoHelper;
};

class TrimClipOutCommand : public QUndoCommand
{
public:
    TrimClipOutCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, bool ripple, QUndoCommand * parent = 0);
    void redo();
    void undo();
protected:
    int id() const { return UndoIdTrimClipOut; }
    bool mergeWith(const QUndoCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_ripple;
    UndoHelper m_undoHelper;
};

class SplitCommand : public QUndoCommand
{
public:
    SplitCommand(MultitrackModel& model, int trackIndex, int clipIndex, int position, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_position;
};

class FadeInCommand : public QUndoCommand
{
public:
    FadeInCommand(MultitrackModel& model, int trackIndex, int clipIndex, int duration, QUndoCommand * parent = 0);
    void redo();
    void undo();
protected:
    int id() const { return UndoIdFadeIn; }
    bool mergeWith(const QUndoCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_duration;
    int m_previous;
};

class FadeOutCommand : public QUndoCommand
{
public:
    FadeOutCommand(MultitrackModel& model, int trackIndex, int clipIndex, int duration, QUndoCommand * parent = 0);
    void redo();
    void undo();
protected:
    int id() const { return UndoIdFadeOut; }
    bool mergeWith(const QUndoCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_duration;
    int m_previous;
};

class AddTransitionCommand : public QUndoCommand
{
public:
    AddTransitionCommand(MultitrackModel& model, int trackIndex, int clipIndex, int position, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_position;
    int m_transitionIndex;
    UndoHelper m_undoHelper;
};

class TrimTransitionInCommand : public QUndoCommand
{
public:
    TrimTransitionInCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, QUndoCommand * parent = 0);
    void redo();
    void undo();
protected:
    int id() const { return UndoIdTrimTransitionIn; }
    bool mergeWith(const QUndoCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_notify;
};

class TrimTransitionOutCommand : public QUndoCommand
{
public:
    TrimTransitionOutCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, QUndoCommand * parent = 0);
    void redo();
    void undo();
protected:
    int id() const { return UndoIdTrimTransitionOut; }
    bool mergeWith(const QUndoCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_notify;
};

class AddTransitionByTrimInCommand : public QUndoCommand
{
public:
    AddTransitionByTrimInCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, QUndoCommand * parent = 0);
    void redo();
    void undo();
protected:
    int id() const { return UndoIdAddTransitionByTrimIn; }
    bool mergeWith(const QUndoCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_notify;
};

class AddTransitionByTrimOutCommand : public QUndoCommand
{
public:
    AddTransitionByTrimOutCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, QUndoCommand * parent = 0);
    void redo();
    void undo();
protected:
    int id() const { return UndoIdAddTransitionByTrimOut; }
    bool mergeWith(const QUndoCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_notify;
};

class AddTrackCommand: public QUndoCommand
{
public:
    AddTrackCommand(MultitrackModel& model, TrackType trackType, QUndoCommand* parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    bool m_isVideo;
    TrackType m_trackType;
};

class InsertTrackCommand : public QUndoCommand
{
public:
    InsertTrackCommand(MultitrackModel& model, int trackIndex, QUndoCommand* parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    TrackType m_trackType;
};

class RemoveTrackCommand : public QUndoCommand
{
public:
    RemoveTrackCommand(MultitrackModel& model, int trackIndex, QUndoCommand* parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    QString m_xml;
    TrackType m_trackType;
    QString m_trackName;
};

class ChangeBlendModeCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    ChangeBlendModeCommand(Mlt::Transition& transition, const QString& propertyName, const QString& mode, QUndoCommand* parent = 0);
    void redo();
    void undo();
signals:
    void modeChanged(QString& mode);
private:
    Mlt::Transition m_transition;
    QString m_propertyName;
    QString m_newMode;
    QString m_oldMode;
};

class UpdateCommand : public QUndoCommand
{
public:
    UpdateCommand(TimelineDock& timeline, int trackIndex, int clipIndex, int position,
        QUndoCommand * parent = 0);
    void setXmlAfter(const QString& xml) { m_xmlAfter = xml; }
    void redo();
    void undo();
private:
    TimelineDock& m_timeline;
    int m_trackIndex;
    int m_clipIndex;
    int m_position;
    QString m_xmlAfter;
    bool m_isFirstRedo;
    UndoHelper m_undoHelper;
};



class RemoveTransitionCommand: public QUndoCommand
{
public:
    RemoveTransitionCommand(MultitrackModel &model, int trackIndex, int clipIndex, int transitionIndex, int position, QUndoCommand *parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel &m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_transitionIndex;
    int m_position;
    UndoHelper m_undoHelper;
};


class RemoveTransitionsOnClipCommand: public QUndoCommand
{
public:
    RemoveTransitionsOnClipCommand(MultitrackModel &model, TimelineDock &timeline, int trackIndex, int clipIndex, QUndoCommand *parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel &m_model;
    int m_trackIndex;
    int m_clipIndex;
    TimelineDock &m_timeline;
    UndoHelper m_undoHelper;
};

class MoveInsertClipCommand:public QUndoCommand
{
public:
    MoveInsertClipCommand(MultitrackModel &model, int fromTrack, int toTrack, int clipIndex, int position, QUndoCommand *parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel &m_model;
    int m_fromTrackIndex;
    int m_toTrackIndex;
    int m_clipIndex;
    int m_position;
    UndoHelper m_undoHelper;
};


class FilterClipCommand : public QUndoCommand
{
public:
    FilterClipCommand(MultitrackModel& model, int TrackIndex, int clipIndex, QString strFromXml, QString strToXml, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    MultitrackModel& m_model;
    int     m_trackIndex;
    int     m_clipIndex;
    QString m_strFromXml;
    QString m_strToXml;
    UndoHelper m_undoHelper;
};



class FilterCommand: public QUndoCommand
{

public:
    FilterCommand(Mlt::Filter* filter, QString name,  double from_value, double to_value, QUndoCommand * parent= 0);
    FilterCommand(Mlt::Filter* filter, QString name,  int from_value, int to_value, QUndoCommand * parent= 0);
    FilterCommand(Mlt::Filter* filter, QString name,  QString from_value, QString to_value, QUndoCommand * parent= 0);
    FilterCommand(Mlt::Filter* filter, QString name,  QRectF from_value, QRectF to_value, QUndoCommand * parent= 0);

    ~FilterCommand();
    void redo();
    void undo();
protected:
    int id() const { return UndoIdFilterCommand; }
    bool mergeWith(const QUndoCommand *other);

    int transitionValue(QVariant &varFrom, QVariant &varTo, Mlt::Filter* filter, QString name,  double from_value, double to_value);

    void notify();
    void set_value(QVariant value);
protected:
    Mlt::Filter* m_filter;
    QString     m_keyName;

    QVariant  m_from_value;
    QVariant  m_to_value;

    bool      m_bFirstExec;
};


class KeyFrameCommand: public QUndoCommand
{

public:
    KeyFrameCommand(Mlt::Filter* filter, const QVector<key_frame_item>  &from_value, const QVector<key_frame_item>  &to_value, QUndoCommand * parent= 0);
    ~KeyFrameCommand();
    void redo();
    void undo();
protected:
    int id() const { return UndoIdKeyFrameCommand; }
    bool mergeWith(const QUndoCommand *other);
    void set_value(const QVector<key_frame_item>  &value);
    void notify();

protected:
    Mlt::Filter* m_filter;

    QVector<key_frame_item>   m_from_value;
    QVector<key_frame_item>   m_to_value;

    bool      m_bFirstExec;
    QTime     m_execTime;
};



class FilterAttachCommand: public QUndoCommand
{

public:
    FilterAttachCommand(Mlt::Filter* filter, const QVector<key_frame_item>  &from_value, const QVector<key_frame_item>  &to_value, QUndoCommand * parent= 0);
    ~FilterAttachCommand();
    void redo();
    void undo();
protected:
    void set_value(const QVector<key_frame_item>  &value);
    void notify();

protected:
    Mlt::Filter* m_filter;

    QVector<key_frame_item>   m_from_value;
    QVector<key_frame_item>   m_to_value;

    bool      m_bFirstExec;
};

} // namespace Timeline

#endif
