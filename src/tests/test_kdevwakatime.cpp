#include <QJsonObject>
#include <QLoggingCategory>
#include <QProcess>
#include <QSignalSpy>
#include <QTest>

#include <tests/autotestshell.h>
#include <tests/testproject.h>
#include <tests/testcore.h>

#include <interfaces/iplugin.h>
#include <interfaces/iplugincontroller.h>
#include <interfaces/iruntimecontroller.h>
#include <interfaces/iruntime.h>

#include <KLocalizedString>
#include <KPluginFactory>

using namespace KDevelop;

class WakaTimeTest: public QObject
{
	Q_OBJECT
public:

	WakaTimeTest()
	{
		QLoggingCategory::setFilterRules(QStringLiteral("*.debug=false\ndefault.debug=true\nkdevelop.plugins.wakatime=true\n"));
	}

	IRuntime* m_initialRuntime = nullptr;

private Q_SLOTS:
	void initTestCase()
	{
		AutoTestShell::init({QStringLiteral("kdevwakatime"), QStringLiteral("KDevGenericManager")});
		TestCore::initialize();

		m_initialRuntime = ICore::self()->runtimeController()->currentRuntime();

		auto plugin = ICore::self()->pluginController()->loadPlugin("kdevwakatime");
		QVERIFY(plugin);
	}
};

QTEST_MAIN( WakaTimeTest )

#include "test_kdevwakatime.moc"
