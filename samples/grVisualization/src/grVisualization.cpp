#include "grVisualization.h"
#include "ui_grVisualisation.h"



GRVisualization::GRVisualization(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::GRVisualization)
{

    ui->setupUi(this);
#ifdef GR_VISUALIZATION_LOGGING_ENABLED
	printf("GR visualization: setting up UI...\n");
#endif

	initUiProps();

    // Set OpenGL Models for appliers
	rightArmApplier.setArm(&ui->GLwidget->rightArm);
	rightArmApplier.assignDeviceName("GR[R]");
	leftArmApplier.setArm(&ui->GLwidget->leftArm);
	leftArmApplier.assignDeviceName("GR[L]");

	plotter_acc = new GRDataPlotter(ui->only_accelerometer);
	plotter_gyro = new GRDataPlotter(ui->only_gyroscope);
	plotter_mag = new GRDataPlotter(ui->only_magnetometer);

	plotter_all_acc = new GRDataPlotter(ui->accelerometer);
	plotter_all_gyro = new GRDataPlotter(ui->gyroscope);
	plotter_all_mag = new GRDataPlotter(ui->magnetometer);

    buffer = new std::deque<std::vector<float>>();
}

GRVisualization::~GRVisualization()
{
	delete plotter_acc;
	delete plotter_gyro;
	delete plotter_mag;

	delete plotter_all_acc;
	delete plotter_all_gyro;
	delete plotter_all_mag;
	delete ui;
    delete buffer;


#ifdef GR_VISUALIZATION_LOGGING_ENABLED
	printf("GR visualization was ended successful\n");
#endif
}

// constructor helper
bool GRVisualization::initUiProps()
{
	// splitter init
	QList<int> sizes;
	sizes.push_back(300);
	sizes.push_back(100);
	ui->splitter->setSizes(sizes);
	// checkboxes style init
    ui->hackerModeCheckBox->setStyleSheet("background-color: #76B8DC;");
    ui->leftHandCheckBox->setStyleSheet("background-color: #76B8DC;");
    ui->rightHandCheckBox->setStyleSheet("background-color: #76B8DC;");
    ui->trajectoryCheckBox->setStyleSheet("background-color: #76B8DC;");
    ui->loggingCheckBox->setStyleSheet("background-color: #76B8DC;");
    ui->settings->setStyleSheet("background-color: #76B8DC;");
    ui->devicesTree->setStyleSheet("background-color: #76B8DC;");
	// QColor backgroundColor = ui->settings->palette().light().color();
	// backgroundColor.setAlpha(100);
	// QPainter customPainter(this);
	// customPainter.fillRect(rect(),backgroundColor);

	// disable push random data button
	ui->randomData->setVisible(false);


	return true;
}


/*
 * Runs a process of device searching, data reading and plotting
 * red, green, blue, alpha
*/
bool GRVisualization::runDataReading()
{

#ifdef GR_VISUALIZATION_LOGGING_ENABLED
	printf("GRVisualization: running data reading...\n");
#endif

    // configuring connection to devices

    std::map<int, device_t> availableDevices;
    availableDevices = conn.getAvalibleDevices();
    bool l = false;
    bool r = false; 
    for(std::map<int, device_t>::const_iterator it = availableDevices.begin(); it != availableDevices.end(); ++it)
    {
	if( it->second.name == leftArmApplier.deviceName )
	{
    		// for LEFT arm find device, activate it and start reading to OpenGL model
        	//
		leftArmApplier.setDeviceId(it->second.id);	
		leftArmApplier.setConnection(&conn);   
		
        	activeDevices[it->first] = it->second; // add information about active devices
		
        conn->setActiveDevice(it->first);
		leftArmApplier.run();
    		l = true;
		printf("Activated device with name %s\n", leftArmApplier.deviceName.c_str());
	}
	else if( it->second.name == rightArmApplier.deviceName )
	{
		// for RIGHT arm find device, activate it and start reading to OpenGL model
		//
		rightArmApplier.setDeviceId(it->second.id);	
		rightArmApplier.setConnection(&conn);   
        	//
        	activeDevices[it->first] = it->second; // add information about active devices
		
        conn->setActiveDevice(it->first);
        r = true;
		// setting up fetching function calls
        	printf("Activated device with name %s\n", rightArmApplier.deviceName.c_str());
	}
    }
    if(r)
    {
		rightArmApplier.run();
    }
    if(l)
    {
        leftArmApplier.run();
    }

    // setting up buffer for plotter
   // plotter_all_acc->setupPlot(buffer);
    // left arm data to this buffer
  //  leftArmApplier.writeQuanternionHistory(buffer);

    

    //plotter_all_acc->runPlotting();
    
    
    // UI
    // find devices and put them into UI devices tree
    
    device_t d;
    
    if(availableDevices.empty()) // if there are no activeDevices - show some sample data in UI devices tree
    {
        d.name = "SOME DEVICE";
        d.address = "08:FC:0S:SD:0P";
        availableDevices[4] = d;
    }
    
    for(std::map<int, device_t>::const_iterator it = availableDevices.begin(); it != availableDevices.end(); it++)
    {
        d = it->second;
        QTreeWidgetItem* item = new QTreeWidgetItem();
        QTreeWidgetItem* address = new QTreeWidgetItem();
        QTreeWidgetItem* id = new QTreeWidgetItem();
        item->setText(0, QString(d.name.c_str()));
        address->setText(0, QString(d.address.c_str()));
        id->setText(0, QString(QString::number(d.id)));
        item->addChild(id);
        item->addChild(address);
        ui->devicesTree->addTopLevelItem(item);
    }

	return true;
}

