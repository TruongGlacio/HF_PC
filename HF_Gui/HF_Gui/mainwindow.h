#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include<remote_control.h>
//#include<receiver.h>
//#include<qtgui/dockfft.h>
#include <QTimer>
#include<QPointer>
//#include<QAudioDeviceInfo>
//#include<ioconfig.h>
//#include"qtgui/dockaudio.h"
//#include"qtgui/dockinputctl.h"
//#include"dsp/afsk1200/cafsk12.h"
#include<QComboBox>
//#ifdef WITH_PULSEAUDIO
//#include "pulseaudio/pa_device_list.h"
//#elif WITH_PORTAUDIO
//#include "portaudio/device_list.h"
//#elif defined(GQRX_OS_MACX)
//#include "osxaudio/device_list.h"
//#endif

#define FILTER_PRESET_WIDE      0
#define FILTER_PRESET_NORMAL    1
#define FILTER_PRESET_NARROW    2
#define FILTER_PRESET_USER      3
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int Volume;
    qint64 Frequency_centrer_value=91000000.0;// gia tri tan so trung tam của kenh
    qint64 Frequency_bandwith=1000000.0;// Gia tri cua bang thong kenh thu
    qint64 Frequency_Step=100000.0;// Phan le cua tan so trung tam
    QStringList *ArrayinputDevevice=new QStringList;// List danh sach ten cac thiet bị SDR
    QString NameDevieceOutput="default";// ten thiet bi audio se ket noi
    QString DeveiceInput=NULL;// Ten thiet bi SDR se ket noi

    qint64 d_lnb_lo;  /* LNB LO in Hz. */
    qint64 d_hw_freq;
    double SquechValue=-30.0;
    /* data decoders */

    //QInputMethod *mInputmethod;
    // Khai bao cac doi tuong
    /*****************************************************/
    bool mute_sound=false,Start_HF=false;
   // RemoteControl *remote;
    //receiver *rx;//doi tuong chua cac phuong thuc cai dat
    QTimer   *meter_timer;
   // DockFft      *uiDockFft;
   // DockAudio *uiDockAudio;
   // DockInputCtl   *uiDockInputCtl;// Doi tuong dieu chinh Gain (IF,BF,RF) của may thu
    // DockFft        *uiDockFft;
    //QTimer   *iq_fft_timer

    std::map<QString, QVariant> devList;// luu tru list cac thiet bi audio
  //  enum receiver::filter_shape d_filter_shape;
    QVarLengthArray<float, 16384> tmpbuf;
  //  CAfsk12 *decoder;
    /******************************************************/
    /*!< Application wide settings. */



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum rxopt_mode_idx {
        MODE_OFF        = 0, /*!< Demodulator completely off. */
        MODE_RAW        = 1, /*!< Raw I/Q passthrough. */
        MODE_AM         = 2, /*!< Amplitude modulation. */
        MODE_NFM        = 3, /*!< Narrow band FM. */
        MODE_WFM_MONO   = 4, /*!< Broadcast FM (mono). */
        MODE_WFM_STEREO = 5, /*!< Broadcast FM (stereo). */
        MODE_LSB        = 6, /*!< Lower side band. */
        MODE_USB        = 7, /*!< Upper side band. */
        MODE_CWL        = 8, /*!< CW using LSB filter. */
        MODE_CWU        = 9, /*!< CW using USB filter. */
        MODE_WFM_STEREO_OIRT = 10, /*!< Broadcast FM (stereo oirt). */
        MODE_LAST       = 11
    };// liet ke cac che do dieu che


private:
    Ui::MainWindow *ui;
public:

    void getDevieceOutputAudio(QString Name);//Lay danh sach ten cac thiet bi audio và ket noi vvoi may thu
    static QStringList ModulationStrings;// List danh sach cac thiet bi
    void setFrequency_Center();// cai dat tan so trung tam
    void getaudiooutput(QComboBox *combobox);// lay list ten cac thiet bi audio
    void getSampleRateInput();// lay ten cac thiet bi SRD
    void SetupforDockInputCtl();// ly thong tin do nhay tu HAckRF
    void setAudioOutputDevice(int index);// ket noi thiet bi audio dau ra
    void updateGainStages(bool read_from_device);// update lai thong so gain
    // Cac chuc nang chua dung den
    /*_________________________________*/
    unsigned int filterIdxFromLoHi(int lo, int hi) const;
    void setFilterParam(int lo, int hi);
    void on_plotter_newFilterFreq(int low, int high);
    void getFilterPreset(int mode, int preset, int * lo, int * hi) const;
    void process_samples(float *buffer, int length);
   /*____________________________________*/
public slots:
    void setInputdeviece(int index);// connect thiet bi audio
    void setNewFrequency(int rx_freq_int);//set tan so trung tam toi gia tri moi
    void selectDemod(int mode_idx);//chon che do dieu che
    void setPassband(int bandwidth);//set bang thong cho kenh thu
    void setGain(QString name, double gain);//dieu chinh gain cua may thu (bao gom IF,BF,RF)
    void setSquechvalue(int Squechvalue);// dieu chinh gia tri do nhay cong suat
    void setAudioGain(int valueint);// dieu chinh gia tri am luong cua loa
    void setAutoGain(bool enabled);
    void setinputSampleRate(int SampleRateInput);// dieu chinh toc do lay mau tin hieu tu analogsang digital
    void setAntenna(const QString antenna);
    void setDecimation(int Decimation_index);
    void setAgcGain(int gain);


    //cac chuc nang chua dung den
/*____________________________________________*/
    void on_NB_AFC_radioButton_clicked();
    void on_AGC_radioButton_clicked();
    void on_APF_IF_radioButton_clicked();
    void on_Attenuation_10_radioButton_clicked();
    void on_Attenuation_30_radioButton_clicked();
    void on_pushButton_mute_sound_clicked();
    void on_actionExit_triggered();
    void on_actionFFT_Setting_triggered();
    void decoderTimeout();

    void setIqSwap(bool reversed);
    void setDcCancel(bool enabled);
    void setIqBalance(bool enabled);
/*________________________________________________*/

    // function
    void meterTimeout();
    void on_Start_pushButton_clicked();
    void on_Mode_band_comboBox_currentIndexChanged(int index);
    void on_comboBox_activated(int index);
    void on_Recorder_radioButton_toggled(bool checked);
    void on_Deveice_Output_Audio_comboBox_currentIndexChanged(const QString &arg1);
    void on_Frequency_Centrer_value_spinBox_valueChanged(int arg1);
    void on_Frequency_Step_value_SpinBox_valueChanged(int arg1);
    void on_BandWidth_value_SpinBox_2_valueChanged(int arg1);
    void on_Sample_Rate_Input_comboBox_currentIndexChanged(int index);
    void on_reset_pushButton_clicked();
    void on_Show_dockinput_control_radioButton_2_toggled(bool checked);
    void on_Show_dockinput_control_checkBox_toggled(bool checked);
    void startAudioStream(const QString udp_host, int udp_port);
    void stopAudioStreaming();
signals :
    int SampleRateInputValuechange(int Samplerate);

//private:
//#ifdef WITH_PULSEAUDIO
//    vector<pa_device>           outDevList;
//#elif WITH_PORTAUDIO
//    vector<portaudio_device>    outDevList;
//#elif defined(GQRX_OS_MACX)
//    vector<osxaudio_device>     outDevList;
//#endif
};


#endif // MAINWINDOW_H
