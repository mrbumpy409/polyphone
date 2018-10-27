#include "toolfrequencypeaks.h"
#include "toolfrequencypeaks_gui.h"
#include "soundfontmanager.h"
#include "contextmanager.h"

ToolFrequencyPeaks::ToolFrequencyPeaks() : AbstractTool(new ToolFrequencyPeaks_parameters(), new ToolFrequencyPeaks_gui())
{

}

bool ToolFrequencyPeaks::isCompatible(IdList ids)
{
    return !ids.getSelectedIds(elementSmpl).empty();
}

void ToolFrequencyPeaks::run(SoundfontManager * sm, QWidget * parent, IdList ids, AbstractToolParameters * parameters)
{
    Q_UNUSED(sm)
    Q_UNUSED(ids)
    Q_UNUSED(parent)
    ToolFrequencyPeaks_parameters * params = (ToolFrequencyPeaks_parameters *) parameters;

    // Création fichier csv
    QFile file(params->getFilePath());
    if (!file.open(QIODevice::WriteOnly))
        return;

    // Header
    QString sep = trUtf8(";");
    QTextStream stream(&file);
    stream << "\"" << trUtf8("Échantillon") << "\"" << sep << "\"" << trUtf8("Pic") << "\"" << sep << "\""
           << trUtf8("Intensité") << "\"" << sep << "\"" << trUtf8("Fréquence") << "\"" << sep << "\""
           << trUtf8("Note") << "\"" << sep << "\"" << trUtf8("Correction") << "\"";

    // Data
    foreach (SampleFrequencyInfo sfi, params->getSampleFrequencies())
    {
        QString sampleName = sfi.name.replace(QRegExp(QString::fromUtf8("[`~*|:<>«»?/{}\"\\\\]")), "_");

        int number = 1;
        foreach (FrequencyInfo fi, sfi.frequencies)
        {
            stream << endl;
            stream << "\"" << sampleName << "\"" << sep;
            stream << number << sep;
            stream << QString::number(fi.factor).replace(".", trUtf8(",")) << sep;
            stream << QString::number(fi.frequency).replace(".", trUtf8(",")) << sep;
            stream << ContextManager::keyName()->getKeyName(fi.key) << sep;
            stream << QString::number(fi.correction);
            number++;
        }
    }
    file.close();
}

QString ToolFrequencyPeaks::getConfirmation()
{
    return trUtf8("");
}
