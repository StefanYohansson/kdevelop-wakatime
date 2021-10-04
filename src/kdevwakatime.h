#ifndef KDEVWAKATIME_H
#define KDEVWAKATIME_H

#include <interfaces/iplugin.h>

class kdevwakatime : public KDevelop::IPlugin
{
    Q_OBJECT

public:
    // KPluginFactory-based plugin wants constructor with this signature
    kdevwakatime(QObject* parent, const QVariantList& args);
};

#endif // KDEVWAKATIME_H
