#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAudioDeviceInfo>
#include<QMessageBox>
#include<QDebug>
static const int filter_preset_table[MainWindow::MODE_LAST][3][2] =
{   //     WIDE             NORMAL            NARROW
    {{      0,      0}, {     0,     0}, {     0,     0}},  // MODE_OFF
    {{ -15000,  15000}, { -5000,  5000}, { -1000,  1000}},  // MODE_RAW
    {{ -10000,  10000}, { -5000,  5000}, { -2500,  2500}},  // MODE_AM
    {{ -10000,  10000}, { -5000,  5000}, { -2500,  2500}},  // MODE_NFM
    {{-100000, 100000}, {-80000, 80000}, {-60000, 60000}},  // MODE_WFM_MONO
    {{-100000, 100000}, {-80000, 80000}, {-60000, 60000}},  // MODE_WFM_STEREO
    {{  -4000,   -100}, { -2800,  -100}, { -1600,  -200}},  // MODE_LSB
    {{    100,   4000}, {   100,  2800}, {   200,  1600}},  // MODE_USB
    {{  -1000,   1000}, {  -250,   250}, {  -100,   100}},  // MODE_CWL
    {{  -1000,   1000}, {  -250,   250}, {  -100,   100}},  // MODE_CWU
    {{-100000, 100000}, {-80000, 80000}, {-60000, 60000}}   // MODE_WFM_STEREO_OIRT
};
QStringList MainWindow::ModulationStrings;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* create dock widgets */
   // uiDockAudio = new DockAudio();
   // uiDockInputCtl = new DockInputCtl();
   // uiDockFft = new DockFft();
   // rx = new receiver("", "", 1);
   // rx->set_rf_freq(91000000.0f);
//    rx->set_filter_offset(0);

   // d_filter_shape = receiver::FILTER_SHAPE_NORMAL;

    /* create receiver object */

    // Filter preset table per mode, preset and lo/hi

    if (ModulationStrings.size() == 0)
    {
        // Keep in sync with rxopt_mode_idx
        ModulationStrings.append("Demod Off");
        ModulationStrings.append("Raw I/Q");
        ModulationStrings.append("AM");
        ModulationStrings.append("Narrow FM");
        ModulationStrings.append("WFM (mono)");
        ModulationStrings.append("WFM (stereo)");
        ModulationStrings.append("LSB");
        ModulationStrings.append("USB");
        ModulationStrings.append("CW-L");
        ModulationStrings.append("CW-U");
        ModulationStrings.append("WFM (oirt)");
    }
    ui->Mode_band_comboBox->addItems(ModulationStrings);
//    ioconfig::getDeviceList(devList,ui->Deveice_Input_comboBox, ArrayinputDevevice);
//    qDebug() << "numberInput."<<ArrayinputDevevice->size();

