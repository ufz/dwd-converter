#include <QtCore/QCoreApplication>
#include <QFile>
#include <QMap>
#include <QRegExp>
#include <QDebug>
#include <QTextStream>

#include <station.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Check command line paramters
    if(argc != 7)
    {
        qDebug() << "Parameters must be"                    << endl
                 << "path to output folder"                 << endl
                 << "path to folder pressure"               << endl
                 << "path to folder relative humidity "     << endl
                 << "path to folder average temperature"    << endl
                 << "path to folder max temperature"        << endl
                 << "path to folder min temperature"        << endl
                 << "Dont Mess Up The Order!"               << endl << endl
                 << "Programm EXIT";
        exit(1);
    }

    QMap<QString, QString> givenFolders;
    QList<QString> extractedStations;
    Station s;
    QString dataFileLine, startDate, endDate;
    QStringList stationsFileLine, timeSpan;
    QMapIterator<QString, QString> iGivenFoldersForStations(givenFolders);
    QMapIterator<QString, QString> iGivenFoldersForData(givenFolders);
    QListIterator<QString> iStationsFileLine(stationsFileLine);

    QRegExp dateRx("[0-9]{4}\\-[0-9]{2}\\-[0-9]{2}");

    givenFolders.insert("pre", argv[2]);
    givenFolders.insert("rh", argv[3]);
    givenFolders.insert("tem", argv[4]);
    givenFolders.insert("tmax", argv[5]);
    givenFolders.insert("tmin", argv[6]);
    iGivenFoldersForStations.operator =(givenFolders);
    iGivenFoldersForData.operator =(givenFolders);

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
        //generate a csv file with every measurements for every station in it
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
                            QFile dataFile(iGivenFoldersForData.peekNext().value()
                                           + s.getID() + ".dat");
                            dataFile.open(QIODevice::ReadOnly);

                            if(dataFile.exists())
                            {
                                qDebug() << "Extracting data from file"
                                         << dataFile.fileName();

                                //Extract measure values and the related time span
                                while(!dataFile.atEnd())
                                {
                                    dataFileLine = QString::fromAscii(dataFile.readLine());
                                    dataFileLine = dataFileLine.simplified();

                                    //Check if the current line is a data line
                                    if(dataFileLine.contains(QRegExp("^(\\-)?\\d+")))
                                    {
                                        dataFileLine = dataFileLine.simplified();

                                        //Put values to the related measure type
                                        if(iGivenFoldersForData.peekNext().key() == "pre")
                                            s.getPressure()->append(dataFileLine);
                                        else if (iGivenFoldersForData.peekNext().key() == "rh")
                                            s.getRelativeHumidity()->append(dataFileLine);
                                        else if (iGivenFoldersForData.peekNext().key() == "tem")
                                            s.getAverageTemperature()->append(dataFileLine);
                                        else if (iGivenFoldersForData.peekNext().key() == "tmax")
                                            s.getMaxTemperature()->append(dataFileLine);
                                        else if (iGivenFoldersForData.peekNext().key() == "tmin")
                                            s.getMinTemperature()->append(dataFileLine);
                                        else
                                            qDebug() << "Unknow data class"
                                                     << iGivenFoldersForData.peekNext().key();
                                    }else{ //Line is a header line
                                        //Extract time span if its the right line
                                        if(dataFileLine.contains(dateRx))
                                        {
                                            timeSpan = dataFileLine.split(" ");
                                            timeSpan.removeAt(2);
                                            timeSpan.removeAt(0);
                                            startDate = timeSpan.at(0);
                                            startDate = startDate.remove(0, 2);
                                            endDate = timeSpan.at(1);
                                            endDate = endDate.remove(0,2);

                                            //Put time span to the related measure type
                                            if(iGivenFoldersForData.peekNext().key() == "pre")
                                                s.setTimeSpanPressure(startDate, endDate);
                                            else if (iGivenFoldersForData.peekNext().key() == "rh")
                                                s.setTimeSpanRelativeHumidity(startDate,
                                                                                    endDate);
                                            else if (iGivenFoldersForData.peekNext().key() == "tem")
                                                s.setTimeSpanAverageTemperature(startDate,
                                                                                      endDate);
                                            else if (iGivenFoldersForData.peekNext().key() == "tmax")
                                                s.setTimeSpanMaxTemperature(startDate,
                                                                                  endDate);
                                            else if (iGivenFoldersForData.peekNext().key() == "tmin")
                                                s.setTimeSpanMinTemperature(startDate,
                                                                                  endDate);
                                        }
                                    }
                                }
                                dataFile.close();
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
                            s.writeFile(argv[1]);
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

    exit(0);
    return a.exec();
}
