
#ifndef IOCONFIG_H
#define IOCONFIG_H

#include <QDialog>
#include <QSettings>
#include <QString>
#include<ui_ioconfig.h>
#ifdef WITH_PULSEAUDIO
#include "pulseaudio/pa_device_list.h"
#elif WITH_PORTAUDIO
#include "portaudio/device_list.h"
#elif defined(GQRX_OS_MACX)
#include "osxaudio/device_list.h"
#endif
#include <QDialog>

/*namespace Ui {
class ioconfig;
}

class ioconfig : public QDialog
{
    Q_OBJECT

public:
    explicit ioconfig(QWidget *parent = 0);
    ~ioconfig();

private:
    Ui::ioconfig *ui;
};*/
/* -*- c++ -*- */
/*
 * Gqrx SDR: Software defined radio receiver powered by GNU Radio and Qt
 *           http://gqrx.dk/
 *
 * Copyright 2011-2014 Alexandru Csete OZ9AEC.
 *
 * Gqrx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gqrx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gqrx; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */



namespace Ui {
    class ioconfig;
}

/** @brief Inout/output device configurator. */
class ioconfig : public QDialog
{
    Q_OBJECT

public:
    explicit ioconfig(QSettings *settings, std::map<QString, QVariant> &devList, QWidget *parent = 0);
    virtual ~ioconfig();
    static void getDeviceList(std::map<QString, QVariant> &devList,QComboBox *combobox,QStringList *ArrayListDevieceInput);

private slots:
    void saveConfig();
    void inputDeviceSelected(int index);
    void inputDevstrChanged(const QString &text);
    void inputRateChanged(const QString &text);
    void decimationChanged(int index);

private:
    void updateInputSampleRates(int rate);
    void updateDecimations(void);
    int  idx2decim(int idx) const;
    int  decim2idx(int decim) const;

private:
    Ui::ioconfig  *ui;
    QSettings      *m_settings;

#ifdef WITH_PULSEAUDIO
    vector<pa_device>           outDevList;
#elif WITH_PORTAUDIO
    vector<portaudio_device>    outDevList;
#elif defined(GQRX_OS_MACX)
    vector<osxaudio_device>     outDevList;
#endif

};

#endif // IOCONFIG_H