//     getDevieceOutputAudio(NameDevieceOutput);
//    // remote controller
//    remote = new RemoteControl();
    /* meter timer */
    meter_timer = new QTimer(this);
    connect(meter_timer, SIGNAL(timeout()), this, SLOT(meterTimeout()));

    //add Widget to mainwindow
  //  tabifyDockWidget(uiDockRxOpt, uiDockFft);
 //   connect(ui->Filter_WidthcomboBox, SIGNAL(activated(int)), this, SLOT(on_filterCombo_activated(int)));

    connect(ui->Volume_horizontalSlider,SIGNAL(valueChanged(int)),ui->Volume_value_Lable,SLOT(setNum(int)));
    Volume=ui->Volume_value_Lable->text().toInt();
    connect(ui->Frequency_Centrer_horizontalSlider,SIGNAL(valueChanged(int)),ui->Frequency_Centrer_value_lable,SLOT(setNum(int)));
    connect(ui->Frequency_Centrer_horizontalSlider,SIGNAL(valueChanged(int)),ui->Frequency_Centrer_value_spinBox,SLOT(setValue(int)));
    connect(ui->Frequency_Centrer_value_spinBox,SIGNAL(valueChanged(int)),this,SLOT(setNewFrequency(int)));
 //   connect(ui->Frequency_Centrer_horizontalSlider,SIGNAL(bool isLeftToRight()),ui->ReMote_label,SLOT(bool setEnabled()));
    //ui->Frequency_Centrer_horizontalSlider.isLeftToRight()
          //  ui->Deveice_Input_Audio_comboBox->addItems();
    connect(ui->Frequency_Centrer_value_spinBox,SIGNAL(valueChanged(int)),ui->Frequency_Centrer_horizontalSlider,SLOT(setValue(int)));
    connect(ui->Frequency_Centrer_horizontalSlider,SIGNAL(valueChanged(int)),ui->label_FrequencyCentrer_value,SLOT(setNum(int)));
   // connect(ui->Frequency_Centrer_horizontalSlider,SIGNAL(valueChanged(int)),rx,SLOT());

    connect(ui->Frequency_Centrer_unit_valuecomboBox,SIGNAL(currentIndexChanged(QString)),ui->label_Frequencycentrer_unit,SLOT(setText(QString)));
    connect(ui->Frequency_Centrer_unit_valuecomboBox,SIGNAL(currentIndexChanged(QString)),ui->label_Frequency_Unit_Slider,SLOT(setText(QString)));

    // ui->Frequency_Step_value_SpinBox->valueChanged()
    connect(ui->Frequency_Step_value_SpinBox,SIGNAL(valueChanged(int)),ui->label_Frequency_Step_Value_,SLOT(setNum(int)));
    connect(ui->AGC_Gain_value_verticalSlider,SIGNAL(valueChanged(int)),ui->AF_Gain_Value_Lable,SLOT(setNum(int)));
    connect(ui->APF_IF_value_verticalSlider,SIGNAL(valueChanged(int)),ui->APF_IF_Value_lable,SLOT(setNum(int)));
    connect(ui->Squech_value_verticalSlider_2,SIGNAL(valueChanged(int)),ui->Squech_value_lable,SLOT(setNum(int)));
    //connect(ui->Show_keyBoard_pushButton,SIGNAL(clicked()),SLOT(show()));
    connect(ui->Mode_band_comboBox,SIGNAL(currentIndexChanged(QString)),ui->Mode_band_label,SLOT(setText(QString)));
    connect(ui->Frequency_Step_unit_value_comboBox,SIGNAL(currentIndexChanged(QString)),ui->label_Frequency_Step_unit,SLOT(setText(QString)));
    connect(ui->decimCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setDecimation(int)));
    //connect(ui->NB_AFC_radioButton,SIGNAL(bool isChecked()),ui->Statut_Noise_Bank_label,SLOT(bool setEnabled()));

   // connect(remote, SIGNAL(newMode(int)), this, SLOT(selectDemod(int)));
    connect(ui->Mode_band_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectDemod(int)));
 //   connect(remote, SIGNAL(newPassband(int)), this, SLOT(setPassband(int)));
    connect(ui->BandWidth_value_SpinBox_2,SIGNAL(valueChanged(int)),this,SLOT(setPassband(int)));
    connect(ui->Squech_value_verticalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(setSquechvalue(int)));
    connect(ui->Deveice_Input_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setInputdeviece(int)));
    connect(this,SIGNAL(SampleRateInputValuechange(int)),this,SLOT(setinputSampleRate(int)));

    // setup for DockinputCtl
//    connect(uiDockInputCtl, SIGNAL(gainChanged(QString, double)), this, SLOT(setGain(QString,double)));
//    connect(uiDockInputCtl, SIGNAL(autoGainChanged(bool)), this, SLOT(setAutoGain(bool)));
//    connect(uiDockInputCtl, SIGNAL(freqCorrChanged(double)), this, SLOT(setFreqCoruechr(double)));
//    connect(uiDockInputCtl, SIGNAL(iqSwapChanged(bool)), this, SLOT(setIqSwap(bool)));
//    connect(uiDockInputCtl, SIGNAL(dcCancelChanged(bool)), this, SLOT(setDcCancel(bool)));
//    connect(uiDockInputCtl, SIGNAL(iqBalanceChanged(bool)), this, SLOT(setIqBalance(bool)));
//    connect(uiDockInputCtl, SIGNAL(antennaSelected(QString)), this, SLOT(setAntenna(QString)));
//    connect(uiDockInputCtl, SIGNAL(iqBalanceChanged(bool)), this, SLOT(setIqBalance(bool)));

    // Get list audio device to combobox
    connect(ui->Deveice_Output_Audio_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setAudioOutputDevice(int)));
