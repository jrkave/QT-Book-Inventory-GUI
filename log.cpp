#include <QFile>
#include <QTextStream>
#include <QDateTime>

QString fileName = "logFile.log";

void logToFile(QString message)
{
    // Open the log file in append mode to add new log messages
    QFile logFile(fileName);
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        // Handle error
        return;
    }

    // Create a text stream to write to the log file
    QTextStream out(&logFile);

    // Write the message with a timestamp to the log file
    QDateTime timestamp = QDateTime::currentDateTime();
    QString timestampString = timestamp.toString(Qt::ISODate);
    out << timestampString << " " << message << "\n";

    // Close the log file
    logFile.close();
}

void clearLogFile()
{
    // Open the log file in write-only mode to clear previous contents
    QFile logFile(fileName);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Handle error
        return;
    }
    logFile.close();
}
