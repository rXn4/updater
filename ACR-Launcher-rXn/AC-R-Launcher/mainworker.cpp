#include "mainworker.h"

MainWorker::MainWorker() : QObject()
{ }

//Delete completely?
//void MainWorker::parseCmd(QStringList av)
//{
//    if(av.length() < 2) {
//        help("");
//        return;
//    } else if(!QString(av[1]).contains("init", Qt::CaseInsensitive)) {
//        loadData();
//    }
//    //Better than calling functions directly by name (imo)
//    if(QString(av[1]).contains("runclient", Qt::CaseInsensitive)) {
//        runClient();
//    } else if(QString(av[1]).contains("runserver", Qt::CaseInsensitive)) {
//        runServer(false);
//    } else if(QString(av[1]).contains("updateall", Qt::CaseInsensitive)) {
//        updateAll();
//    } else if(QString(av[1]).contains("updateme", Qt::CaseInsensitive)) {
//        updateMe();
//    } else if(QString(av[1]).contains("updateacr", Qt::CaseInsensitive)) {
//        updateGame();
//    } else if(QString(av[1]).contains("updatemods", Qt::CaseInsensitive)) {
//        //TODO
//    } else if(QString(av[1]).contains("installmods", Qt::CaseInsensitive)) {
//        //TODO
//    } else if(QString(av[1]).contains("listmods", Qt::CaseInsensitive)) {
//        //TODO
//    } else if(QString(av[1]).contains("installlocalmods", Qt::CaseInsensitive)) {
//        //TODO
//    } else if(QString(av[1]).contains("installedmods", Qt::CaseInsensitive)) {
//        //TODO
//    } else if(QString(av[1]).contains("backup", Qt::CaseInsensitive)) {
//        //TODO
//    } else if(QString(av[1]).contains("init", Qt::CaseInsensitive)) {
//        init();
//    } else {
//        help(av[2]);
//    }
//    //Quick and dirty...
//    cout << msgOut.readAll().toStdString();
//}


//Remove mod from list -REALLY NECCESSARY?! rXn 01.2015
//void MainWorker::pruneMod(QString badMod) {
//    QString ret_value = "Not found!";
//    QList<ModEntry> newList;
//    for(int i = 0; i < SavedData->getInstalledMods().count(); i++) {
//        if(SavedData->getInstalledMods()[i].Name.compare(badMod.trimmed, Qt::CaseInsensitive)) {
//            newList.append(SavedData->getInstalledMods()[i]);
//            ret_value = "\"" + badMod + "\"" + " removed!";
//        }
//    }
//    SavedData->setInstalledMods(newList);
//}

