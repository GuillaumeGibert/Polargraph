#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

#include "ImageDisplay.h"
#include "ImageProcessor.h"
#include "ArduinoController.h"
#include "Config.h"

class MainWindow: public QWidget
{
    Q_OBJECT

    public:
        MainWindow();
        virtual ~MainWindow();

    public slots:
	void setArduinoCommand(const QByteArray);
	void setImageLoaded();
	void setImageProcessed();
	void setImageCleared();
	void setStartButton();
	void setPauseButton();
	void setStopButton();
    
    
    protected:
	void disableAllButtons();
	void disableButton(QPushButton*);
	void enableButton(QPushButton*);
    
    signals:
	void sigSendArduinoCommand(const QByteArray);
	void sendResetToProcessedImage();

    protected:
        QHBoxLayout* m_pMainLayout;
        QVBoxLayout* m_pButtonLayout;
        QHBoxLayout* m_pImageLayout;
    
   	QPushButton* m_pStartButton;
	QPushButton* m_pPauseButton;
	QPushButton* m_pStopButton;
	QPushButton* m_pLoadImageButton;
	QPushButton* m_pClearImageButton;
	QPushButton* m_pProcessImageButton;
    
	QLabel* m_pImageLabel;
	QLabel* m_pArduinoLabel;
    
	ImageDisplay* m_pImageDisplay;
	    
	ImageProcessor* m_pImageProcessor;
    
	ArduinoController* m_pArduinoController;
};

#endif
