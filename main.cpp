#include <QtCore/QCoreApplication>
#include <QFile>
#include <QMap>
#include <QRegExp>
#include <QDebug>
#include <QTextStream>
#include <QDir>
#include <station.h>

//generate output message for wrong input
void wrongInput() {
    qDebug() << "Parameters should be"    << endl << endl
             << "For a single station:" << endl
             << "---------------------" << endl
             << "Station file (.csv) followed by pre|rh|tem|tmax|tmin and the corresponding measure files (.dat)" << endl
             << "For example: dwd-converter ~/Desktop/04474.csv tmax ~/Desktop/tmax/04474.dat tmin ~/Desktop/tmin/04474.dat" << endl << endl
             << "For a set of files:"   << endl
             << "-------------------"   << endl
             << "Output folder followed by pre|rh|tem|tmax|tmin and the corresponding measure folders" << endl
             << "For example: dwd-converter ~/Desktop/results/ tmax ~/Desktop/tmax/ tmin ~/Desktop/tmin/" << endl << endl
             << "Programm Exit";
    exit(1);
}

//generate station object with informations from the given file
Station readStationFromFile(QString path) {
    QFile f(path);
    Station s;
    QStringList l;
    bool firstLine = true;

    f.open(QIODevice::ReadOnly);

    QString temp = QString(f.readLine()).remove(0, 5);
    temp.chop(1);
    s.setID(temp);

    f.readLine(); //skip table header

    while(!f.atEnd()) {
        l = QString::fromAscii(f.readLine())
                .split(QRegExp(","));
        l.last().chop(1);

        //Set start dates
        if (firstLine) {
            s.getTimeSpanPressure()->setStartDate(l.at(1));
            s.getTimeSpanRelativeHumidity()->setStartDate(l.at(1));
            s.getTimeSpanAverageTemperature()->setStartDate(l.at(1));
            s.getTimeSpanMaxTemperature()->setStartDate(l.at(1));
            s.getTimeSpanMinTemperature()->setStartDate(l.at(1));
            firstLine = false;
        }

        //Append values
        s.getPressure()->append(l.at(2));
        s.getRelativeHumidity()->append(l.at(3));
        s.getAverageTemperature()->append(l.at(4));
        s.getMaxTemperature()->append(l.at(5));
        s.getMinTemperature()->append(l.at(6));

        //Set end dates
        if (f.atEnd()) {
            s.getTimeSpanPressure()->setEndDate(l.at(1));
            s.getTimeSpanRelativeHumidity()->setEndDate(l.at(1));
            s.getTimeSpanAverageTemperature()->setEndDate(l.at(1));
            s.getTimeSpanMaxTemperature()->setEndDate(l.at(1));
            s.getTimeSpanMinTemperature()->setEndDate(l.at(1));
        }

    }
    f.close();

    return s;
}