//Get config data and mods -DONE
QString MainWorker::loadData() {

    //Message to return to UI
    QString ret_val = "";

    // Deal with global options //
    QFile configFile("updater.conf");
    if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ret_val =+ "Error loading ConfigData! ";
    }
    QString line;
    line = configFile.readLine();
    line = line.trimmed();
    while(line != NULL) {
        if(line != "") {
            if(!line.split("=")[0].compare("version", Qt::CaseInsensitive)) {
                version = line.split("=")[1];
            } else if(!line.split("=")[0].compare("modserverurl", Qt::CaseInsensitive)) {
                modserverurl = line.split("=")[1];
            }
        }
        line = configFile.readLine();
        line = line.trimmed();
    }
    //Check if read correctly
    if(version.isNull() || modserverurl.isNull() || !modserverurl.contains("/")) {
        configFile.close();
        ret_val =+ "Error loading ConfigData! ";
    }
    configFile.close();

    // Mod options and config //

    //Read mod list
    QFile list("localMods.conf");
    if(!list.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ret_val =+ "Error loading ModData! ";
    }
    bool mod_started = false;
    bool somethingDone = false;
    ModEntry tmp;
    QString currentLine = list.readLine();
    QString keywords = "start_mod mod_name mod_version mod_desc mod_url end_mod";
    while(!list.atEnd()) {
        currentLine = currentLine.trimmed();
        somethingDone = false;

        //Are we inside a mod def?
        if(currentLine.contains("#start_mod", Qt::CaseInsensitive)) {
            somethingDone = true;
            tmp.clear();
            currentLine = list.readLine();
            mod_started = true;
            continue;
        } else {
            //Getting the infos
            if(!currentLine.split("=")[0].compare("#mod_name", Qt::CaseInsensitive)) {
                somethingDone = true;
                currentLine = currentLine.trimmed();
                tmp.Name = currentLine.split("=")[1];
                currentLine = list.readLine();
                while(!keywords.contains(currentLine.split("=")[0], Qt::CaseInsensitive) && !currentLine.contains("#")) {
                    tmp.Name = tmp.Name + currentLine;
                    currentLine = list.readLine();
                }
            }
            if(!currentLine.split("=")[0].compare("#mod_version", Qt::CaseInsensitive)) {
                somethingDone = true;
                currentLine = currentLine.trimmed();
                tmp.Version = currentLine.split("=")[1];
                currentLine = list.readLine();
                while(!keywords.contains(currentLine.split("=")[0], Qt::CaseInsensitive) && !currentLine.contains("#")) {
                    tmp.Version = tmp.Version + currentLine;
                    currentLine = list.readLine();
                }
            }
            if(!currentLine.split("=")[0].compare("#mod_desc", Qt::CaseInsensitive)) {
                somethingDone = true;
                currentLine = currentLine.trimmed();
                tmp.Description = currentLine.split("=")[1];
                currentLine = list.readLine();
                while(!keywords.contains(currentLine.split("=")[0], Qt::CaseInsensitive) && !currentLine.contains("#")) {
                    tmp.Description = tmp.Description + currentLine;
                    currentLine = list.readLine();
                }
            }
            if(!currentLine.split("=")[0].compare("#mod_url", Qt::CaseInsensitive)) {
                somethingDone = true;
                currentLine = currentLine.trimmed();
                tmp.Url = currentLine.split("=")[1];
                currentLine = list.readLine();
                while(!keywords.contains(currentLine.split("=")[0], Qt::CaseInsensitive) && !currentLine.contains("#")) {
                    tmp.Url = tmp.Url + currentLine;
                    currentLine = list.readLine();
                }
            }
            if(!currentLine.split("=")[0].compare("#mod_author", Qt::CaseInsensitive)) {
                somethingDone = true;
                currentLine = currentLine.trimmed();
                tmp.Author = currentLine.split("=")[1];
                currentLine = list.readLine();
                while(!keywords.contains(currentLine.split("=")[0], Qt::CaseInsensitive) && !currentLine.contains("#")) {
                    tmp.Author = tmp.Author + currentLine;
                    currentLine = list.readLine();
                }
            }
            if(currentLine.contains("#end_mod", Qt::CaseInsensitive)) {
                somethingDone = true;
                availableMods.append(tmp);
                mod_started = false;
                currentLine = list.readLine();
            }

            //If nothing was done (empty line, whatever), read the next line.
            if(!somethingDone) {
                currentLine = list.readLine();
            }
        }

    }

    installedMods.append(tmp);

    //Compare to what we actually have in mod dir
    QDir dir = QDir::current();
    dir.cd("acr");
    dir.cd("mods");
    if(!dir.exists()) {
        ret_val =+ " Cannot read mod directory! ";
    }
    dir.setFilter(QDir::Dirs);
    dir.setFilter(QDir::NoDot);
    dir.setFilter(QDir::NoDotDot);
    QStringList modDirList = dir.entryList();
    if(!modDirList.isEmpty() && installedMods.count() > 0) { //Mods found, but no mod list
            ret_val =+ " Your mod config file is empty while there are mods installed! ";
    }
    if(installedMods.count() != modDirList.count()) { //Not the same number of mods in dir and list
            ret_val =+ " Your mod config does not fit! ";
    }

    //Get new mods anyway ;)
    mdloader = new ModListDownloader(modserverurl);
    connect(mdloader, SIGNAL(isReady(QList<ModEntry>)), this, SLOT(on_modListDownloader_isReady(QList<ModEntry>)));
    mdloader->start();
    //Return message
    return ret_val;
}

void MainWorker::on_modListDownloader_isReady(QList<ModEntry> l) {
    availableMods = l;
    emit modListDownloader_isReady();
    mdloader->deleteLater();
}



