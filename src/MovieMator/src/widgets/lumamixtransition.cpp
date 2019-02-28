/*
 * Copyright (c) 2014-2016 Meltytech, LLC
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

#include "lumamixtransition.h"
#include "ui_lumamixtransition.h"
#include "settings.h"
#include "mltcontroller.h"
#include "mainwindow.h"
#include "util.h"
#include <QFileDialog>
#include <QFileInfo>
#include <Logger.h>
#include "timespinbox.h"

static const int kLumaComboDissolveIndex = 0;
static const int kLumaComboCutIndex = 1;
static const int kLumaComboCustomIndex = 24;

LumaMixTransition::LumaMixTransition(Mlt::Producer &producer, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LumaMixTransition)
    , m_producer(producer)
    , m_previewMovie(0)
{
    ui->setupUi(this);
//    Util::setColorsToHighlight(ui->label_2);

    QScopedPointer<Mlt::Transition> transition(getTransition("luma"));
    if (transition && transition->is_valid()) {
        QString resource = transition->get("resource");
        if (!resource.isEmpty() && resource.indexOf("%luma") != -1) {
            ui->lumaCombo->setCurrentIndex(resource.midRef(resource.indexOf("%luma") + 5).left(2).toInt() + 1);
        } else if (!resource.isEmpty() && resource.startsWith("color:")) {
            ui->lumaCombo->setCurrentIndex(kLumaComboCutIndex);
            ui->softnessLabel->setText(tr("Position"));
            ui->softnessSlider->setValue(qRound(QColor(resource.mid(6)).redF() * 100.0));
            ui->invertCheckBox->setDisabled(true);
        } else if (!resource.isEmpty()) {
            ui->lumaCombo->setCurrentIndex(kLumaComboCustomIndex);
        } else {
            ui->invertCheckBox->setDisabled(true);
            ui->softnessSlider->setDisabled(true);
            ui->softnessSpinner->setDisabled(true);
        }
        ui->invertCheckBox->setChecked(transition->get_int("invert"));
        if (transition->get("softness") && !resource.startsWith("color:"))
            ui->softnessSlider->setValue(qRound(transition->get_double("softness") * 100.0));
        updateCustomLumaLabel(*transition);
    }
    transition.reset(getTransition("mix"));
    if (transition && transition->is_valid()) {
        if (transition->get_int("start") == -1) {
            ui->crossfadeRadioButton->setChecked(true);
            ui->mixSlider->setDisabled(true);
            ui->mixSpinner->setDisabled(true);
        } else {
            ui->mixRadioButton->setChecked(true);
        }
        ui->mixSlider->setValue(qRound(transition->get_double("start") * 100.0));
    }

    m_durationSpinBox = new TimeSpinBox(this);
    m_durationSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_durationSpinBox->setToolTip(tr("Transition Duration"));
    m_durationSpinBox->setEnabled(true);
    m_durationSpinBox->setKeyboardTracking(false);
    ui->horizontalLayout_5->addWidget(m_durationSpinBox);
//    ui->horizontalLayout_5->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    m_durationSpinBox->blockSignals(true);
    m_durationSpinBox->setValue(m_producer.get_length());
    m_durationSpinBox->blockSignals(false);

    ui->label_2->setStyleSheet("background-color:rgb(82,82,82);");
    ui->line->setStyleSheet("color:black;");
    ui->line_2->setStyleSheet("color:black;");
    QString style = "background-color:rgb(82,82,82);color:rgb(225,225,225);";
    ui->lumaCombo->setStyleSheet(style);
    ui->softnessSpinner->setStyleSheet(style);
    ui->mixSpinner->setStyleSheet(style);
    ui->invertCheckBox->setStyleSheet("QCheckBox::indicator:unchecked{border-radius:3px;" + style + "}");
    m_durationSpinBox->setStyleSheet(style);
    QString strStyle = "QSlider::sub-page:horizontal{background:rgb(15,114,103);border-radius: 4px}";
    strStyle.append("QSlider::groove:horizontal{background:'grey';height:4px;border-radius: 4px;}");
    strStyle.append("QSlider::handle:horizontal{background:'white';width:5px;height:15px;margin: -5px 0px -5px 0px;border-radius: 2px}");
    ui->softnessSlider->setStyleSheet(strStyle);
    ui->mixSlider->setStyleSheet(strStyle);


    connect(m_durationSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(setTransitionDuration(int)));
//    connect(m_durationSpinBox, SIGNAL(editingFinished()), this, SLOT(setTransitionDuration(int)));

    m_previewMovie = new QMovie();

    m_previewMovie->setFileName( QString(":/transitions/%1.gif").arg(ui->lumaCombo->currentIndex()) );
    ui->label_preview->setMovie(m_previewMovie);
    m_previewMovie->start();

#if MOVIEMATOR_FREE
    for (int i = 1; i < ui->lumaCombo->count(); i++)
    {
        ui->lumaCombo->setItemData(i, QBrush(Qt::gray), Qt::TextColorRole);
    }
#endif

}

LumaMixTransition::~LumaMixTransition()
{
    delete ui;
    delete m_previewMovie;
}

void LumaMixTransition::on_invertCheckBox_clicked(bool checked)
{
    QScopedPointer<Mlt::Transition> transition(getTransition("luma"));
    if (transition && transition->is_valid()) {
        transition->set("invert", checked);
        MLT.refreshConsumer();
    }
}

void LumaMixTransition::on_softnessSlider_valueChanged(int value)
{
    QScopedPointer<Mlt::Transition> transition(getTransition("luma"));
    if (transition && transition->is_valid()) {
        if (kLumaComboCutIndex == ui->lumaCombo->currentIndex()) {
            qreal r = qreal(value) / 100.0;
            QColor color = QColor::fromRgbF(r, r, r);
            QString resource = QString("color:%1").arg(color.name());
            transition->set("resource", resource.toLatin1().constData());
        } else {
            transition->set("softness", value / 100.0);
        }
        MLT.refreshConsumer();
    }
}

void LumaMixTransition::on_crossfadeRadioButton_clicked()
{
    QScopedPointer<Mlt::Transition> transition(getTransition("mix"));
    if (transition && transition->is_valid()) {
        transition->set("start", -1);
    }
    ui->mixSlider->setDisabled(true);
    ui->mixSpinner->setDisabled(true);
}

void LumaMixTransition::on_mixRadioButton_clicked()
{
    QScopedPointer<Mlt::Transition> transition(getTransition("mix"));
    if (transition && transition->is_valid()) {
        transition->set("start", ui->mixSlider->value() / 100.0);
    }
    ui->mixSlider->setEnabled(true);
    ui->mixSpinner->setEnabled(true);
}

void LumaMixTransition::on_mixSlider_valueChanged(int value)
{
    QScopedPointer<Mlt::Transition> transition(getTransition("mix"));
    if (transition && transition->is_valid()) {
        transition->set("start", value / 100.0);
    }
}

Mlt::Transition *LumaMixTransition::getTransition(const QString &name)
{
    QScopedPointer<Mlt::Service> service(m_producer.producer());
    while (service && service->is_valid()) {
        if (service->type() == transition_type) {
            Mlt::Transition transition(*service);
            if (name == transition.get("mlt_service"))
                return new Mlt::Transition(transition);
            else if (name == "luma" && QString("movit.luma_mix") == transition.get("mlt_service"))
                return new Mlt::Transition(transition);
        }
        service.reset(service->producer());
    }
    return 0;
}

void LumaMixTransition::updateCustomLumaLabel(Mlt::Transition &transition)
{
    QString resource = transition.get("resource");
    if (resource.isEmpty() || resource.indexOf("%luma") != -1 || resource.startsWith("color:")) {
        ui->customLumaLabel->hide();
        ui->customLumaLabel->setToolTip(QString());
    } else if (!resource.isEmpty() && !resource.startsWith("color:")) {
        ui->customLumaLabel->setText(QFileInfo(transition.get("resource")).fileName());
        ui->customLumaLabel->setToolTip(transition.get("resource"));
        ui->customLumaLabel->show();
    }
}

void LumaMixTransition::on_lumaCombo_activated(int index)
{
    if (index == kLumaComboDissolveIndex || index == kLumaComboCutIndex) {
        on_invertCheckBox_clicked(false);
        ui->invertCheckBox->setChecked(false);
    }
    ui->invertCheckBox->setEnabled( index != kLumaComboDissolveIndex && index != kLumaComboCutIndex);
    ui->softnessSlider->setEnabled( index != kLumaComboDissolveIndex);
    ui->softnessSpinner->setEnabled(index != kLumaComboDissolveIndex);


    m_previewMovie->stop();
    m_previewMovie->setFileName( QString(":/transitions/%1.gif").arg(ui->lumaCombo->currentIndex()) );
    m_previewMovie->start();

#if MOVIEMATOR_FREE
    if (index != kLumaComboDissolveIndex)
    {
        //专业版功能，提示升级
        MAIN.showProFeaturePromptDialog();
        return;
    }
#endif

    QScopedPointer<Mlt::Transition> transition(getTransition("luma"));
    if (transition && transition->is_valid()) {
        if (index == kLumaComboDissolveIndex) {
            transition->set("resource", "");
            ui->softnessLabel->setText(tr("Softness"));
        } else if (index == kLumaComboCutIndex) { // Cut
            ui->softnessLabel->setText(tr("Position"));
            ui->softnessSlider->setValue(50);
        } else if (index == kLumaComboCustomIndex) {
            ui->softnessLabel->setText(tr("Softness"));
            // Custom file
            QString path = Settings.openPath();
#ifdef Q_OS_MAC
            path.append("/*");
#endif
            QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), path,
                tr("Images (*.bmp *.jpeg *.jpg *.pgm *.png *.svg *.tga *.tif *.tiff);;All Files (*)"));
            activateWindow();
            if (!filename.isEmpty()) {
                transition->set("resource", filename.toUtf8().constData());
                MLT.getHash(*transition);
            }
        } else {
            ui->softnessLabel->setText(tr("Softness"));
            transition->set("resource", QString("%luma%1.pgm").arg(index - 1, 2, 10, QChar('0')).toLatin1().constData());
        }
        if (qstrcmp(transition->get("resource"), "")) {
            transition->set("progressive", 1);
            if (index == kLumaComboCutIndex) {
                transition->set("invert", 0);
                transition->set("softness", 0);
            } else {
                transition->set("invert", ui->invertCheckBox->isChecked());
                transition->set("softness", ui->softnessSlider->value() / 100.0);
            }
        }
        updateCustomLumaLabel(*transition);
        MLT.refreshConsumer();
    }
}

//void::LumaMixTransition::on_lumaCombo_currentIndexChanged(int index)
//{
//    if (index == kLumaComboDissolveIndex || index == kLumaComboCutIndex) {
//        on_invertCheckBox_clicked(false);
//        ui->invertCheckBox->setChecked(false);
//    }
//    ui->invertCheckBox->setEnabled( index != kLumaComboDissolveIndex && index != kLumaComboCutIndex);
//    ui->softnessSlider->setEnabled( index != kLumaComboDissolveIndex);
//    ui->softnessSpinner->setEnabled(index != kLumaComboDissolveIndex);

//    QScopedPointer<Mlt::Transition> transition(getTransition("luma"));
//    if (transition && transition->is_valid()) {
//        if (index == kLumaComboDissolveIndex) {
//            transition->set("resource", "");
//            ui->softnessLabel->setText(tr("Softness"));
//        } else if (index == kLumaComboCutIndex) { // Cut
//            ui->softnessLabel->setText(tr("Position"));
//            ui->softnessSlider->setValue(50);
//        } else if (index == kLumaComboCustomIndex) {
//            ui->softnessLabel->setText(tr("Softness"));
//            // Custom file
//            QString path = Settings.openPath();
//#ifdef Q_OS_MAC
//            path.append("/*");
//#endif
//            QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), path,
//                tr("Images (*.bmp *.jpeg *.jpg *.pgm *.png *.svg *.tga *.tif *.tiff);;All Files (*)"));
//            activateWindow();
//            if (!filename.isEmpty()) {
//                transition->set("resource", filename.toUtf8().constData());
//                MLT.getHash(*transition);
//            }
//        } else {
//            ui->softnessLabel->setText(tr("Softness"));
//            transition->set("resource", QString("%luma%1.pgm").arg(index - 1, 2, 10, QChar('0')).toLatin1().constData());
//        }
//        if (qstrcmp(transition->get("resource"), "")) {
//            transition->set("progressive", 1);
//            if (index == kLumaComboCutIndex) {
//                transition->set("invert", 0);
//                transition->set("softness", 0);
//            } else {
//                transition->set("invert", ui->invertCheckBox->isChecked());
//                transition->set("softness", ui->softnessSlider->value() / 100.0);
//            }
//        }
//        updateCustomLumaLabel(*transition);
//        MLT.refreshConsumer();
//    }
//}
