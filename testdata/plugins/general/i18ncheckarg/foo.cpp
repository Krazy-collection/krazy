#include <KLocalizedString>

int main(int argc, char** argv) {

    i18n("String without comment"); //i18ncheckarg (missing KUIT context marker)

    i18nc("Free context", "String without rol"); //i18ncheckarg (missing KUIT context marker)

    //KDE::DoNotExtract has a special meaning only in XML files
    i18nc("KDE::DoNotExtract", "String without rol (DoNotExtract)"); //i18ncheckarg (missing KUIT context marker)

    i18nc("@action", "String without free context"); //ok

    i18nc("@action Free context", "String"); //ok

    i18nc("@action KDE::DoNotExtract", "String (DoNotExtract)"); //ok

    //Ambiguous string without comment
    i18n("Name"); //i18ncheckarg (missing KUIT context marker, ambiguous message)

    //Ambiguous string without rol
    i18nc("Free context", "Name"); //i18ncheckarg (missing KUIT context marker)

    //Ambiguous string without rol (DoNotExtract)
    i18nc("KDE::DoNotExtract", "Name"); //i18ncheckarg (missing KUIT context marker)

    //Ambiguous string without free context
    i18nc("@action", "Name"); //i18ncheckarg (ambiguous message)

    //Ambiguous string
    i18nc("@action Free context", "Name"); //ok

    //Ambiguous string (DoNotExtract)
    i18nc("@action KDE::DoNotExtract", "Name"); //ok

}