//read values from the given file for the given measure type to the given station
void readMeasureValuesFromFileToStation(Station* s, QString measureType, QString measureFile) {
    QString line, startDate, endDate;
    QStringList timeSpan, measureValues;

    QFile file(measureFile);
    file.open(QIODevice::ReadOnly);

    if (file.error()) {
        qDebug() << "File" << file.fileName() << "not found";
        exit(1);
    } else {
        qDebug() << "Extracting data from file"
                 << file.fileName();

        //Extract measure values and the related time span
        while(!file.atEnd()) {
            line = QString::fromAscii(file.readLine());
            line = line.simplified();

            //Check if the current line is a data line
            if(line.contains(QRegExp("^(\\-)?\\d+"))) {
                line = line.simplified();
                measureValues.append(line);
            } else //Line is a header line
                //Extract time span if its the right line
                if(line.contains(QRegExp("[0-9]{4}\\-[0-9]{2}\\-[0-9]{2}"))) {
                    timeSpan = line.split(" ");
                    timeSpan.removeAt(2);
                    timeSpan.removeAt(0);
                    startDate = timeSpan.at(0);
                    startDate = startDate.remove(0, 2);
                    endDate = timeSpan.at(1);
                    endDate = endDate.remove(0,2);
                }
        }
        file.close();

        //Put values and time span to the corresponding measure type
        if(measureType.operator ==("pre")) {
            s->setPressure(measureValues);
            s->setTimeSpanPressure(startDate, endDate);
        } else if (measureType.operator ==("rh")) {
            s->setRelativeHumidity(measureValues);
            s->setTimeSpanRelativeHumidity(startDate, endDate);
        } else if (measureType.operator ==("tem")) {
            s->setAverageTemperature(measureValues);
            s->setTimeSpanAverageTemperature(startDate, endDate);
        } else if (measureType.operator ==("tmax")) {
            s->setMaxTemperature(measureValues);
            s->setTimeSpanMaxTemperature(startDate, endDate);
        } else if (measureType.operator ==("tmin")) {
            s->setMinTemperature(measureValues);
            s->setTimeSpanMinTemperature(startDate, endDate);
        }

    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QMap<QString, QString> givenFolders;
    QList<QString> extractedStations;
    Station s;
    QStringList stationsFileLine;
    QMapIterator<QString, QString> iGivenFoldersForStations(givenFolders);
    QMapIterator<QString, QString> iGivenFoldersForData(givenFolders);
    QListIterator<QString> iStationsFileLine(stationsFileLine);

    //Check number of command line parameters
    if(argc < 3)
        wrongInput();

    //Check if single station or set of stations
    if (QString(argv[1]).contains(QRegExp("(\\d+)(.csv)$"))) {
        //Single station-----------------------------------------------------------

        //Check number of command line parameters
        if (argc % 2 != 0 || argc < 4)
            wrongInput();

        for (int i = 3 ; i < argc; i+=2)
            //Check if even parameters are pre|rh|tem|tmax|tmin
            //and odd parameters are dat files
            if( !QString(argv[i-1]).contains(QRegExp("pre|rh|tem|tmax|tmin"))
                    || !QString(argv[i]).endsWith(".dat") ) {
                qDebug() << "Error: Wrong input for measure type" << argv[i-1] << "or file" << argv[i];
                wrongInput();
            }

        qDebug() << "Single station";

        s = readStationFromFile(argv[1]);

        //Read measure files to station
        for (int i = 3; i < argc; i+=2) {
            readMeasureValuesFromFileToStation(&s, argv[i-1], argv[i]);
            s.generateMissingValues();
        }

        s.writeFile(argv[1]);

        exit(0);
    }else{
       //Set of stations------------------------------------------------------------

        //Check number of command line parameters
        if (argc % 2 != 0 || argc < 4)
            wrongInput();

        if (!QDir(argv[1]).exists())
            wrongInput();

        for (int i = 3 ; i < argc; i+=2)
            //Check if even parameters are pre|rh|tem|tmax|tmin
            //and odd parameters are folders
            if( !QString(argv[i-1]).contains(QRegExp("pre|rh|tem|tmax|tmin"))
                    || !QDir(argv[i]).exists() ) {
                qDebug() << "Error: Wrong input for measure type" << argv[i-1] << "or directory" << argv[i];
                wrongInput();
            }

        //Generate list of folders
        for(int i = 3; i < argc; i+=2)
            givenFolders.insert(argv[i-1], argv[i]);
        iGivenFoldersForStations.operator =(givenFolders);
        iGivenFoldersForData.operator =(givenFolders);

        qDebug() << "Set of stations";

        //Iterate over stations from all given station files------------------------
        qDebug() << endl << "Start station extraction...";

        /*
            Iterate over all folders given as command line parameters
            to extract stations
        */
        while(iGivenFoldersForStations.hasNext())
        {
            qDebug() << "Switch to folder"
                     << iGivenFoldersForStations.peekNext().value()
                     << "for station extraction" << endl;
            QFile stationsFile(iGivenFoldersForStations.peekNext().value()
                                   + "StationLuT_all.txt");

            qDebug() << endl << "Extract stations from file"
                     << stationsFile.fileName() << endl;

            stationsFile.open(QIODevice::ReadOnly);
            if(!stationsFile.exists()){
                qDebug() << "No file" << stationsFile.fileName()
                         << "found -> EXIT" << endl;
                exit(1);
            }

            //Iterate over the current StationLuT_all.txt file and
            //generate a csv file with all measurements for every station in it
            while(!stationsFile.atEnd())
            {
                //Extract line and crop what is not needed
                stationsFileLine = QString::fromAscii(stationsFile.readLine())
                        .split(QRegExp("((\\s\\s+)|(\\t))"));
                stationsFileLine.removeAll("");
                stationsFileLine.removeAt(11);
                stationsFileLine.removeAt(10);
                stationsFileLine.removeAt(8);
                stationsFileLine.removeAt(7);

                while(iStationsFileLine.hasNext())
                    iStationsFileLine.next().simplified();

                if(!stationsFileLine.isEmpty())
                {
                    //Check if the current line of the stations file is a data line
                    if(stationsFileLine.first().contains(QRegExp("^\\d+")) )
                    {
                        s = Station(stationsFileLine);

                        //Check if the current station was not worked before
                        //(StationLuT_all.txt file from previous folder)
                        if(!extractedStations.contains(s.getID()))
                        {
                            //Put station ID into list of stations
                            extractedStations.append(s.getID());

                            qDebug() << "Start data extraction for station"
                                     << s.getID();

                            //Iterate over all folders to get every measure type for
                            //the current station
                            iGivenFoldersForData.toFront();
                            while(iGivenFoldersForData.hasNext())
                            {
                                qDebug() << "Switching to folder"
                                         << iGivenFoldersForData.peekNext().value()
                                         << "for data extraction";

                                //Extract data for current station

                                //Extract data for current station
                                QFile dataFile(iGivenFoldersForData.peekNext().value()
                                               + s.getID() + ".dat");
                                dataFile.open(QIODevice::ReadOnly);

                                if(dataFile.exists())
                                {
                                    //Read measure files to station
                                    readMeasureValuesFromFileToStation(
                                                &s, iGivenFoldersForData.peekNext().key(),
                                                iGivenFoldersForData.peekNext().value() + s.getID() + ".dat");
                                }else
                                    qDebug() << "Station" << s.getID()
                                             << "has no measurement values in"
                                             << iGivenFoldersForData.peekNext().value();

                                iGivenFoldersForData.next();
                            }

                            if(s.isConsistent())
                            {
                                s.generateMissingMeasureTypes();
                                s.generateMissingValues();
                                s.writeFile(QString(argv[1]) + s.getID() + ".csv");
                            }else{
                                qDebug() << "Inconsistencies in station" << s.getID()
                                         << "detected -> Writing to log file";

                                QFile error(QString(argv[1]) + "errorLog.txt");
                                error.open(QIODevice::WriteOnly | QIODevice::Append);
                                QTextStream tsOut(&error);

                                if(!stationsFile.exists()){
                                    qDebug() << "No file" << error.fileName()
                                             << "found -> EXIT" << endl;
                                    exit(1);
                                }

                                tsOut << s.toQString() << "\n";
                                error.close();
                            }
                        }
                    }
                }
            }
            stationsFile.close();

            iGivenFoldersForData.toFront();
            iGivenFoldersForStations.next();
        }

        qDebug() << "Data for" << extractedStations.size() << "stations successfully written";
    }

    exit(0);
    return a.exec();
}
