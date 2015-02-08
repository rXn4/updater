#ifndef MODENTRY_H
#define MODENTRY_H

#include <QString>

//Class to symbolise a mod
class ModEntry {
public:
    //ugly
    QString Name;
    QString Author;
    QString Version;
    QString Description;
    QString Url;

    void clear() {
        Name.clear();
        Author.clear();
        Version.clear();
        Description.clear();
        Url.clear();
    }

    bool operator == (const ModEntry &compareTo) {
        if(compareTo.Name == this->Name &&
                compareTo.Author == this->Author &&
                compareTo.Version == this->Version &&
                compareTo.Description == this->Description &&
                compareTo.Url == this->Url) {
            return true;
        }
        return false;
    }

    QString toString() {
        return this->Name;
    }

};

#endif // MODENTRY_H