//Writes the Client.sh/.bat file -DONE
QString MainWorker::writeClientExecutable() {
    QString extraMods = "";
    foreach (ModEntry mod, installedMods) {
        extraMods += "--mod=mods/" + mod.Name.trimmed().split('-')[0] + " ";
    }
#ifdef Q_OS_WIN32 //Windows
    QFile batch("Client.bat");
    if(!batch.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return "Unable to write custom launcher, do you have writing permission?";
    }
    batch.write("cd acr\nbin_win32\\ac_client.exe --home=data "
                 + extraMods + "--mod=acr --init %*");
    batch.close();
    return "Wrote Client Launcher!";
#endif
#ifdef Q_OS_UNIX //Unix

    //Taken from my *quick* fix of the python version. Feel free to improve ;)
    QFile origFile("acr/client.sh");
    if(!origFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "Unable to read original launcher, check your installation!";
    }
    QFile shell("Client.sh");
    if(!shell.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return "Unable to write custom launcher, do you have writing permission?";
    }
    QTextStream sout(&shell);
    QString tmp = origFile.readLine();
    sout << tmp << "cd acr/" << endl;
    tmp = origFile.readLine();
    while(!tmp.contains("CUBE_OPTIONS", Qt::CaseInsensitive)) {
        sout << tmp;
        tmp = origFile.readLine();
    }
    //Now skip and replace the "bad" line...
    sout << "CUBE_OPTIONS=\"--home=data " + extraMods + "--mod=acr --init\"" << endl;
    origFile.readLine(); origFile.readLine();
    //...and write the remaining stuff.
    sout << origFile.readAll();
    shell.close();
    origFile.close();
    //Too lazy to figure out native file permission handling ;) //TODO ?
    QProcess p;
    p.start("chmod +x ./Client.sh");
    p.waitForFinished(-1);
#ifndef Q_OS_LINUX //Unix, but not Linux
     return "You are using an uncommon UNIX, probably you'll have to compile the game yourself.\n Wrote Client Launcher.";
#endif
    return "Successfully wrote Client Launcher.";
#endif
    return "You have to supply your own bat/sh type file. I don't know how to write one for your system!";
}

//Writes the Server.sh/.bat file -DONE
QString MainWorker::writeServerExecutable() {
    QString extraMods = "";
    foreach (ModEntry mod, installedMods) {
        extraMods += "--mod=mods/" + mod.Name.trimmed().split('-')[0] + " ";
    }
#ifdef Q_OS_WIN32 //Windows
    QFile batch("Server.bat");
    if(!batch.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return "Unable to write custom launcher, do you have writing permission?";
    }
    batch.write("cd acr\nbin_win32\\ac_server.exe --home=data "
                 + extraMods + "--mod=acr --init %*");
    batch.close();
    return "Wrote Server Launcher!";
#endif
#ifdef Q_OS_UNIX //Unix
    //Taken from my *quick* fix of the python version. Feel free to improve ;)
    QFile origFile("acr/server.sh");
    if(!origFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "Unable to read original launcher, check your installation!";
    }
    QFile shell("Server.sh");
    if(!shell.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return "Unable to write custom launcher, do you have writing permission?";
    }
    QTextStream sout(&shell);
    QString tmp = origFile.readLine();
    sout << tmp << "cd acr/" << endl;
    tmp = origFile.readLine();
    while(!tmp.contains("CUBE_OPTIONS", Qt::CaseInsensitive)) {
        sout << tmp;
        tmp = origFile.readLine();
    }
    //Now skip and replace the "bad" line...
    sout << "CUBE_OPTIONS=\"--home=data " + extraMods + "--mod=acr --init\"" << endl;
    origFile.readLine(); origFile.readLine();
    //...and write the remaining stuff.
    sout << origFile.readAll();
    shell.close();
    origFile.close();
    //Too lazy to figure out native file permission handling ;)
    QProcess p;
    p.start("chmod +x ./Server.sh");
    p.waitForFinished(-1);
#ifndef Q_OS_LINUX //Unix, but not Linux
    return "You are using an uncommon UNIX, probably you'll have to compile the game yourself.\n
            Wrote Server Launcher anyway.";
#endif
    return "Wrote Server Launcher!";
#endif
    return "You have to supply your own bat/sh type file. I don't know how to write one for your system.";
}

void MainWorker::init() {
    //TODO TBD
}

void MainWorker::updateAll() {
    //TODO TBD
}