//
// Qt Slots
//
/*
//if(!fetchRunning)
//
//	{
        // ok, another fetch is not running at the moment
//		fetchRunning = true;
		if ( rightArmActivated )
		{
		//	printf("!!Right arm fetch!\n");
			rightArmApplier.fetchData();
		}
		
		if ( leftArmActivated )
		{
		//	printf("!!Left arm fetch!\n");
			leftArmApplier.fetchData();
		}
		
//		fetchRunning = false;
		return true;

//	}
    return false;
};
*/

/*
 * Render or not trajectory
*/
void GRVisualization::on_trajectoryCheckBox_toggled(bool checked)
{
	leftArmApplier.withTrajectory = checked;
	rightArmApplier.withTrajectory = checked;
//	ui->GLwidget->renderTrajectory(checked);
}


/*
 * Render or not Left hand
*/
void GRVisualization::on_leftHandCheckBox_toggled(bool checked)
{
	ui->GLwidget->renderLeftHand(checked);
}

/*
 * Render or not Right hand
*/
void GRVisualization::on_rightHandCheckBox_toggled(bool checked)
{
	ui->GLwidget->renderRightHand(checked);
}

void GRVisualization::on_hackerModeCheckBox_toggled(bool checked)
{
	if (checked)
	{
        ui->GLwidget->getRenderer()->setLinesColor(249, 173, 201, 255);
        ui->GLwidget->getRenderer()->setPlaneColor(0,0,0, 0);
        ui->GLwidget->getRenderer()->setLineWidth(1.5f);
//		ui->hackerModeCheckBox->setStyleSheet("color: white;"
//				                                      "background: #e67e22;"
//				                                      "selection-color: #34495e;"
//				                                      "selection-background-color: white;");
	} else
	{
        // go to default    a r g b
        ui->GLwidget->getRenderer()->setLineWidth(2.f);
        ui->GLwidget->getRenderer()->setLinesColor(235, 86, 141, 255);
		ui->GLwidget->getRenderer()->setPlaneColor(255, 255, 255, 255);
//		ui->hackerModeCheckBox->setStyleSheet("color: white;"
//                                                      "background: #76B8DC;"
//				                                      "selection-color: white;"
//				                                      "selection-background-color: white;");
	}

}


/*
 * Show&Hide randomData button
*/
void GRVisualization::on_loggingCheckBox_toggled(bool checked)
{
	ui->randomData->setVisible(checked);

//	rightArmApplier.isLoggingEnabled = checked;
}


/*
 * Pushes random data to history
*/
void GRVisualization::on_randomData_clicked()
{
//	printf("\nGRVisualization: The function Random data is disabled now\n");
    int max = 256, min = -256;
    std::vector<double> s;
    s.push_back(qrand() % ((max + 1) - min) + min);
//    s.append(" ");
    s.push_back(qrand() % ((max + 1) - min) + min);
//    s.append(" ");
    s.push_back(qrand() % ((max + 1) - min) + min);
    rightArmApplier.addHistoryData(s);

}

/*
 * Pause&run plotting
*/
void GRVisualization::on_pausePlotCheckBox_toggled(bool checked)
{
	plotter_acc->pause = checked;
	plotter_mag->pause = checked;
	plotter_gyro->pause = checked;

	plotter_all_acc->pause = checked;
	plotter_all_mag->pause = checked;
	plotter_all_gyro->pause = checked;
}


void GRVisualization::on_rotationsCheckBox_toggled(bool checked)
{
	leftArmApplier.withRotations = checked;
	rightArmApplier.withRotations = checked;
}
