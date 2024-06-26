#include "MainWindow.h"

MainWindow::MainWindow() :
	m_pMainLayout(nullptr),
	m_pImageLayout(nullptr),
	m_pButtonLayout(nullptr),
	m_pImageDisplay(nullptr),
	m_pImageProcessor(nullptr),
	m_pArduinoController(nullptr),
	m_pLoadImageButton(nullptr),
	m_pClearImageButton(nullptr),
	m_pProcessImageButton(nullptr),
	m_pStartButton(nullptr),
	m_pPauseButton(nullptr),
	m_pStopButton(nullptr),
	m_pImageLabel(nullptr),
	m_pArduinoLabel(nullptr)
{
	// resizes the main window
	resize(mainWindowWidth, mainWindowHeight);
	
	// set title
	setWindowTitle("2D Plotter");
	
	// init processor
	m_pImageProcessor = new ImageProcessor;
	
	// init controller
	m_pArduinoController = new ArduinoController;
	m_pArduinoController->setFps(ArduinoControllerFPS);

	// designs the interface
		// create all the Buttons (left panel)
		// image
	m_pImageLabel = new QLabel();
	m_pImageLabel->setText("IMAGE");
	m_pImageLabel->setAlignment(Qt::AlignHCenter);
	
	m_pLoadImageButton = new QPushButton("Load");
	m_pClearImageButton = new QPushButton("Clear");
	m_pProcessImageButton = new QPushButton("Process");
		// arduino
	m_pArduinoLabel = new QLabel();
	m_pArduinoLabel->setText("ARDUINO");
	m_pArduinoLabel->setAlignment(Qt::AlignHCenter);

	m_pStartButton = new QPushButton("Start Drawing");
	m_pPauseButton = new QPushButton("Pause Drawing");
	m_pStopButton = new QPushButton("Stop Drawing");
		// set the Buttons on a layout (left panel)
	m_pButtonLayout = new QVBoxLayout();
	m_pButtonLayout->addWidget(m_pImageLabel);
		m_pButtonLayout->addWidget(m_pLoadImageButton);
		m_pButtonLayout->addWidget(m_pClearImageButton);
		m_pButtonLayout->addWidget(m_pProcessImageButton);
	m_pButtonLayout->addWidget(m_pArduinoLabel);	
		m_pButtonLayout->addWidget(m_pStartButton);
		m_pButtonLayout->addWidget(m_pPauseButton);
		m_pButtonLayout->addWidget(m_pStopButton);
		
		// Image display (right panel)
	m_pImageLayout = new QHBoxLayout();
	m_pImageDisplay = new ImageDisplay;
	m_pImageLayout->addWidget(m_pImageDisplay);
		
		// Main display (left & right panels)
	m_pMainLayout = new QHBoxLayout();
		m_pMainLayout->addLayout(m_pButtonLayout);
		m_pMainLayout->addLayout(m_pImageLayout);


	//=======Qt CONNECTIONS=========
		// buttons (clicked signals)
	QObject::connect(m_pLoadImageButton, 	SIGNAL(clicked()), m_pImageDisplay,    	SLOT(openDialog()));
	QObject::connect(m_pClearImageButton, 	SIGNAL(clicked()), m_pImageDisplay,    	SLOT(clearImage()));
	QObject::connect(m_pProcessImageButton, SIGNAL(clicked()), m_pImageProcessor,    	SLOT(process()));
	QObject::connect(m_pStartButton, 		SIGNAL(clicked()), m_pArduinoController,  	SLOT(setStartButton()));
	QObject::connect(m_pStartButton, 		SIGNAL(clicked()), this,  				SLOT(setStartButton()));
	QObject::connect(m_pPauseButton, 		SIGNAL(clicked()), m_pArduinoController,   SLOT(setPauseButton()));
	QObject::connect(m_pPauseButton, 		SIGNAL(clicked()), this,   				SLOT(setPauseButton()));
	QObject::connect(m_pStopButton, 		SIGNAL(clicked()), m_pArduinoController,   SLOT(setStopButton()));
	QObject::connect(m_pStopButton, 		SIGNAL(clicked()), this,				SLOT(setStopButton()));
		// imageDisplay -> imageProcessor (original image)
	QObject::connect(m_pImageDisplay, SIGNAL(sendOriginalImage(QImage)), m_pImageProcessor,    SLOT(setImage(QImage)));
		// imageProcessor -> imageDisplay (contours)
	QObject::connect(m_pImageProcessor, SIGNAL(sendContours(std::vector<std::vector<cv::Point>>)), m_pImageDisplay,    SLOT(setContours(std::vector<std::vector<cv::Point>>)));
		// imageProcessor -> arduinoController (contours)
	QObject::connect(m_pImageProcessor, SIGNAL(sendContours(std::vector<std::vector<cv::Point>>)), m_pArduinoController,    SLOT(setContours(std::vector<std::vector<cv::Point>>)));
		// arduinoController -> mainwindow (arduino cmd)
	QObject::connect(m_pArduinoController, SIGNAL(sigSendArduinoCommand(const QByteArray)), this,    SLOT(setArduinoCommand(const QByteArray)));
		// imageDisplay ->arduino controller (image size)
	QObject::connect(m_pImageDisplay, SIGNAL(sendImageSize(int, int)), m_pArduinoController,    SLOT(setImageSize(int, int)));
		// imageDisplay ->MainWindow (image loaded)
	QObject::connect(m_pImageDisplay, SIGNAL(sendImageLoaded()), this,    SLOT(setImageLoaded()));
		// imageDisplay ->MainWindow (image cleared)
	QObject::connect(m_pImageDisplay, SIGNAL(sendImageCleared()), this,    SLOT(setImageCleared()));
		// imageProcessor ->MainWindow (image processed)
	QObject::connect(m_pImageProcessor, SIGNAL(sendImageProcessed()), this,    SLOT(setImageProcessed()));
		// ArduinoController ->imageDisplay (contour)
	QObject::connect(m_pArduinoController, SIGNAL(sigSendCurrentPixel(cv::Point)), m_pImageDisplay, SLOT(setPoint(cv::Point)));
		// this ->imageDisplay (reset to processed image)
	QObject::connect(this, SIGNAL(sendResetToProcessedImage()), m_pImageDisplay, SLOT(resetToProcessedImage()));
	
	// attachs the layout to the main window
	setLayout(m_pMainLayout);
	
	// disable all buttons except load image at start
	disableAllButtons();
	m_pLoadImageButton->setEnabled(true);
}