//    connect(uiDockAudio, SIGNAL(audioStreamingStarted(QString,int)), this, SLOT(startAudioStream(QString,int)));
//    connect(uiDockAudio, SIGNAL(audioStreamingStopped()), this, SLOT(stopAudioStreaming()));
    // Set gain AGC, Squech
    connect(ui->Volume_horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(setAudioGain(int)));
    connect(ui->AGC_Gain_value_verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(setAgcGain(int)));
}
void MainWindow::getDevieceOutputAudio(QString Name){
unsigned int i=0;
int idx=ui->Deveice_Output_Audio_comboBox->currentIndex();
QString outdev;//=QString(outDevList[idx].get_name().c_str());
    // get list of audio output devices

//#ifdef WITH_PULSEAUDIO
//   pa_device_list devices;
//   outDevList = devices.get_output_devices();

//   qDebug() << __FUNCTION__ << ": Available output devices:";
//   for (i = 0; i < outDevList.size(); i++)
//   {
//       qDebug() << "   " << i << ":" << QString(outDevList[i].get_description().c_str());
//       //qDebug() << "     " << QString(outDevList[i].get_name().c_str());
//       ui->Deveice_Output_Audio_comboBox->addItem(QString(outDevList[i].get_description().c_str()));

//       // note that item #i in devlist will be item #(i+1)
//       // in combo box due to "default"
//       if (outdev == QString(outDevList[i].get_name().c_str()))
//           ui->Deveice_Output_Audio_comboBox->setCurrentIndex(i);
//   }
//#elif WITH_PORTAUDIO
//   portaudio_device_list   devices;

//   outDevList = devices.get_output_devices();
//   for (i = 0; i < outDevList.size(); i++)
//   {
//       ui->Deveice_Output_Audio_comboBox->addItem(QString(outDevList[i].get_description().c_str()));

//       // note that item #i in devlist will be item #(i+1)
//       // in combo box due to "default"
//       if (outdev == QString(outDevList[i].get_name().c_str()))
//           ui->Deveice_Output_Audio_comboBox->setCurrentIndex(i);
//   }
//   //ui->outDevCombo->setEditable(true);

//#elif defined(GQRX_OS_MACX)
//   osxaudio_device_list devices;
//   outDevList = devices.get_output_devices();

//   qDebug() << __FUNCTION__ << ": Available output devices:";
//   for (i = 0; i < outDevList.size(); i++)
//   {
//       qDebug() << "   " << i << ":" << QString(outDevList[i].get_name().c_str());
//       ui->Deveice_Output_Audio_comboBox->addItem(QString(outDevList[i].get_name().c_str()));

//       // note that item #i in devlist will be item #(i+1)
//       // in combo box due to "default"
//       if (outdev == QString(outDevList[i].get_name().c_str()))
//           ui->Deveice_Output_Audio_comboBox->setCurrentIndex(i);
//   }

//#else
//   ui->Deveice_Output_Audio_comboBox->addItem(settings->value("output/device", "Default").toString(),settings->value("output/device", "Default").toString());
//  // ui->Deveice_Output_Audio_comboBox->setEditable(true);
//#endif // WITH_PULSEAUDIO
getaudiooutput(ui->Deveice_Output_Audio_comboBox);

}
void MainWindow::getaudiooutput(QComboBox *combobox)
    {
    QAudioDeviceInfo device;
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for(int i = 0; i < devices.size(); ++i) {
    combobox->addItem(devices.at(i).deviceName());}

    }


void MainWindow::getSampleRateInput(){
    int Samplerate;
    int i=ui->Sample_Rate_Input_comboBox->currentIndex();
    switch (i) {
    case 0:
        Samplerate=48000;
        break;
    case 1:
        Samplerate=48000;

        break;
    case 2:
        Samplerate=96000;


        break;
    case 3:
        Samplerate=192000;

        break;
    case 4:
        Samplerate=8000000;

        break;
    case 5:
        Samplerate=10000000;

        break;
    case 6:
        Samplerate=12500000;

        break;
    case 7:
        Samplerate=16000000;

        break;
    case 8:
        Samplerate=20000000;

        break;
    default:
        Samplerate=48000;

        break;

    }
emit SampleRateInputValuechange(Samplerate);
   }
void MainWindow::setinputSampleRate(int SampleRateInput){
    double SampleRateInput_double=(double)SampleRateInput;
//    rx->set_input_rate(SampleRateInput_double);
//    qDebug() << "Configuration Sample Input :" << SampleRateInput_double;

}



MainWindow::~MainWindow()
{
    delete ui;
//    delete rx;
//    delete uiDockInputCtl;

}

void MainWindow::on_NB_AFC_radioButton_clicked()
{
    if(ui->NB_AFC_radioButton->isChecked())
    {
        ui->Statut_Noise_Bank_label->setEnabled(true);
        ui->Drop_label->setEnabled(true);
    }
    else
    {
        ui->Statut_Noise_Bank_label->setEnabled(false);
        ui->Drop_label->setEnabled(false);

    }
}

void MainWindow::on_AGC_radioButton_clicked()
{
    if(ui->AGC_radioButton->isChecked())
    {
        ui->Statut_AGC_label->setEnabled(true);
  //      rx->set_agc_on(true);
        ui->AGC_Gain_value_verticalSlider->setEnabled(true);
    }
    else
    {
        ui->Statut_AGC_label->setEnabled(false);
  //      rx->set_agc_on(false);
        ui->AGC_Gain_value_verticalSlider->setEnabled(false);
    }

}

