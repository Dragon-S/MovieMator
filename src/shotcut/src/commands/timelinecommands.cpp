/*
 * Copyright (c) 2013-2016 Meltytech, LLC
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

#include "timelinecommands.h"
#include "mltcontroller.h"
#include "shotcut_mlt_properties.h"
#include "mainwindow.h"
#include "controllers/filtercontroller.h"
#include "docks/timelinedock.h"
#include <Logger.h>
#include <QMetaObject>

namespace Timeline {

AppendCommand::AppendCommand(MultitrackModel &model, int trackIndex, const QString &xml, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_xml(xml)
    , m_undoHelper(m_model)
{
    setText(QObject::tr("Append to track"));
}


void AppendCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex;
    m_undoHelper.recordBeforeState();
    Mlt::Producer producer(MLT.profile(), "xml-string", m_xml.toUtf8().constData());
    m_model.appendClip(m_trackIndex, producer);
    m_undoHelper.recordAfterState();
}

void AppendCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex;
    m_undoHelper.undoChanges();
}

InsertCommand::InsertCommand(MultitrackModel &model, int trackIndex,
    int position, const QString &xml, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_position(position)
    , m_xml(xml)
    , m_undoHelper(m_model)
{
    setText(QObject::tr("Insert into track"));
}

void InsertCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "position" << m_position;
    m_undoHelper.recordBeforeState();
    Mlt::Producer clip(MLT.profile(), "xml-string", m_xml.toUtf8().constData());
    m_model.insertClip(m_trackIndex, clip, m_position);
    m_undoHelper.recordAfterState();
}

void InsertCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "position" << m_position;
    m_undoHelper.undoChanges();
}

OverwriteCommand::OverwriteCommand(MultitrackModel &model, int trackIndex,
    int position, const QString &xml, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_position(position)
    , m_xml(xml)
    , m_undoHelper(m_model)
{
    setText(QObject::tr("Overwrite onto track"));
}

void OverwriteCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "position" << m_position;
    m_undoHelper.recordBeforeState();
    Mlt::Producer clip(MLT.profile(), "xml-string", m_xml.toUtf8().constData());
    m_playlistXml = m_model.overwrite(m_trackIndex, clip, m_position);
    m_undoHelper.recordAfterState();
}

void OverwriteCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "position" << m_position;
    m_undoHelper.undoChanges();
}

LiftCommand::LiftCommand(MultitrackModel &model, TimelineDock &timeline, int trackIndex,
    int clipIndex, const QString &xml, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_xml(xml)
    , m_undoHelper(m_model)
    , m_timeline(timeline)
{
    setText(QObject::tr("Lift from track"));
}

void LiftCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex;
    m_undoHelper.recordBeforeState();
//    int clipIndex = m_timeline.removeTransitionOnClip(m_trackIndex, m_clipIndex);
//    m_model.liftClip(m_trackIndex, clipIndex);
    m_model.liftClip(m_trackIndex, m_clipIndex);
    m_undoHelper.recordAfterState();
}

void LiftCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex;
    m_undoHelper.undoChanges();
}

RemoveCommand::RemoveCommand(MultitrackModel &model, TimelineDock& timeline, int trackIndex,
    int clipIndex, const QString &xml, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_xml(xml)
    , m_undoHelper(m_model)
    , m_timeline(timeline)
{
    setText(QObject::tr("Remove from track"));
}

void RemoveCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex;
    m_undoHelper.recordBeforeState();
//    int clipIndex = m_timeline.removeTransitionOnClip(m_trackIndex, m_clipIndex);
//    m_model.removeClip(m_trackIndex, clipIndex);
    m_model.removeClip(m_trackIndex, m_clipIndex);
    m_undoHelper.recordAfterState();
}

void RemoveCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex;
    m_undoHelper.undoChanges();
}


NameTrackCommand::NameTrackCommand(MultitrackModel &model, int trackIndex,
    const QString &name, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_name(name)
    , m_oldName(model.data(m_model.index(trackIndex), MultitrackModel::NameRole).toString())
{
    setText(QObject::tr("Change track name"));
}

void NameTrackCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "name" << m_name;
    m_model.setTrackName(m_trackIndex, m_name);
}

void NameTrackCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "name" << m_name;
    m_model.setTrackName(m_trackIndex, m_oldName);
}

MuteTrackCommand::MuteTrackCommand(MultitrackModel &model, int trackIndex, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_oldValue(model.data(m_model.index(trackIndex), MultitrackModel::IsMuteRole).toBool())
{
    setText(QObject::tr("Toggle track mute"));
}

void MuteTrackCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "mute" << !m_oldValue;
    m_model.setTrackMute(m_trackIndex, !m_oldValue);
}

void MuteTrackCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "mute" << !m_oldValue;
    m_model.setTrackMute(m_trackIndex, m_oldValue);
}

HideTrackCommand::HideTrackCommand(MultitrackModel &model, int trackIndex, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_oldValue(model.data(m_model.index(trackIndex), MultitrackModel::IsHiddenRole).toBool())
{
    setText(QObject::tr("Toggle track hidden"));
}

void HideTrackCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "hide" << !m_oldValue;
    m_model.setTrackHidden(m_trackIndex, !m_oldValue);
}

void HideTrackCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "hide" << !m_oldValue;
    m_model.setTrackHidden(m_trackIndex, m_oldValue);
}

CompositeTrackCommand::CompositeTrackCommand(MultitrackModel &model, int trackIndex, Qt::CheckState value, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_value(value)
    , m_oldValue(Qt::CheckState(model.data(m_model.index(trackIndex), MultitrackModel::IsCompositeRole).toInt()))
{
    setText(QObject::tr("Change track compositing"));
}

void CompositeTrackCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "composite" << m_value;
    m_model.setTrackComposite(m_trackIndex, m_value);
}

void CompositeTrackCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "composite" << m_value;
    m_model.setTrackComposite(m_trackIndex, m_oldValue);
}

LockTrackCommand::LockTrackCommand(MultitrackModel &model, int trackIndex, bool value, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_value(value)
    , m_oldValue(model.data(m_model.index(trackIndex), MultitrackModel::IsLockedRole).toBool())
{
    setText(QObject::tr("Lock track"));
}

void LockTrackCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "lock" << m_value;
    m_model.setTrackLock(m_trackIndex, m_value);
}

void LockTrackCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "lock" << m_value;
    m_model.setTrackLock(m_trackIndex, m_oldValue);
}

MoveClipCommand::MoveClipCommand(MultitrackModel &model, int fromTrackIndex, int toTrackIndex, int clipIndex, int position, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_fromTrackIndex(fromTrackIndex)
    , m_toTrackIndex(toTrackIndex)
    , m_fromClipIndex(clipIndex)
    , m_fromStart(model.data(
        m_model.index(clipIndex, 0, m_model.index(fromTrackIndex)),
            MultitrackModel::StartRole).toInt())
    , m_toStart(position)
    , m_undoHelper(m_model)
{
    setText(QObject::tr("Move clip"));
}

void MoveClipCommand::redo()
{
    LOG_DEBUG() << "fromTrack" << m_fromTrackIndex << "toTrack" << m_toTrackIndex;
    m_undoHelper.recordBeforeState();
    m_model.moveClip(m_fromTrackIndex, m_toTrackIndex, m_fromClipIndex, m_toStart);
    m_undoHelper.recordAfterState();
    qDebug()<<"redo ends";
}

void MoveClipCommand::undo()
{
    LOG_DEBUG() << "fromTrack" << m_fromTrackIndex << "toTrack" << m_toTrackIndex;
    m_undoHelper.undoChanges();
}

TrimClipInCommand::TrimClipInCommand(MultitrackModel &model, int trackIndex, int clipIndex, int delta, bool ripple, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_originalClipIndex(clipIndex)
    , m_delta(delta)
    , m_ripple(ripple)
    , m_undoHelper(m_model)
{
    setText(QObject::tr("Trim clip in point"));
    if (!ripple)
        m_undoHelper.setHints(UndoHelper::SkipXML);
}

void TrimClipInCommand::redo()
{
    m_undoHelper.recordBeforeState();
    m_clipIndex = m_model.trimClipIn(m_trackIndex, m_originalClipIndex, m_delta, m_ripple);
    m_undoHelper.recordAfterState();
}

void TrimClipInCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "delta" << m_delta;
    m_undoHelper.undoChanges();
}

bool TrimClipInCommand::mergeWith(const QUndoCommand *other)
{
    const TrimClipInCommand* that = static_cast<const TrimClipInCommand*>(other);
    if (that->id() != id() || that->m_trackIndex != m_trackIndex || (that->m_clipIndex != m_clipIndex && that->m_clipIndex != m_originalClipIndex))
        return false;
    m_undoHelper.recordAfterState();
    m_delta += static_cast<const TrimClipInCommand*>(other)->m_delta;
    return true;
}

TrimClipOutCommand::TrimClipOutCommand(MultitrackModel &model, int trackIndex, int clipIndex, int delta, bool ripple, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_delta(delta)
    , m_ripple(ripple)
    , m_undoHelper(m_model)
{
    setText(QObject::tr("Trim clip out point"));
    if (!ripple)
        m_undoHelper.setHints(UndoHelper::SkipXML);
}

void TrimClipOutCommand::redo()
{
    m_undoHelper.recordBeforeState();
    m_clipIndex = m_model.trimClipOut(m_trackIndex, m_clipIndex, m_delta, m_ripple);
    m_undoHelper.recordAfterState();
}

void TrimClipOutCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "delta" << m_delta;
    m_undoHelper.undoChanges();
}

bool TrimClipOutCommand::mergeWith(const QUndoCommand *other)
{
    const TrimClipOutCommand* that = static_cast<const TrimClipOutCommand*>(other);
    if (that->id() != id() || that->m_trackIndex != m_trackIndex || that->m_clipIndex != m_clipIndex)
        return false;
    m_undoHelper.recordAfterState();
    m_delta += static_cast<const TrimClipOutCommand*>(other)->m_delta;
    return true;
}

SplitCommand::SplitCommand(MultitrackModel &model, int trackIndex,
    int clipIndex, int position, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_position(position)
{
    setText(QObject::tr("Split clip"));
}

void SplitCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "position" << m_position;
    m_model.splitClip(m_trackIndex, m_clipIndex, m_position);
}

void SplitCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "position" << m_position;
    m_model.joinClips(m_trackIndex, m_clipIndex);
}

FadeInCommand::FadeInCommand(MultitrackModel &model, int trackIndex, int clipIndex, int duration, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_duration(duration)
{
    QModelIndex modelIndex = m_model.index(clipIndex, 0, m_model.index(trackIndex));
    m_previous = model.data(modelIndex, MultitrackModel::FadeInRole).toInt();
    setText(QObject::tr("Adjust fade in"));
}

void FadeInCommand::redo()
{
    m_model.fadeIn(m_trackIndex, m_clipIndex, m_duration);
}

void FadeInCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "duration" << m_duration;
    m_model.fadeIn(m_trackIndex, m_clipIndex, m_previous);
}

bool FadeInCommand::mergeWith(const QUndoCommand *other)
{
    const FadeInCommand* that = static_cast<const FadeInCommand*>(other);
    if (that->id() != id() || that->m_trackIndex != m_trackIndex || that->m_clipIndex != m_clipIndex)
        return false;
    m_duration = static_cast<const FadeInCommand*>(other)->m_duration;
    return true;
}

FadeOutCommand::FadeOutCommand(MultitrackModel &model, int trackIndex, int clipIndex, int duration, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_duration(duration)
{
    QModelIndex modelIndex = m_model.index(clipIndex, 0, m_model.index(trackIndex));
    m_previous = model.data(modelIndex, MultitrackModel::FadeOutRole).toInt();
    setText(QObject::tr("Adjust fade out"));
}

void FadeOutCommand::redo()
{
    m_model.fadeOut(m_trackIndex, m_clipIndex, m_duration);
}

void FadeOutCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "duration" << m_duration;
    m_model.fadeOut(m_trackIndex, m_clipIndex, m_previous);
}

bool FadeOutCommand::mergeWith(const QUndoCommand *other)
{
    const FadeOutCommand* that = static_cast<const FadeOutCommand*>(other);
    if (that->id() != id() || that->m_trackIndex != m_trackIndex || that->m_clipIndex != m_clipIndex)
        return false;
    m_duration = static_cast<const FadeOutCommand*>(other)->m_duration;
    return true;
}

AddTransitionCommand::AddTransitionCommand(MultitrackModel &model, int trackIndex, int clipIndex, int position, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_position(position)
    , m_transitionIndex(-1)
    , m_undoHelper(model)
{
    setText(QObject::tr("Add transition"));
}

void AddTransitionCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "position" << m_position;
    m_undoHelper.recordBeforeState();
    m_transitionIndex = m_model.addTransition(m_trackIndex, m_clipIndex, m_position);
    m_undoHelper.recordAfterState();
}

void AddTransitionCommand::undo()
{
    m_undoHelper.undoChanges();
//    if (m_transitionIndex >= 0) {
//        LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "position" << m_position;
//        m_model.removeTransition(m_trackIndex, m_transitionIndex);
//        // Delete the blank that was inserted.
//        int i = m_model.trackList().at(m_trackIndex).mlt_index;
//        QScopedPointer<Mlt::Producer> track(m_model.tractor()->track(i));
//        if (track) {
//            Mlt::Playlist playlist(*track);
//            if (playlist.is_blank(m_clipIndex + 1) && m_transitionIndex == m_clipIndex) { // dragged left
//                m_model.removeClip(m_trackIndex, m_clipIndex + 1);
//            } else if (playlist.is_blank(m_clipIndex)) {
//                m_model.removeClip(m_trackIndex, m_clipIndex);
//            }
//        }
//    } else {
//        LOG_WARNING() << "Failed to undo the transition!";
//    }
}

TrimTransitionInCommand::TrimTransitionInCommand(MultitrackModel &model, int trackIndex, int clipIndex, int delta, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_delta(delta)
    , m_notify(false)
{
    setText(QObject::tr("Trim transition in point"));
}

void TrimTransitionInCommand::redo()
{
    m_model.trimTransitionIn(m_trackIndex, m_clipIndex, m_delta);
    if (m_notify && m_clipIndex >= 0)
        m_model.notifyClipIn(m_trackIndex, m_clipIndex);
}

void TrimTransitionInCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "delta" << m_delta;
    if (m_clipIndex >= 0) {
        m_model.trimTransitionIn(m_trackIndex, m_clipIndex, -m_delta);
        m_model.notifyClipIn(m_trackIndex, m_clipIndex);
        m_notify = true;
    }
    else LOG_WARNING() << "invalid clip index" << m_clipIndex;
}

bool TrimTransitionInCommand::mergeWith(const QUndoCommand *other)
{
    const TrimTransitionInCommand* that = static_cast<const TrimTransitionInCommand*>(other);
    if (that->id() != id() || that->m_trackIndex != m_trackIndex || that->m_clipIndex != m_clipIndex)
        return false;
    m_delta += static_cast<const TrimTransitionInCommand*>(other)->m_delta;
    return true;
}

TrimTransitionOutCommand::TrimTransitionOutCommand(MultitrackModel &model, int trackIndex, int clipIndex, int delta, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_delta(delta)
    , m_notify(false)
{
    setText(QObject::tr("Trim transition out point"));
}

void TrimTransitionOutCommand::redo()
{
    m_model.trimTransitionOut(m_trackIndex, m_clipIndex, m_delta);
    if (m_notify && m_clipIndex >= 0)
        m_model.notifyClipOut(m_trackIndex, m_clipIndex);
}

void TrimTransitionOutCommand::undo()
{
    if (m_clipIndex >= 0) {
        LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "delta" << m_delta;
        m_model.trimTransitionOut(m_trackIndex, m_clipIndex, -m_delta);
        m_model.notifyClipOut(m_trackIndex, m_clipIndex);
        m_notify = true;
    }
    else LOG_WARNING() << "invalid clip index" << m_clipIndex;
}

bool TrimTransitionOutCommand::mergeWith(const QUndoCommand *other)
{
    const TrimTransitionOutCommand* that = static_cast<const TrimTransitionOutCommand*>(other);
    if (that->id() != id() || that->m_trackIndex != m_trackIndex || that->m_clipIndex != m_clipIndex)
        return false;
    m_delta += static_cast<const TrimTransitionOutCommand*>(other)->m_delta;
    return true;
}

AddTransitionByTrimInCommand::AddTransitionByTrimInCommand(MultitrackModel &model, int trackIndex, int clipIndex, int delta, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_delta(delta)
    , m_notify(false)
{
    setText(QObject::tr("Add transition"));
}

void AddTransitionByTrimInCommand::redo()
{
    m_model.addTransitionByTrimIn(m_trackIndex, m_clipIndex, m_delta);
    if (m_notify && m_clipIndex > 0)
        m_model.notifyClipOut(m_trackIndex, m_clipIndex - 1);
}

void AddTransitionByTrimInCommand::undo()
{
    if (m_clipIndex > 0) {
        LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "delta" << m_delta;
        m_model.removeTransitionByTrimIn(m_trackIndex, m_clipIndex, m_delta);
        m_notify = true;
    }
    else LOG_WARNING() << "invalid clip index" << m_clipIndex;
}

bool AddTransitionByTrimInCommand::mergeWith(const QUndoCommand *other)
{
    const AddTransitionByTrimInCommand* that = static_cast<const AddTransitionByTrimInCommand*>(other);
    if (that->id() != id() || that->m_trackIndex != m_trackIndex ||
        (that->m_clipIndex != m_clipIndex && m_clipIndex != that->m_clipIndex - 1))
        return false;
    return true;
}

AddTransitionByTrimOutCommand::AddTransitionByTrimOutCommand(MultitrackModel &model, int trackIndex, int clipIndex, int delta, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_delta(delta)
    , m_notify(false)
{
    setText(QObject::tr("Add transition"));
}

void AddTransitionByTrimOutCommand::redo()
{
    m_model.addTransitionByTrimOut(m_trackIndex, m_clipIndex, m_delta);
    if (m_notify)
        m_model.notifyClipIn(m_trackIndex, m_clipIndex + 2);
}

void AddTransitionByTrimOutCommand::undo()
{
    if (m_clipIndex + 2 < m_model.rowCount(m_model.index(m_trackIndex))) {
        LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "delta" << m_delta;
        m_model.removeTransitionByTrimOut(m_trackIndex, m_clipIndex, m_delta);
        m_notify = true;
    }
    else LOG_WARNING() << "invalid clip index" << m_clipIndex;
}

bool AddTransitionByTrimOutCommand::mergeWith(const QUndoCommand *other)
{
    const AddTransitionByTrimOutCommand* that = static_cast<const AddTransitionByTrimOutCommand*>(other);
    if (that->id() != id() || that->m_trackIndex != m_trackIndex || that->m_clipIndex != m_clipIndex)
        return false;
    return true;
}

AddTrackCommand::AddTrackCommand(MultitrackModel& model, TrackType trackType, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackType(trackType)
{
    if (trackType == VideoTrackType)
        setText(QObject::tr("Add video track"));
    else if (trackType == AudioTrackType)
        setText(QObject::tr("Add audio track"));
    else if (trackType == FilterTrackType)
        setText(QObject::tr("Add filter track"));
    else if (trackType == TextTrackType)
        setText(QObject::tr("Add text track"));
}

void AddTrackCommand::redo()
{
    LOG_DEBUG() << (m_isVideo? "video" : "audio");
//    if (m_isVideo)

    if (m_trackType == VideoTrackType)
        m_trackIndex = m_model.addVideoTrack();
    else if (m_trackType == AudioTrackType)
        m_trackIndex = m_model.addAudioTrack();
    else if (m_trackType == FilterTrackType)
        m_trackIndex = m_model.addFilterTrack();
    else if(m_trackType == TextTrackType)
        m_trackIndex = m_model.addTextTrack();
}

void AddTrackCommand::undo()
{
    LOG_DEBUG() << (m_isVideo? "video" : "audio");
    m_model.removeTrack(m_trackIndex);
}

InsertTrackCommand::InsertTrackCommand(MultitrackModel& model, int trackIndex, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_trackType(model.trackList().size() > 0 ? model.trackList().at(trackIndex).type : VideoTrackType)
{
    if (m_trackType == AudioTrackType)
        setText(QObject::tr("Insert audio track"));
    else if (m_trackType == VideoTrackType)
        setText(QObject::tr("Insert video track"));
}

void InsertTrackCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "type" << (m_trackType == AudioTrackType? "audio" : "video");
    m_model.insertTrack(m_trackIndex, m_trackType);
}

void InsertTrackCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "type" << (m_trackType == AudioTrackType? "audio" : "video");
    m_model.removeTrack(m_trackIndex);
}

RemoveTrackCommand::RemoveTrackCommand(MultitrackModel& model, int trackIndex, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_trackType(model.trackList().at(trackIndex).type)
{
    if (m_trackType == AudioTrackType)
        setText(QObject::tr("Remove audio track"));
    else if (m_trackType == VideoTrackType)
        setText(QObject::tr("Remove video track"));

    // Save track XML.
    int mlt_index = m_model.trackList().at(m_trackIndex).mlt_index;
    QScopedPointer<Mlt::Producer> producer(m_model.tractor()->multitrack()->track(mlt_index));
    if (producer && producer->is_valid()) {
        m_xml = MLT.XML(producer.data());
        m_trackName = QString::fromUtf8(producer->get(kTrackNameProperty));
    }
}

void RemoveTrackCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "type" << (m_trackType == AudioTrackType? "audio" : "video");
    m_model.removeTrack(m_trackIndex);
}

void RemoveTrackCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "type" << (m_trackType == AudioTrackType? "audio" : "video");
    m_model.insertTrack(m_trackIndex, m_trackType);
    m_model.setTrackName(m_trackIndex, m_trackName);

    // Restore track from XML.
    Mlt::Producer producer(MLT.profile(), "xml-string", m_xml.toUtf8().constData());
    Mlt::Playlist playlist(producer);
    m_model.appendFromPlaylist(&playlist, m_trackIndex);

    // Re-attach filters.
    int n = playlist.filter_count();
    if (n > 0) {
        int mlt_index = m_model.trackList().at(m_trackIndex).mlt_index;
        QScopedPointer<Mlt::Producer> producer(m_model.tractor()->multitrack()->track(mlt_index));
        for (int i = 0; i < n; ++i) {
            QScopedPointer<Mlt::Filter> filter(playlist.filter(i));
            if (filter && filter->is_valid())
                producer->attach(*filter);
        }
    }
}

ChangeBlendModeCommand::ChangeBlendModeCommand(Mlt::Transition& transition, const QString& propertyName, const QString& mode, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_transition(transition)
    , m_propertyName(propertyName)
    , m_newMode(mode)
{
    setText(QObject::tr("Change track blend mode"));
    m_oldMode = m_transition.get(m_propertyName.toLatin1().constData());
}

void ChangeBlendModeCommand::redo()
{
    LOG_DEBUG() << "mode" << m_newMode;
    if (!m_newMode.isEmpty()) {
        m_transition.set(m_propertyName.toLatin1().constData(), m_newMode.toUtf8().constData());
        MLT.refreshConsumer();
        emit modeChanged(m_newMode);
    }
}

void ChangeBlendModeCommand::undo()
{
    LOG_DEBUG() << "mode" << m_newMode;
    if (!m_oldMode.isEmpty()) {
        m_transition.set(m_propertyName.toLatin1().constData(), m_oldMode.toUtf8().constData());
        MLT.refreshConsumer();
        emit modeChanged(m_oldMode);
    }
}

UpdateCommand::UpdateCommand(TimelineDock& timeline, int trackIndex, int clipIndex,
    int position, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_timeline(timeline)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_position(position)
    , m_isFirstRedo(true)
    , m_undoHelper(*timeline.model())
{
    setText(QObject::tr("Change clip properties"));
    m_undoHelper.recordBeforeState();
}

void UpdateCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "position" << m_position;
    if (!m_isFirstRedo)
        m_undoHelper.recordBeforeState();
    Mlt::Producer clip(MLT.profile(), "xml-string", m_xmlAfter.toUtf8().constData());
//    m_timeline.model()->liftClip(m_trackIndex, m_clipIndex);
//    m_timeline.model()->overwrite(m_trackIndex, clip, m_position, false);
    m_timeline.model()->removeClip(m_trackIndex, m_clipIndex);
    m_timeline.model()->insertClip(m_trackIndex, clip, m_position);
    m_undoHelper.recordAfterState();
}

void UpdateCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "position" << m_position;
    m_undoHelper.undoChanges();
    m_timeline.emitSelectedFromSelection();
    m_isFirstRedo = false;
}

RemoveTransitionCommand::RemoveTransitionCommand(MultitrackModel &model, int trackIndex, int clipIndex, int transitionIndex, int position, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_transitionIndex(transitionIndex)
    , m_position(position)
    , m_undoHelper(model)
{
    setText(QObject::tr("Remove transition"));
}

void RemoveTransitionCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex << "transitionIndex" << m_transitionIndex << "position" << m_position;
    m_undoHelper.recordBeforeState();
    m_model.removeTransition(m_trackIndex, m_transitionIndex);
    m_undoHelper.recordAfterState();
}

void RemoveTransitionCommand::undo()
{
//    QString projectXML =  MLT.XML(m_model.tractor());
//    QFile outFile1("/Users/gdb/Desktop/file1.xml");
//    outFile1.open(QIODevice::WriteOnly);
//    QTextStream ts1(&outFile1);
//    ts1 << projectXML << endl;

    m_undoHelper.undoChanges();

//    projectXML =  MLT.XML(m_model.tractor());
//    QFile outFile2("/Users/gdb/Desktop/file2.xml");
//    outFile2.open(QIODevice::WriteOnly);
//    QTextStream ts2(&outFile2);
//    ts2 << projectXML << endl;


//    m_model.addTransition(m_trackIndex, m_clipIndex, m_position);
//    // Delete the blank that was inserted.
//    int i = m_model.trackList().at(m_trackIndex).mlt_index;
//    QScopedPointer<Mlt::Producer> track(m_model.tractor()->track(i));
//    if (track) {
//        Mlt::Playlist playlist(*track);
//        if (playlist.is_blank(m_clipIndex + 2) && m_transitionIndex == m_clipIndex) { // dragged left
//            m_model.removeClip(m_trackIndex, m_clipIndex + 2);
//        }
//    }
}

RemoveTransitionsOnClipCommand::RemoveTransitionsOnClipCommand(MultitrackModel &model, TimelineDock &timeline, int trackIndex, int clipIndex, QUndoCommand *parent)
    :QUndoCommand(parent)
    , m_model(model)
    , m_timeline(timeline)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_undoHelper(m_model)
{
    setText("Remove transition");
}

void RemoveTransitionsOnClipCommand::redo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex;
    m_undoHelper.recordBeforeState();
    m_timeline.removeTransitionOnClip(m_trackIndex, m_clipIndex);
    m_undoHelper.recordAfterState();
}

void RemoveTransitionsOnClipCommand::undo()
{
    LOG_DEBUG() << "trackIndex" << m_trackIndex << "clipIndex" << m_clipIndex;
    m_undoHelper.undoChanges();
}

MoveInsertClipCommand::MoveInsertClipCommand(MultitrackModel &model, int fromTrack, int toTrack, int clipIndex, int position, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_fromTrackIndex(fromTrack)
    , m_toTrackIndex(toTrack)
    , m_clipIndex(clipIndex)
    , m_position(position)
    , m_undoHelper(m_model)
{
    setText(QObject::tr("Move clip"));
}

void MoveInsertClipCommand::redo()
{
    LOG_DEBUG() << "fromTrack" << m_fromTrackIndex << "toTrack" << m_toTrackIndex << "clipIndex" << m_clipIndex << "position" << m_position;
    m_undoHelper.recordBeforeState();
    m_model.moveInsertClip(m_fromTrackIndex, m_toTrackIndex, m_clipIndex, m_position);
    m_undoHelper.recordAfterState();
}

void MoveInsertClipCommand::undo()
{
    LOG_DEBUG() << "fromTrack" << m_fromTrackIndex << "toTrack" << m_toTrackIndex << "clipIndex" << m_clipIndex << "position" << m_position;
    m_undoHelper.undoChanges();
}

FilterCommand::FilterCommand(Mlt::Filter* filter, QString name, double from_value, double to_value, QUndoCommand * parent)
 : QUndoCommand(parent)
{
    m_filter = new Mlt::Filter(filter->get_filter());
    m_keyName   = name;
    m_from_value    = QVariant(from_value);
    m_to_value      = QVariant(to_value);
}

FilterCommand::FilterCommand(Mlt::Filter* filter, QString name,  int from_value, int to_value, QUndoCommand * parent)
: QUndoCommand(parent)
{
    m_filter = new Mlt::Filter(filter->get_filter());
    m_keyName   = name;
    m_from_value    = QVariant(from_value);
    m_to_value      = QVariant(to_value);
}

FilterCommand::FilterCommand(Mlt::Filter* filter, QString name,  QString from_value, QString to_value, QUndoCommand * parent)
: QUndoCommand(parent)
{
    m_filter = new Mlt::Filter(filter->get_filter());
    m_keyName   = name;
    m_from_value    = QVariant(from_value);
    m_to_value      = QVariant(to_value);
}

FilterCommand::FilterCommand(Mlt::Filter* filter, QString name,  QRectF from_value, QRectF to_value, QUndoCommand * parent)
: QUndoCommand(parent)
{
    m_filter = new Mlt::Filter(filter->get_filter());
    m_keyName   = name;
    m_from_value    = QVariant(from_value);
    m_to_value      = QVariant(to_value);
}

FilterCommand::~FilterCommand()
{
    delete m_filter;
}

void FilterCommand::notify()
{
    MLT.refreshConsumer();
    emit MAIN.filterController()->attachedModel()->changed();
    emit MAIN.timelineDock()->positionChanged();
}

void FilterCommand::set_value(QVariant value)
{
    QVariant::Type value_type = value.type();


    if(value_type == QVariant::Double)
    {
        m_filter->set(m_keyName.toUtf8().constData(), value.toDouble());
    }
    else if(value_type == QVariant::Int)
    {
         m_filter->set(m_keyName.toUtf8().constData(), value.toInt());
    }
    else if(value_type == QVariant::String)
    {
        m_filter->set(m_keyName.toUtf8().constData(), value.toString().toUtf8().constData());
    }
    else if(value_type == QVariant::RectF)
    {
        QRectF rectF = value.toRectF();

        m_filter->set(m_keyName.toUtf8().constData(), (double)rectF.left(), (double)rectF.top(), (double)rectF.width(), (double)rectF.height(), 1.0);
    }

}

void FilterCommand::redo()
{
    set_value(m_to_value);
    notify();
}

void FilterCommand::undo()
{
    set_value(m_from_value);
    notify();
}

/*
FilterClipCommand::FilterClipCommand(MultitrackModel& model, int trackIndex, int clipIndex, QString strFromXml, QString strToXml, QUndoCommand * parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_trackIndex(trackIndex)
    , m_clipIndex(clipIndex)
    , m_strFromXml(strFromXml)
    , m_strToXml(strToXml)
    , m_undoHelper(m_model)
{

   setText(QObject::tr("FilterClipCommand"));
}

void FilterClipCommand::redo()
{
    //m_undoHelper.recordBeforeState();
    m_model.refreshClipFromXmlForFilter(m_trackIndex, m_clipIndex, m_strToXml);

    //m_model.moveClip(m_fromTrackIndex, m_toTrackIndex, m_fromClipIndex, m_toStart);
    //m_undoHelper.recordAfterState();
    //qDebug()<<"redo ends";
}

void FilterClipCommand::undo()
{
   // LOG_DEBUG() << "fromTrack" << m_fromTrackIndex << "toTrack" << m_toTrackIndex;
   // m_undoHelper.undoChanges();
     m_model.refreshClipFromXmlForFilter(m_trackIndex, m_clipIndex, m_strFromXml);

}
*/
} // namespace

#include "moc_timelinecommands.cpp"