MainWindow::~MainWindow()
{
	auto deleteAndNullify = [](auto pointer) -> void
	{
		if (nullptr != pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	};

	deleteAndNullify(m_pImageLabel);
	deleteAndNullify(m_pArduinoLabel);
	deleteAndNullify(m_pStartButton);
	deleteAndNullify(m_pPauseButton);
	deleteAndNullify(m_pStopButton);
	deleteAndNullify(m_pLoadImageButton);
	deleteAndNullify(m_pClearImageButton);
	deleteAndNullify(m_pProcessImageButton);
	deleteAndNullify(m_pImageDisplay);
	deleteAndNullify(m_pMainLayout);
	deleteAndNullify(m_pImageLayout);
	deleteAndNullify(m_pButtonLayout);
	deleteAndNullify(m_pImageProcessor);
	
}


void MainWindow::setArduinoCommand(const QByteArray arduinoCmd)
{
	emit sigSendArduinoCommand(arduinoCmd);
}

void MainWindow::disableAllButtons()
{
	disableButton(m_pStartButton);
	disableButton(m_pPauseButton);
	disableButton(m_pStopButton);
	disableButton(m_pLoadImageButton);
	disableButton(m_pClearImageButton);
	disableButton(m_pProcessImageButton);
}

void MainWindow::disableButton(QPushButton* pButton)
{
	if (nullptr != pButton)
	{
		pButton->setEnabled(false);
	}
}

void MainWindow::enableButton(QPushButton* pButton)
{
	if (nullptr != pButton)
	{
		pButton->setEnabled(true);
	}
}

void MainWindow::setImageLoaded()
{
	disableAllButtons();
	
	enableButton(m_pLoadImageButton);
	enableButton(m_pProcessImageButton);
}

void MainWindow::setImageProcessed()
{
	enableButton(m_pClearImageButton);
	enableButton(m_pStartButton);
}

void MainWindow::setImageCleared()
{
	disableAllButtons();
	
	enableButton(m_pLoadImageButton);
	enableButton(m_pProcessImageButton);
}

void MainWindow::setStartButton()
{
	disableButton(m_pStartButton);
	
	enableButton(m_pPauseButton);
	enableButton(m_pStopButton);
}

void MainWindow::setPauseButton()
{
	disableButton(m_pPauseButton);
	
	enableButton(m_pStartButton);
	enableButton(m_pStopButton);
}

void MainWindow::setStopButton()
{
	disableButton(m_pPauseButton);
	disableButton(m_pStopButton);
	
	enableButton(m_pStartButton);
	
	emit sendResetToProcessedImage();
}