void MainWindow::on_APF_IF_radioButton_clicked()
{

    if(ui->APF_IF_radioButton->isChecked())
    {
        ui->Statut_APF_label->setEnabled(true);
        ui->APF_IF_value_verticalSlider->setEnabled(true);
    }
    else
    {
        ui->Statut_APF_label->setEnabled(false);
        ui->APF_IF_value_verticalSlider->setEnabled(false);
    }
}

void MainWindow::on_Attenuation_10_radioButton_clicked()
{
    if(ui->Attenuation_10_radioButton->isChecked())
    {
        ui->Statut_Attenuation_10_label->setEnabled(true);
        ui->Statut_Attenuation_30_label->setEnabled(false);
    }
    else
    {
        ui->Statut_Attenuation_10_label->setEnabled(false);
    }
}

void MainWindow::on_Attenuation_30_radioButton_clicked()
{
    if(ui->Attenuation_30_radioButton->isChecked())
    {
        ui->Statut_Attenuation_30_label->setEnabled(true);
        ui->Statut_Attenuation_10_label->setEnabled(false);
    }
    else
    {
        ui->Statut_Attenuation_30_label->setEnabled(false);
    }
}

void MainWindow::on_pushButton_mute_sound_clicked()
{
    mute_sound=!mute_sound;
    if(mute_sound==false){
        ui->pushButton_mute_sound->setIcon(QIcon(":/sound_on.png"));
    }
    else {
        ui->pushButton_mute_sound->setIcon(QIcon(":/sound_off.png"));

    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::meterTimeout()
{
    float level;

   // level = rx->get_signal_pwr(true);
  //  ui->sMeter->setLevel(level);
   // remote->setSignalLevel(level);
}
void MainWindow::on_actionFFT_Setting_triggered()

{
   // addDockWidget(Qt::RightDockWidgetArea, uiDockFft);

}

void MainWindow::on_Start_pushButton_clicked()
{

   // checked=! checked;
    Start_HF=!Start_HF;
  //   rx->set_output_device("");
   // setFrequency_Center();

//    remote->setReceiverStatus(Start_HF);

    if (Start_HF)
    {


     //   rx->start();

        ui->Start_pushButton->setText("Stop");

    }
    else
    {
       // stop receiver
      //  rx->stop();
        ui->Start_pushButton->setText("Start");
    }
}
void MainWindow::setNewFrequency(int rx_freq_int)
{   int i=ui->Frequency_Centrer_unit_valuecomboBox->currentIndex();
    switch (i) {
    case 0:
        rx_freq_int=rx_freq_int*1000000;
        break;
    case 1:
        rx_freq_int=rx_freq_int*1000;
        break;
    }
  //  qint64 rx_freq=(qint64)rx_freq_int;
   // double hw_freq = (double)(rx_freq - d_lnb_lo) - rx->get_filter_offset();
    //qint64 center_freq = rx_freq - (qint64)rx->get_filter_offset();

   // d_hw_freq = (qint64)hw_freq;

    // set receiver frequency
//    rx->set_rf_freq(hw_freq);
//    qDebug() << "New Frequency:"<<hw_freq;

    // update widgets
}
void MainWindow::selectDemod(int mode_idx)
{
    double quad_rate;
    double cwofs = 700;
    float maxdev;
    //int filter_preset = uiDockRxOpt->currentFilter();
    int flo=0, fhi=0, click_res=100;
    bool rds_enabled;

    // validate mode_idx
    if (mode_idx < 2)
    {
      //  qDebug() << "Invalid mode index:" << mode_idx;
        mode_idx = 1;
    }
    //qDebug() << "New mode index:" << mode_idx;

 //   uiDockRxOpt->getFilterPreset(mode_idx, filter_preset, &flo, &fhi);
  //  d_filter_shape = (receiver::filter_shape)ui->filter_shape_comboBox->currentIndex();

    //rds_enabled = rx->is_rds_decoder_active();
  //  if (rds_enabled)
   //    setRdsDecoder(false);
   // uiDockRDS->setDisabled();

    switch (mode_idx) {

    case 1:// NOt Mode
        /* Spectrum analyzer only */
//        if (rx->is_recording_audio())
//        {
//       //     stopAudioRec();
//    //        uiDockAudio->setAudioRecButtonState(false);
//        }

//        rx->set_demod(receiver::RX_DEMOD_OFF);
//       // rx->set_
//        click_res = 1000;
//        qDebug() << "New mode index:" << receiver::RX_DEMOD_OFF;
       break;

    case 2://Mode RAW
        /* Raw I/Q */
//        rx->set_demod(receiver::RX_DEMOD_NONE);
//     //   ui->plotter->setDemodRanges(-45000, -200, 200, 45000, true);
//        uiDockAudio->setFftRange(0,24000);
//        click_res = 100;
//        qDebug() << "New mode index:" << receiver::RX_DEMOD_NONE;
        break;

    case 3://DockRxOpt::MODE_AM
//        rx->set_demod(receiver::RX_DEMOD_AM);
//      //  ui->plotter->setDemodRanges(-45000, -200, 200, 45000, true);
//        uiDockAudio->setFftRange(0,6000);
//        click_res = 100;
//        qDebug() << "New mode index:" << receiver::RX_DEMOD_AM;
        break;

    case 4://DockRxOpt::MODE_NFM
//        rx->set_demod(receiver::RX_DEMOD_NFM);
//        click_res = 100;
//        qDebug() << "New mode index:" << receiver::RX_DEMOD_NFM;
        /*maxdev = uiDockRxOpt->currentMaxdev();
        if (maxdev < 20000.0)
        {   // FIXME
            ui->plotter->setDemodRanges(-45000, -250, 250, 45000, true);
            uiDockAudio->setFftRange(0,6000);
        }
        else
        {
            ui->plotter->setDemodRanges(-45000, -1000, 1000, 45000, true);
            uiDockAudio->setFftRange(0,24000);
        }*/
        break;

    case 5://DockRxOpt::MODE_WFM_MONO:
//        rx->set_demod(receiver::RX_DEMOD_WFM_M);
//        qDebug() << "New mode index:" << receiver::RX_DEMOD_WFM_M;
        break;

  //  case DockRxOpt::MODE_WFM_STEREO:
    case 6://DockRxOpt::MODE_WFM_STEREO_OIRT
//        rx->set_demod(receiver::RX_DEMOD_WFM_S);
//        qDebug() << "New mode index:" << receiver::RX_DEMOD_WFM_S;


        /* Broadcast FM */
//        quad_rate = rx->get_input_rate();
        break;

     /*   if (quad_rate < 500.0e3)
            ui->plotter->setDemodRanges(-quad_rate/2.0, -10000, 10000, quad_rate/2.0, true);
        else
            ui->plotter->setDemodRanges(-250000, -10000, 10000, 250000, true);
        uiDockAudio->setFftRange(0,24000);  /** FIXME: get audio rate from rx *
        click_res = 1000;*/
       case 7:
//        rx->set_demod(receiver::RX_DEMOD_WFM_S_OIRT);
//        qDebug() << "New mode index:" << receiver::RX_DEMOD_WFM_S_OIRT;

       // uiDockRDS->setEnabled();
    //    if (rds_enabled)
    //        setRdsDecoder(true);
        break;

    case 8:
        /* LSB */
//        rx->set_demod(receiver::RX_DEMOD_SSB);
//        qDebug() << "New mode index:" << receiver::RX_DEMOD_SSB;
//     //   ui->plotter->setDemodRanges(-40000, -100, -5000, 0, false);
//        uiDockAudio->setFftRange(0,3000);
        click_res = 100;
        break;

    case 9:
        /* USB */
//        rx->set_demod(receiver::RX_DEMOD_SSB);
//   //     ui->plotter->setDemodRanges(0, 5000, 100, 40000, false);
//        uiDockAudio->setFftRange(0,3000);
        click_res = 100;
        break;

    case 10://DockRxOpt::MODE_CWL
        /* CW-L */
//        rx->set_demod(receiver::RX_DEMOD_SSB);
//     //   cwofs = -uiDockRxOpt->getCwOffset();
//    //    ui->plotter->setDemodRanges(-5000, -100, 100, 5000, true);
//        uiDockAudio->setFftRange(0,1500);
        click_res = 10;
        break;

    case 11://DockRxOpt::MODE_CWU
        /* CW-U */
//        rx->set_demod(receiver::RX_DEMOD_SSB);
//   //     cwofs = uiDockRxOpt->getCwOffset();
//     //   ui->plotter->setDemodRanges(-5000, -100, 100, 5000, true);
//        uiDockAudio->setFftRange(0,1500);
        click_res = 10;
        break;

    default:
       // qDebug() << "Unsupported mode selection (can't happen!): " << mode_idx;
        flo = -5000;
        fhi = 5000;
        click_res = 100;
        break;
    }

  //  qDebug() << "Filter preset for mode" << mode_idx << "LO:" << flo << "HI:" << fhi;
 //   ui->plotter->setHiLowCutFrequencies(flo, fhi);
  //  ui->plotter->setClickResolution(click_res);
   // ui->plotter->setFilterClickResolution(click_res);
   // rx->set_filter((double)flo, (double)fhi, d_filter_shape);
//    rx->set_cw_offset(cwofs);
//    rx->set_sql_level((double)ui->Squech_value_verticalSlider_2->value());

//    remote->setMode(mode_idx);
//    remote->setPassband(flo, fhi);

  //  d_have_audio = (mode_idx != DockRxOpt::MODE_OFF);

   // uiDockRxOpt->setCurrentDemod(mode_idx);
}

void MainWindow::on_Mode_band_comboBox_currentIndexChanged(int index)
{
    index=ui->Mode_band_comboBox->currentIndex();
   // qDebug<<"Mode Index:"<<""+index;
}
/* Get filter index from filter LO / HI values.
* @param lo The filter low cut frequency.
* @param hi The filter high cut frequency.
*
* Given filter low and high cut frequencies, this function checks whether the
* filter settings correspond to one of the presets in filter_preset_table and
* returns the corresponding index to ui->filterCombo;
*/
unsigned int MainWindow::filterIdxFromLoHi(int lo, int hi) const
{
   int mode_index = ui->Mode_band_comboBox->currentIndex();

   if (lo == filter_preset_table[mode_index][FILTER_PRESET_WIDE][0] &&
       hi == filter_preset_table[mode_index][FILTER_PRESET_WIDE][1])
       return FILTER_PRESET_WIDE;
   else if (lo == filter_preset_table[mode_index][FILTER_PRESET_NORMAL][0] &&
            hi == filter_preset_table[mode_index][FILTER_PRESET_NORMAL][1])
       return FILTER_PRESET_NORMAL;
   else if (lo == filter_preset_table[mode_index][FILTER_PRESET_NARROW][0] &&
            hi == filter_preset_table[mode_index][FILTER_PRESET_NARROW][1])
       return FILTER_PRESET_NARROW;

   return FILTER_PRESET_USER;
}



void MainWindow::on_comboBox_activated(int index)
{
    Q_UNUSED(index);

  //  qDebug() << "New filter preset:" << ui->Filter_WidthcomboBox->currentText();
  //  qDebug() << "            shape:" << ui->filterShapeCombo->currentIndex();
    //emit demodSelected(ui->modeSelector->currentIndex());
}
void MainWindow::setFilterParam(int lo, int hi)
{
    int filter_index = filterIdxFromLoHi(lo, hi);

    ui->Filter_WidthcomboBox->setCurrentIndex(filter_index);
    if (filter_index == FILTER_PRESET_USER)
    {
        float width_f;
        //width_f = fabs((hi-lo)/1000.f);
        ui->Filter_WidthcomboBox->setItemText(FILTER_PRESET_USER, QString("User (%1 k)").arg(width_f));
    }
}
void MainWindow::on_plotter_newFilterFreq(int low, int high)
{
//    receiver::status retcode;

//    /* parameter correctness will be checked in receiver class */
//    retcode = rx->set_filter((double) low, (double) high, d_filter_shape);

//    /* Update filter range of plotter, in case this slot is triggered by
//     * switching to a bookmark */
//  //  ui->plotter->setHiLowCutFrequencies(low, high);

//    if (retcode == receiver::STATUS_OK)
        setFilterParam(low, high);
}
void MainWindow::setPassband(int bandwidth)
{
    /* Check if filter is symmetric or not by checking the presets */
    int mode = ui->Mode_band_comboBox->currentIndex();
    int preset = ui->filter_shape_comboBox->currentIndex();
    int indexunit=ui->bandWidth_unit_comboBox_2->currentIndex();
    switch (indexunit) {
    case 1:
        bandwidth=bandwidth*1000;
        break;
    case 2:
        bandwidth=bandwidth*1000000;
        break;
    default:
        bandwidth=1000000;
        break;
    }

    int lo, hi;
    getFilterPreset(mode, preset, &lo, &hi);

    if(lo + hi == 0)
    {
        lo = -bandwidth / 2;
        hi =  bandwidth / 2;
    }
    else if(lo >= 0 && hi >= 0)
    {
        hi = lo + bandwidth;
    }
    else if(lo <= 0 && hi <= 0)
    {
        lo = hi - bandwidth;
    }
//    rx->set_analog_bandwidth(bandwidth);
//     qDebug() << __func__ << "New Bandwidth" << bandwidth;
//    remote->setPassband(lo, hi);

    on_plotter_newFilterFreq(lo, hi);
}
void MainWindow::getFilterPreset(int mode, int preset, int * lo, int * hi) const
{
    if (mode < 0 || mode >= MODE_LAST)
    {
      // qDebug() << __func__ << ": Invalid mode:" << mode;
        mode = MODE_AM;
    }
    else if (preset < 1 || preset > 3)
    {
     //   qDebug() << __func__ << ": Invalid preset:" << preset;
        preset = FILTER_PRESET_NORMAL;
    }
    *lo = filter_preset_table[mode][preset][0];
    *hi = filter_preset_table[mode][preset][1];
}
void MainWindow::setSquechvalue(int SquechValue){
  //  QSettings *settings;
    SquechValue=ui->Squech_value_verticalSlider_2->value();
    // note: dBFS*10 as int


    double sql_lvl =(double) SquechValue;
//    qDebug() << __func__ << ": Squech Value:" << sql_lvl;
//    rx->set_sql_level(sql_lvl);
    //qDebug() << __func__ << ": Squech Value:" << sql_lvl;
  //  if (sql_lvl > -150.0)

   //     settings->setValue("receiver/sql_level", sql_lvl);
   // else
    //   settings->remove("receiver/sql_level");
}
#define DATA_BUFFER_SIZE 48000
void MainWindow::decoderTimeout()
{

    float buffer[DATA_BUFFER_SIZE];
    unsigned int num;

 //   rx->get_sniffer_data(&buffer[0], num);
        process_samples(&buffer[0], num);
}
void MainWindow::on_Recorder_radioButton_toggled(bool checked)
{

    if (checked == true)
    {
      //  qDebug() << "Starting RDS decoder.";
      //  uiDockRDS->showEnabled();
    //    rx->start_rds_decoder();
     //   rx->reset_rds_parser();
       //rds_timer->start(250);
    }
    else
    {
       // qDebug() << "Stopping RDS decoder.";
    //    uiDockRDS->showDisabled();
    //    rx->stop_rds_decoder();
      //  rds_timer->stop();
    }
}
/*! \brief Process new set of samples. */
void MainWindow::process_samples(float *buffer, int length)
{
    int overlap = 18;
    int i;

    for (i = 0; i < length; i++) {
        tmpbuf.append(buffer[i]);
    }

   // decoder->demod(tmpbuf.data(), length);

    /* clear tmpbuf and store "overlap" */
    tmpbuf.clear();
    for (i = length-overlap; i < length; i++) {
        tmpbuf.append(buffer[i]);
    }

}

void MainWindow::on_Deveice_Output_Audio_comboBox_currentIndexChanged(const QString &arg1)
{
   NameDevieceOutput=ui->Deveice_Output_Audio_comboBox->currentText();
    //qDebug() << "Set audio output" << NameDevieceOutput;
}
void MainWindow::setFrequency_Center(){
    int Unit_number=ui->Frequency_Centrer_unit_valuecomboBox->currentIndex();
    switch (Unit_number) {
    case 0:
        Frequency_centrer_value=ui->Frequency_Centrer_horizontalSlider->value()* 1000000.00;
   //     qDebug() << "Set Frequency Centrer" << Frequency_centrer_value;

        break;
    case 1:
        Frequency_centrer_value=ui->Frequency_Centrer_horizontalSlider->value()*1000.00;
     //   qDebug() << "Set Frequency Centrer" << Frequency_centrer_value;
        break;
    default:
        break;
    }
}

void MainWindow::on_Frequency_Centrer_value_spinBox_valueChanged(int arg1)
{
    setFrequency_Center();
}

void MainWindow::on_Frequency_Step_value_SpinBox_valueChanged(int arg1)
{ int number=ui->Frequency_Step_unit_value_comboBox->currentIndex();
    switch (number) {
    case 0:
        Frequency_Step=ui->Frequency_Step_value_SpinBox->value()*1000.0;
      //  qDebug() << "Set Frequency Centrer Step" << Frequency_Step;

        break;
    case 1:
        Frequency_Step=ui->Frequency_Step_value_SpinBox->value()*1000000.0;
    //    qDebug() << "Set Frequency Centrer Step" << Frequency_Step;

        break;
    default:
        break;
    }


}
void MainWindow::on_BandWidth_value_SpinBox_2_valueChanged(int arg1)
{
    int number=ui->bandWidth_unit_comboBox_2->currentIndex();
        switch (number) {
        case 0:
            Frequency_bandwith=ui->BandWidth_value_SpinBox_2->value()*1000.0;
        //    qDebug() << "Set Band Width Frequency" << Frequency_bandwith;

            break;
        case 1:
            Frequency_bandwith=ui->BandWidth_value_SpinBox_2->value()*1000000.0;
       //     qDebug() << "Set Band Width Frequency" << Frequency_bandwith;
            break;
        default:
            break;
        }
}
void MainWindow::setInputdeviece(int index)
{  // insert the device list in device combo box
int i=ui->Deveice_Input_comboBox->currentIndex();

       DeveiceInput=ArrayinputDevevice->at(i-1);
          qDebug() << "Configure I/O devices."<<ArrayinputDevevice->at(i-1);

       //rx->set_input_device(DeveiceInput.toStdString());
   //    std::vector<std::string> antennas = rx->get_antennas();
      // uiDockInputCtl->setAntennas(antennas);
    //   rx->set_antenna(antennas);

}


void MainWindow::on_Sample_Rate_Input_comboBox_currentIndexChanged(int index)
{
    getSampleRateInput();
}
void MainWindow::setAudioGain(int valueint)
{
   float value=(float)valueint;
    //rx->set_af_gain(value);
 //   qDebug() << "Configure Audio Gain."<<value;

}
void MainWindow::SetupforDockInputCtl(){
    // Add available antenna connectors to the UI
//    std::vector<std::string> antennas = rx->get_antennas();
//    uiDockInputCtl->setAntennas(antennas);

    updateGainStages(true);

}
void MainWindow::updateGainStages(bool read_from_device)
{
  //  gain_list_t gain_list;
    //std::vector<std::string> gain_names = rx->get_gain_names();
 //   gain_t gain;

    std::vector<std::string>::iterator it;
//    for (it = gain_names.begin(); it != gain_names.end(); ++it)
//    {
//        gain.name = *it;
//        rx->get_gain_range(gain.name, &gain.start, &gain.stop, &gain.step);
//        if (read_from_device)
//        {
//            gain.value = rx->get_gain(gain.name);
//        }
//        else
//        {
//            gain.value = gain.stop;
//            rx->set_gain(gain.name, gain.value);
//     //       qDebug() << "Set Gain Stages"<<gain.name;
//         //   qDebug() <<":"<<gain.name;
//            qDebug() <<"SetGain Stages"<<gain.value;


//        }
//        gain_list.push_back(gain);
//    }

//    uiDockInputCtl->setGainStages(gain_list);
}
void MainWindow::setGain(QString name, double gain)
{
   // rx->set_gain(name.toStdString(), gain);
}
void MainWindow::setAutoGain(bool enabled)
{
   // rx->set_auto_gain(enabled);

}

/** Enable/disable I/Q reversion. */
void MainWindow::setIqSwap(bool reversed)
{
 //   rx->set_iq_swap(reversed);
}
/** Enable/disable automatic DC removal. */
void MainWindow::setDcCancel(bool enabled)
{
 //   rx->set_dc_cancel(enabled);
}

/** Enable/disable automatic IQ balance. */
void MainWindow::setIqBalance(bool enabled)
{
  //  rx->set_iq_balance(enabled);
}
/** Select new antenna connector. */
void MainWindow::setAntenna(const QString antenna)
{
   // qDebug() << "New antenna selected:" << antenna;
  //  std::vector<std::string> antennas = rx->get_antennas();
   // uiDockInputCtl->setAntennas(antennas);
   // rx->set_antenna(antenna.toStdString());
}



void MainWindow::on_reset_pushButton_clicked()
{
    SetupforDockInputCtl();
}

void MainWindow::on_Show_dockinput_control_radioButton_2_toggled(bool checked)
{
  //  ui->verticalLayout_widget4->addWidget(uiDockInputCtl,1,Qt::AlignCenter);

}
void MainWindow::setDecimation(int Decimation_index){
    unsigned int Decimation;
    switch (Decimation_index) {
    case 0:
            Decimation=2;
        break;
    case 1:
            Decimation=2;

        break;

    case 2:
            Decimation=4;

        break;

    case 3:
            Decimation=8;
        break;

    case 4:
            Decimation=16;

        break;

    case 5:
            Decimation=32;

        break;

    case 6:
            Decimation=64;

        break;

    case 7:
            Decimation=128;

        break;

    case 8:
            Decimation=254;

        break;

    case 9:
            Decimation=512;

        break;


    }
  //  rx->set_input_decim(Decimation);
   // qDebug() << "New Decimation" << Decimation;

}

void MainWindow::on_Show_dockinput_control_checkBox_toggled(bool checked)
{
    if(checked==true)
    {
        //addDockWidget(Qt::RightDockWidgetArea, uiDockInputCtl);
     //   uiDockInputCtl->show();

    }
    else {
       // removeDockWidget(uiDockInputCtl);
       // uiDockInputCtl->hide();

    }
}
void MainWindow::setAudioOutputDevice(int index){
    if(index==0 || index==1)
    {
     //   rx->set_output_device("");
    }
    else if (index>=2) {
     //   rx->set_output_device(ui->Deveice_Output_Audio_comboBox->currentText().toStdString());
    }
    ui->Deveice_Output_Audio_comboBox->setDisabled(true);
}
/** Start streaming audio over UDP. */
void MainWindow::startAudioStream(const QString udp_host, int udp_port)
{
  //  rx->start_udp_streaming(udp_host.toStdString(), udp_port);
}

/** Stop streaming audio over UDP. */
void MainWindow::stopAudioStreaming()
{
   // rx->stop_udp_streaming();
}
/** AGC manual gain changed. */
void MainWindow::setAgcGain(int gain)
{
   // rx->set_agc_manual_gain(gain);
}
