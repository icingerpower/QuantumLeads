#include "gui/MainWindow.h"

#include "../common/workingdirectory/WorkingDirectoryManager.h"
#include "../common/workingdirectory/DialogOpenConfig.h"
#include "../common/types/types.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QList<QList<QVariant>>>();
    qRegisterMetaType<QList<QStringList>>();
    qRegisterMetaType<QSet<QString>>();

    WorkingDirectoryManager::instance()->installDarkOrangePalette();
    DialogOpenConfig dialog;
    dialog.exec();
    if (dialog.wasRejected())
    {
        return 0;
    }
    QCoreApplication::setOrganizationName("Icinger Power");
    QCoreApplication::setApplicationName("Quantum Leads");
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);
    MainWindow w;
    w.show();
    return a.exec();
}