void MainWorker::updateGame() {
    //TODO TBD
    /*Downloader d("file:///");
    d.getData();
    qDebug("dursch");
    /*FileDownloader *fd;
    fd = new FileDownloader(QUrl::fromEncoded("http://example.com"), this);
    connect(fd, SIGNAL(downloaded()), this, SLOT(downloadFinished()));
    fd->start();
    ql.exec();
    qDebug("downready");
    QFile f("test.htm");
    f.open(QIODevice::WriteOnly);
    f.write(fd->downloadedData());
    f.close();*/
}

void MainWorker::updateMe() {
    //TODO TBD
}

//TODO: Mods updating


QStringList MainWorker::getAvialableMods() {
    QStringList tmp;
    foreach (ModEntry me, availableMods) {
        tmp.append(me.toString());
    }
    return tmp;
}

QStringList MainWorker::getInstalledMods() {
    QStringList tmp;
    foreach (ModEntry me, installedMods) {
        tmp.append(me.toString());
    }
    return tmp;
}

QStringList MainWorker::getModInfos(QString mod) {
    QStringList result;
    foreach (ModEntry m, availableMods) {
      if(!m.Name.compare(mod)) {
          result.append(m.Name);
          result.append(m.Url);
          result.append("Version " + m.Version);
          result.append("By " + m.Author);
          result.append(m.Description);
      }
    }
    return result;
}

//Start client script -DONE
QString MainWorker::runClient() {
    QString ret_val = writeClientExecutable();
#ifdef Q_OS_WIN32 //Windows
    QProcess p;
    p.start("Client.bat");
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.waitForFinished(-1);
    return ret_val;
#endif
#ifdef Q_OS_UNIX //Unix
    QProcess p;
    p.start("./Client.sh");
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.waitForFinished(-1);
    return ret_val;
#endif
    return "I don't recognize your OS";
}

//Start a server script in a new window -DONE
QString MainWorker::runServer(bool new_window) {
    QString ret_val = writeServerExecutable();
    if(new_window) {
        ServerConsole *sc = new ServerConsole(0, ret_val);
        sc->show();
        return ret_val;
    }
#ifdef Q_OS_WIN32 //Windows
    QProcess p;
    p.start("Server.bat");
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.waitForFinished(-1);
    return ret_val;
#endif
#ifdef Q_OS_UNIX //Unix
    QProcess p;
    p.start("./server.sh");
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.waitForFinished(-1);
    return ret_val;
#endif
    return "I don't recognize your OS" ;
}

//Create a zip file with config and game -DONE
QString MainWorker::createBackup() {
    QStringList filesAndFolders;
    filesAndFolders.append("acr");
    filesAndFolders.append("updater.conf");
    filesAndFolders.append("localMods.conf");
    return createZipfile(filesAndFolders, QDateTime::currentDateTime().toString());
}

//Nothing to do here? -DONE
QString MainWorker::installBackup(QString file) {
    return extractZipfile(file);
}

//Mod installation, as called from UI -DONE
QString MainWorker::installMod(QString mod) {
    //Check if already installed
    foreach (ModEntry instMod, installedMods) {
        if(!instMod.Name.compare(mod)) {
            return "You already have a version of this mod installed!\nMaybe try an update?";
        }
    }
    //Select the mod we want to install
    ModEntry me;
    foreach (ModEntry currMod, availableMods) {
        if(!currMod.Name.compare(mod)) {
            me = currMod;
        }
    }
    tmpModEntry = &me;
    //Start a download dialog
    dloadDialog = new DownloadDialog(me.Url);
    connect(dloadDialog, SIGNAL(downloadReady(QByteArray)), this, SLOT(on_DownloadDialog_downloadReady(QByteArray)));
    dloadDialog->exec();
    return "Done processing mod \"" + mod +"\"";
}

//Write the mod data to a file and extract it -DONE
void MainWorker::on_DownloadDialog_downloadReady(QByteArray data) {
    dloadDialog->deleteLater();
    tmpModEntry->toString();
    QTemporaryFile q;
    q.open();
    if(!q.isWritable()) {
        emit installerReady("Error installing mod!");
        return;
    }
    q.write(data);
    q.flush();
    q.close();
    QString msg = "";
    msg = extractZipfile(q.fileName(), "acr/mods/");
    q.remove();
    if(msg.compare("Error writing archive!")) {
       installedMods.append(*tmpModEntry);
    }
    emit installerReady(msg + "\nMod installed!");
}

