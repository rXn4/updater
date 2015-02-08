#include "configdata.h"

ConfigData::ConfigData()
{
    //ACR doesnt count as mod
    //installedMods.insert(installedMods.count(), "../acr");
}

int ConfigData::readConfig() {
    






        return SUCCESS;
}

//Read a config file for mods, wild mixture of mod downloading and global config code :P
int ConfigData::readModConfig() {
   
    
    
}


//Just wrappers for quazip functions (which are wrappers themselves?)
int ConfigData::extractZipfile(QString zipfile) {
    if(JlCompress::extractDir(zipfile).size() <= 0) {
        return OTHER_ERROR;
    }
    return SUCCESS;
}
int ConfigData::extractZipfile(QString zipfile, QString target) {
    if(JlCompress::extractDir(zipfile, target).size() <= 0) {
        return OTHER_ERROR;
    }
    return SUCCESS;
}

int ConfigData::createZipfile(QStringList files, QString name) {
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
        return WRITE_ERROR;
    }
    rec_rem(QDir(name));
    return SUCCESS;
}





// DEFAULT GETTERS AND SETTERS BELOW THIS LINE //

QString ConfigData::getVersion() const
{
    return version;
}

void ConfigData::setVersion(const QString &value)
{
    version = value;
}

QList<ModEntry> ConfigData::getInstalledMods() const
{
    return installedMods;
}

void ConfigData::setInstalledMods(const QList<ModEntry> &value)
{
    installedMods = value;
}

QList<ModEntry> ConfigData::getAvailableMods() const
{

    return availableMods;
}

void ConfigData::addToAvailableMods(ModEntry &value) {
    availableMods.append(value);
}

void ConfigData::setAvailableMods(const QList<ModEntry> &value)
{
    availableMods = value;
}
QString ConfigData::getModserverurl() const
{
    return modserverurl;
}

void ConfigData::setModserverurl(const QString &value)
{
    modserverurl = value;
}
