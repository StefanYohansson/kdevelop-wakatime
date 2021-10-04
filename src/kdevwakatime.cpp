#include "kdevwakatime.h"

#include <debug.h>

#include <KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON(kdevwakatimeFactory, "kdevwakatime.json", registerPlugin<kdevwakatime>(); )

kdevwakatime::kdevwakatime(QObject *parent, const QVariantList& args)
    : KDevelop::IPlugin(QStringLiteral("kdevwakatime"), parent)
{
    Q_UNUSED(args);

    qCDebug(PLUGIN_KDEVWAKATIME) << "Hello world, my plugin is loaded!";
}

// needed for QObject class created from K_PLUGIN_FACTORY_WITH_JSON
#include "kdevwakatime.moc"