//Clean exit
void MainWorker::writeToDisk()
{
    //Write mod config
    QFile modConfig("localMods.conf");
    if(modConfig.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream modConfigStream(&modConfig);
        foreach (ModEntry me, installedMods) {
            modConfigStream << "#start_mod" <<  endl;
            modConfigStream << "#mod_name=" <<  me.Name << endl;
            modConfigStream << "#mod_version=" << me.Version <<  endl;
            modConfigStream << "#mod_desc=" << me.Description << endl;
            modConfigStream << "#mod_url=" << me.Url << endl;
            modConfigStream << "#end_mod" << endl;
        }
        modConfigStream.flush();
        modConfig.close();
    }

    //Write global config
    QFile globConfig("updater.conf");
    if(globConfig.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream globConfigStream(&globConfig);
        globConfigStream << "version=" << version << endl;
        globConfigStream << "modserverurl=" << modserverurl << endl;
        globConfigStream.flush();
        globConfig.close();
    }

}

//Really keep?
//void MainWorker::help(QString cmd) {
//    msgOut << "You need to specify a valid option for the updater" << endl;
//    msgOut << "Available options are:" << endl;
//    msgOut << "init - Initiates the updater configs" << endl;
//    msgOut << "backup - Create a backup of the files and config" << endl;
//    msgOut << "updateall - Checks everything for updates" << endl;
//    msgOut << "updateme - update the updater" << endl;
//    msgOut << "updateacr - Checks for updates to ACR" << endl;
//    msgOut << "updatemods - Check for mod updates" << endl;
//    msgOut << "installmods <mod1>, <mod2>... - Comma separated list of mods to install" << endl;
//    msgOut << "listmods - Gets the mod list from the server with versions" << endl;
//    msgOut << "installlocalmod <file> - installs a zipfile as a mod" << endl;
//    msgOut << "removemods <mod1>, <mod2>... - Comma separated list of mods to remove" << endl;
//    msgOut << "installedmods - Prints out the installed mods along with versions" << endl;
//    msgOut << "runclient - runs the client" << endl;
//    msgOut << "runserver - runs the server" << endl;
//    //msgOut << "help <option> - Explains how to use <option> or if excluded gives you this dialogue";
//    return;
//}


// File operations //

//Simple zip file stuff
//Just wrappers for quazip functions (which are wrappers themselves?)
QString MainWorker::createZipfile(QStringList files, QString name = NULL) {
    QDir tmp;
    tmp.mkdir(name);
    foreach(QString s, files) {
        QFileInfo fi(s);
        if(fi.isFile()) {
            QFile::copy(s, name+"/"+s);
        } else if(fi.isDir()) {
            tmp.mkdir(name+"/"+s);
            rec_copy(QDir(s), QDir(name+"/"+s));
        }
    }
    if(!JlCompress::compressDir(name+".zip", name)) {
        return "Error writing archive!";
    }
    rec_rem(QDir(name));
    return "Archive written!";
}
QString MainWorker::extractZipfile(QString zipfile, QString target) {
    if(JlCompress::extractDir(zipfile, target).size() <= 0) {
        return "Error extracting archive!";
    }
    return "Archive extracted!";
}

//Recursive copy
//ugly, but somehow not existing in Qt function library (to be verified)
void MainWorker::rec_copy(QDir folder, QDir current) {
    QDir tmp;
    foreach(QString s, folder.entryList()) {
        if(s.startsWith(".")) {
            continue; // I dont want no hidden unix files (esp. "." and "..")
        }
        QFileInfo fi(folder.filePath(s));
        if(fi.isFile()) {
            QFile::copy(folder.filePath(s), current.filePath(s));
        } else if(fi.isDir()) {
            tmp.mkdir(current.path()+"/"+ s);
            rec_copy(QDir(folder.filePath(s)), QDir(current.path()+"/"+s));
        }
    }
}

//Recursive delete
//(look above)
void MainWorker::rec_rem(QDir folder) {
    QDir tmp;
    foreach(QString s, folder.entryList()) {
        if(s.startsWith(".")) {
            continue;
        }
        QFileInfo fi(folder.filePath(s));
        if(fi.isFile()) {
            QFile::remove(folder.filePath(s));
        } else if(fi.isDir()) {
            rec_rem(QDir(folder.filePath(s)));
        }
    }
    tmp.rmdir(folder.path());
}
