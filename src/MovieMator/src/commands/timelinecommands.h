/*
 * Copyright (c) 2013-2015 Meltytech, LLC
 * Author: Dan Dennedy <dan@dennedy.org>
 *
 * Copyright (c) 2016-2019 EffectMatrix Inc.
 * Author: vgawen <gdb_1986@163.com>
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
#include "qmlmetadata.h"
#include <QString>
#include <QObject>
#include <QTime>
#include <MltTransition.h>
#include "abstractcommand.h"

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

class AppendCommand : public AbstractCommand
{
public:
    AppendCommand(MultitrackModel& model, int trackIndex, const QString& xml, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    QString m_xml;
    UndoHelper m_undoHelper;
};

class InsertCommand : public AbstractCommand
{
public:
    InsertCommand(MultitrackModel& model, int trackIndex, int position, const QString &xml, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_position;
    QString m_xml;
    QStringList m_oldTracks;
    UndoHelper m_undoHelper;
};

class OverwriteCommand : public AbstractCommand
{
public:
    OverwriteCommand(MultitrackModel& model, int trackIndex, int position, const QString &xml, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    QString m_playlistXml;
    int m_position;
    QString m_xml;
    UndoHelper m_undoHelper;
};

class LiftCommand : public AbstractCommand
{
public:
    LiftCommand(MultitrackModel& model, TimelineDock &timeline, int trackIndex, int clipIndex, const QString &xml, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    QString m_xml;
    UndoHelper m_undoHelper;
    TimelineDock &m_timeline;
};

class RemoveCommand : public AbstractCommand
{
public:
    RemoveCommand(MultitrackModel& model, TimelineDock& timeline, int trackIndex, int clipIndex, const QString &xml, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    QString m_xml;
    UndoHelper m_undoHelper;
    TimelineDock& m_timeline;
};

class NameTrackCommand : public AbstractCommand
{
public:
    NameTrackCommand(MultitrackModel& model, int trackIndex, const QString& name, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    QString m_name;
    QString m_oldName;
};

class MuteTrackCommand : public AbstractCommand
{
public:
    MuteTrackCommand(MultitrackModel& model, int trackIndex, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    bool m_oldValue;
};

class HideTrackCommand : public AbstractCommand
{
public:
    HideTrackCommand(MultitrackModel& model, int trackIndex, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    bool m_oldValue;
};

class CompositeTrackCommand : public AbstractCommand
{
public:
    CompositeTrackCommand(MultitrackModel& model, int trackIndex, Qt::CheckState value, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    Qt::CheckState m_value;
    Qt::CheckState m_oldValue;
};

class LockTrackCommand : public AbstractCommand
{
public:
    LockTrackCommand(MultitrackModel& model, int trackIndex, bool value, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    bool m_value;
    bool m_oldValue;
};

class MoveClipCommand : public AbstractCommand
{
public:
    MoveClipCommand(MultitrackModel& model, int fromTrackIndex, int toTrackIndex, int clipIndex, int position, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_fromTrackIndex;
    int m_toTrackIndex;
    int m_fromClipIndex;
    int m_fromStart;
    int m_toStart;
    UndoHelper m_undoHelper;
};

class TrimClipInCommand : public AbstractCommand
{
public:
    TrimClipInCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, bool ripple, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdTrimClipIn; }
    bool mergeWith(const AbstractCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_originalClipIndex;
    int m_delta;
    bool m_ripple;
    UndoHelper m_undoHelper;
};

class TrimClipOutCommand : public AbstractCommand
{
public:
    TrimClipOutCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, bool ripple, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdTrimClipOut; }
    bool mergeWith(const AbstractCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_ripple;
    UndoHelper m_undoHelper;
};

class SplitCommand : public AbstractCommand
{
public:
    SplitCommand(MultitrackModel& model, int trackIndex, int clipIndex, int position, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_position;
};

class FadeInCommand : public AbstractCommand
{
public:
    FadeInCommand(MultitrackModel& model, int trackIndex, int clipIndex, int duration, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdFadeIn; }
    bool mergeWith(const AbstractCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_duration;
    int m_previous;
};

class FadeOutCommand : public AbstractCommand
{
public:
    FadeOutCommand(MultitrackModel& model, int trackIndex, int clipIndex, int duration, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdFadeOut; }
    bool mergeWith(const AbstractCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_duration;
    int m_previous;
};

class AddTransitionCommand : public AbstractCommand
{
public:
    AddTransitionCommand(MultitrackModel& model, int trackIndex, int clipIndex, int position, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_position;
    int m_transitionIndex;
    UndoHelper m_undoHelper;
};

class TrimTransitionInCommand : public AbstractCommand
{
public:
    TrimTransitionInCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdTrimTransitionIn; }
    bool mergeWith(const AbstractCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_notify;
};

class TrimTransitionOutCommand : public AbstractCommand
{
public:
    TrimTransitionOutCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdTrimTransitionOut; }
    bool mergeWith(const AbstractCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_notify;
};

class AddTransitionByTrimInCommand : public AbstractCommand
{
public:
    AddTransitionByTrimInCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdAddTransitionByTrimIn; }
    bool mergeWith(const AbstractCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_notify;
};

class AddTransitionByTrimOutCommand : public AbstractCommand
{
public:
    AddTransitionByTrimOutCommand(MultitrackModel& model, int trackIndex, int clipIndex, int delta, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdAddTransitionByTrimOut; }
    bool mergeWith(const AbstractCommand *other);
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_delta;
    bool m_notify;
};

class AddTrackCommand: public AbstractCommand
{
public:
    AddTrackCommand(MultitrackModel& model, TrackType trackType, AbstractCommand* parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    bool m_isVideo;
    TrackType m_trackType;
};

class InsertTrackCommand : public AbstractCommand
{
public:
    InsertTrackCommand(MultitrackModel& model, int trackIndex, AbstractCommand* parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    TrackType m_trackType;
};

class RemoveTrackCommand : public AbstractCommand
{
public:
    RemoveTrackCommand(MultitrackModel& model, int trackIndex, AbstractCommand* parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int m_trackIndex;
    QString m_xml;
    TrackType m_trackType;
    QString m_trackName;
};

class ChangeBlendModeCommand : public QObject, public AbstractCommand
{
    Q_OBJECT
public:
    ChangeBlendModeCommand(Mlt::Transition& transition, const QString& propertyName, const QString& mode, AbstractCommand* parent = 0);
    void redo_impl();
    void undo_impl();
signals:
    void modeChanged(QString& mode);
private:
    Mlt::Transition m_transition;
    QString m_propertyName;
    QString m_newMode;
    QString m_oldMode;
};

class UpdateCommand : public AbstractCommand
{
public:
    UpdateCommand(TimelineDock& timeline, int trackIndex, int clipIndex, int position,
        AbstractCommand * parent = 0);
    void setXmlAfter(const QString& xml) { m_xmlAfter = xml; }
    void redo_impl();
    void undo_impl();
private:
    TimelineDock& m_timeline;
    int m_trackIndex;
    int m_clipIndex;
    int m_position;
    QString m_xmlAfter;
    bool m_isFirstRedo;
    UndoHelper m_undoHelper;
};



class RemoveTransitionCommand: public AbstractCommand
{
public:
    RemoveTransitionCommand(MultitrackModel &model, int trackIndex, int clipIndex, int transitionIndex, int position, AbstractCommand *parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel &m_model;
    int m_trackIndex;
    int m_clipIndex;
    int m_transitionIndex;
    int m_position;
    UndoHelper m_undoHelper;
};


class RemoveTransitionsOnClipCommand: public AbstractCommand
{
public:
    RemoveTransitionsOnClipCommand(MultitrackModel &model, TimelineDock &timeline, int trackIndex, int clipIndex, AbstractCommand *parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel &m_model;
    int m_trackIndex;
    int m_clipIndex;
    TimelineDock &m_timeline;
    UndoHelper m_undoHelper;
};

class MoveInsertClipCommand:public AbstractCommand
{
public:
    MoveInsertClipCommand(MultitrackModel &model, int fromTrack, int toTrack, int clipIndex, int position, AbstractCommand *parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel &m_model;
    int m_fromTrackIndex;
    int m_toTrackIndex;
    int m_clipIndex;
    int m_position;
    UndoHelper m_undoHelper;
};


class FilterClipCommand : public AbstractCommand
{
public:
    FilterClipCommand(MultitrackModel& model, int TrackIndex, int clipIndex, QString strFromXml, QString strToXml, AbstractCommand * parent = 0);
    void redo_impl();
    void undo_impl();
private:
    MultitrackModel& m_model;
    int     m_trackIndex;
    int     m_clipIndex;
    QString m_strFromXml;
    QString m_strToXml;
    UndoHelper m_undoHelper;
};



class FilterCommand: public AbstractCommand
{

public:
    FilterCommand(Mlt::Filter* filter, QString name,  double from_value, double to_value, AbstractCommand * parent= 0);
    FilterCommand(Mlt::Filter* filter, QString name,  int from_value, int to_value, AbstractCommand * parent= 0);
    FilterCommand(Mlt::Filter* filter, QString name,  QString from_value, QString to_value, AbstractCommand * parent= 0);
    FilterCommand(Mlt::Filter* filter, QString name,  QRectF from_value, QRectF to_value, AbstractCommand * parent= 0);

    ~FilterCommand();
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdFilterCommand; }
    bool mergeWith(const AbstractCommand *other);

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


class KeyFrameCommand: public AbstractCommand
{

public:
    KeyFrameCommand(Mlt::Filter* filter, const QVector<key_frame_item>  &from_value, const QVector<key_frame_item>  &to_value, AbstractCommand * parent= 0);
    ~KeyFrameCommand();
    void redo_impl();
    void undo_impl();
protected:
    int id() const { return UndoIdKeyFrameCommand; }
    bool mergeWith(const AbstractCommand *other);
    void set_value(const QVector<key_frame_item>  &value);
    void notify();

protected:
    Mlt::Filter* m_filter;

    QVector<key_frame_item>   m_from_value;
    QVector<key_frame_item>   m_to_value;

    bool      m_bFirstExec;
    QTime     m_execTime;
};



class FilterAttachCommand: public AbstractCommand
{

public:
    FilterAttachCommand( QmlMetadata *meta, int rowIndex, int metaIndex, bool bAdd, AbstractCommand * parent= 0);
   // ~FilterAttachCommand();
    void redo_impl();
    void undo_impl();
protected:
//    void notify();

protected:
 //   Mlt::Filter*    m_filter;

    QmlMetadata     *m_meta;
    int             m_rowIndex;
    int             m_metaIndex;
    bool            m_bAdd;

    bool            m_bFirstExec;
};

class FilterMoveCommand: public AbstractCommand
{

public:
    FilterMoveCommand(int rowIndexFrom, int rowIndexTo, AbstractCommand * parent= 0);
 //   ~FilterMoveCommand();
    void redo_impl();
    void undo_impl();
protected:
//    void notify();

protected:
    Mlt::Filter*    m_filter;

    int             m_rowIndexFrom;
    int             m_rowIndexTo;

    bool            m_bFirstExec;
};


class ClipsSelectCommand: public AbstractCommand
{

public:
    ClipsSelectCommand(QList<int> newSelection, int newTrackIndex, bool isNewMultitrack,
                       QList<int> oldSelection, int oldTrackIndex, bool isOldMultitrack,
                       AbstractCommand * parent= 0);

    void redo_impl();
    void undo_impl();

protected:
    bool            m_bFirstExec;
    QList<int>  m_newSelection, m_oldSelection;
    int         m_newTrackIndex, m_oldTrackIndex;
    bool        m_bNewIsMultitrack, m_bOldIsMultitrack;
};

} // namespace Timeline

#endif
