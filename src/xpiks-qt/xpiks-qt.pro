TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11

QT += qml quick widgets concurrent svg
CONFIG += qtquickcompiler
CONFIG += c++11

VERSION = 1.4.2.0
QMAKE_TARGET_PRODUCT = Xpiks
QMAKE_TARGET_DESCRIPTION = "Cross-Platform Image Keywording Software"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2014-2017 Taras Kushnir"

SOURCES += main.cpp \
    Models/artitemsmodel.cpp \
    Models/artworkmetadata.cpp \
    Helpers/globalimageprovider.cpp \
    Models/artworksrepository.cpp \
    Models/combinedartworksmodel.cpp \
    Helpers/indiceshelper.cpp \
    Models/artworkuploader.cpp \
    Models/artworksprocessor.cpp \
    Models/uploadinforepository.cpp \
    ../../vendors/tiny-aes/aes.cpp \
    ../../vendors/sqlite/sqlite3.c \
    Encryption/secretsmanager.cpp \
    Helpers/stringhelper.cpp \
    Commands/commandmanager.cpp \
    UndoRedo/undoredomanager.cpp \
    Commands/addartworkscommand.cpp \
    UndoRedo/addartworksitem.cpp \
    Commands/removeartworkscommand.cpp \
    UndoRedo/removeartworksitem.cpp \
    UndoRedo/artworkmetadatabackup.cpp \
    UndoRedo/modifyartworkshistoryitem.cpp \
    Commands/combinededitcommand.cpp \
    Commands/pastekeywordscommand.cpp \
    Helpers/runguard.cpp \
    Encryption/aes-qt.cpp \
    Models/ziparchiver.cpp \
    Helpers/ziphelper.cpp \
    Suggestion/keywordssuggestor.cpp \
    Models/settingsmodel.cpp \
    Helpers/loggingworker.cpp \
    Helpers/logger.cpp \
    Models/logsmodel.cpp \
    Models/filteredartitemsproxymodel.cpp \
    Helpers/helpersqmlwrapper.cpp \
    Models/recentdirectoriesmodel.cpp \
    Connectivity/updateservice.cpp \
    SpellCheck/spellcheckerservice.cpp \
    SpellCheck/spellcheckitem.cpp \
    SpellCheck/spellcheckworker.cpp \
    SpellCheck/spellchecksuggestionmodel.cpp \
    Common/basickeywordsmodel.cpp \
    SpellCheck/spellcheckerrorshighlighter.cpp \
    SpellCheck/spellcheckiteminfo.cpp \
    SpellCheck/spellsuggestionsitem.cpp \
    Connectivity/telemetryservice.cpp \
    Connectivity/updatescheckerworker.cpp \
    Warnings/warningscheckingworker.cpp \
    MetadataIO/metadataiocoordinator.cpp \
    Connectivity/testconnection.cpp \
    Connectivity/ftphelpers.cpp \
    Plugins/pluginmanager.cpp \
    Plugins/pluginwrapper.cpp \
    Plugins/pluginactionsmodel.cpp \
    Plugins/uiprovider.cpp \
    Warnings/warningsservice.cpp \
    Helpers/loghighlighter.cpp \
    Warnings/warningsmodel.cpp \
    Models/languagesmodel.cpp \
    Helpers/filterhelpers.cpp \
    QMLExtensions/triangleelement.cpp \
    Suggestion/shutterstockqueryengine.cpp \
    Suggestion/locallibraryqueryengine.cpp \
    Suggestion/fotoliaqueryengine.cpp \
    QMLExtensions/colorsmodel.cpp \
    Helpers/remoteconfig.cpp \
    Helpers/localconfig.cpp \
    Helpers/jsonhelper.cpp \
    AutoComplete/autocompleteworker.cpp \
    AutoComplete/autocompleteservice.cpp \
    Suggestion/gettyqueryengine.cpp \
    Models/abstractconfigupdatermodel.cpp \
    AutoComplete/stocksftplistmodel.cpp \
    Models/imageartwork.cpp \
    Common/flags.cpp \
    Models/proxysettings.cpp \
    QMLExtensions/imagecachingworker.cpp \
    QMLExtensions/imagecachingservice.cpp \
    QMLExtensions/cachingimageprovider.cpp \
    Commands/findandreplacecommand.cpp \
    Helpers/metadatahighlighter.cpp \
    Models/findandreplacemodel.cpp \
    Commands/deletekeywordscommand.cpp \
    Models/deletekeywordsviewmodel.cpp \
    Models/artworksviewmodel.cpp \
    Helpers/keywordshelpers.cpp \
    Connectivity/uploadwatcher.cpp \
    Connectivity/telemetryworker.cpp \
    Warnings/warningssettingsmodel.cpp \
    Connectivity/simplecurlrequest.cpp \
    Connectivity/curlinithelper.cpp \
    Connectivity/simplecurldownloader.cpp \
    Helpers/updatehelpers.cpp \
    Common/basicmetadatamodel.cpp \
    KeywordsPresets/presetkeywordsmodel.cpp \
    KeywordsPresets/presetkeywordsmodelconfig.cpp \
    QMLExtensions/folderelement.cpp \
    Models/artworkproxymodel.cpp \
    Models/artworkproxybase.cpp \
    Translation/translationservice.cpp \
    Translation/translationmanager.cpp \
    Translation/translationworker.cpp \
    Translation/translationquery.cpp \
    Models/uimanager.cpp \
    Plugins/sandboxeddependencies.cpp \
    Commands/expandpresetcommand.cpp \
    QuickBuffer/currenteditableartwork.cpp \
    QuickBuffer/currenteditableproxyartwork.cpp \
    QuickBuffer/quickbuffer.cpp \
    SpellCheck/userdicteditmodel.cpp \
    Helpers/asynccoordinator.cpp \
    QMLExtensions/tabsmodel.cpp \
    Models/recentitemsmodel.cpp \
    Models/recentfilesmodel.cpp \
    Models/videoartwork.cpp \
    Maintenance/maintenanceworker.cpp \
    Maintenance/maintenanceservice.cpp \
    Maintenance/logscleanupjobitem.cpp \
    Maintenance/updatescleanupjobitem.cpp \
    Maintenance/launchexiftooljobitem.cpp \
    Maintenance/initializedictionariesjobitem.cpp \
    Maintenance/movesettingsjobitem.cpp \
    QMLExtensions/videocachingservice.cpp \
    QMLExtensions/videocachingworker.cpp \
    QMLExtensions/artworksupdatehub.cpp \
    Models/keyvaluelist.cpp \
    Helpers/filehelpers.cpp \
    Helpers/artworkshelpers.cpp \
    Models/sessionmanager.cpp \
    Maintenance/savesessionjobitem.cpp \
    Connectivity/switcherconfig.cpp \
    Models/switchermodel.cpp \
    Connectivity/requestsworker.cpp \
    Connectivity/requestsservice.cpp \
    Helpers/database.cpp \
    Common/statefulentity.cpp \
    QMLExtensions/cachedimage.cpp \
    QMLExtensions/dbimagecacheindex.cpp \
    Maintenance/moveimagecachejobitem.cpp \
    QMLExtensions/cachedvideo.cpp \
    QMLExtensions/dbvideocacheindex.cpp \
    MetadataIO/cachedartwork.cpp \
    MetadataIO/metadatacache.cpp \
    MetadataIO/metadataioworker.cpp \
    MetadataIO/metadataioservice.cpp \
    MetadataIO/artworkssnapshot.cpp \
    Connectivity/connectivityrequest.cpp \
    MetadataIO/metadatareadinghub.cpp \
    AutoComplete/libfacecompletionengine.cpp \
    AutoComplete/autocompletemodel.cpp \
    AutoComplete/keywordsautocompletemodel.cpp \
    AutoComplete/stringsautocompletemodel.cpp \
    AutoComplete/presetscompletionengine.cpp \
    SpellCheck/duplicatesreviewmodel.cpp \
    SpellCheck/duplicateshighlighter.cpp \
    MetadataIO/csvexportworker.cpp \
    MetadataIO/csvexportplansmodel.cpp \
    MetadataIO/csvexportproperties.cpp \
    MetadataIO/csvexportmodel.cpp

RESOURCES += qml.qrc

DEFINES += QT_NO_CAST_TO_ASCII \
           QT_RESTRICTED_CAST_FROM_ASCII \
           QT_NO_CAST_FROM_BYTEARRAY
DEFINES += HUNSPELL_STATIC
DEFINES += QT_MESSAGELOGCONTEXT

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

ICON = xpiks-qt.icns

RC_ICONS = xpiks-qt.ico

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    Models/artitemsmodel.h \
    Models/artworkmetadata.h \
    Helpers/globalimageprovider.h \
    Models/artworksrepository.h \
    Helpers/indiceshelper.h \
    Helpers/clipboardhelper.h \
    Models/combinedartworksmodel.h \
    Common/abstractlistmodel.h \
    Helpers/constants.h \
    Models/artworkuploader.h \
    Models/artworksprocessor.h \
    Models/uploadinfo.h \
    Models/exportinfo.h \
    Models/uploadinforepository.h \
    Models/logsmodel.h \
    Encryption/aes-qt.h \
    ../../vendors/tiny-aes/aes.h \
    ../../vendors/sqlite/sqlite3.h \
    Encryption/secretsmanager.h \
    Helpers/stringhelper.h \
    Helpers/logger.h \
    Commands/commandmanager.h \
    UndoRedo/historyitem.h \
    UndoRedo/undoredomanager.h \
    UndoRedo/addartworksitem.h \
    Commands/commandbase.h \
    Commands/addartworkscommand.h \
    Common/baseentity.h \
    Commands/removeartworkscommand.h \
    UndoRedo/removeartworksitem.h \
    UndoRedo/artworkmetadatabackup.h \
    UndoRedo/modifyartworkshistoryitem.h \
    Commands/combinededitcommand.h \
    Commands/pastekeywordscommand.h \
    Helpers/runguard.h \
    Models/ziparchiver.h \
    Helpers/ziphelper.h \
    Common/basickeywordsmodel.h \
    Suggestion/keywordssuggestor.h \
    Suggestion/suggestionartwork.h \
    Models/settingsmodel.h \
    Helpers/loggingworker.h \
    Common/defines.h \
    Models/filteredartitemsproxymodel.h \
    Common/flags.h \
    Helpers/helpersqmlwrapper.h \
    Models/recentdirectoriesmodel.h \
    Common/version.h \
    Connectivity/updateservice.h \
    SpellCheck/spellcheckerservice.h \
    SpellCheck/spellcheckitem.h \
    SpellCheck/spellcheckworker.h \
    SpellCheck/spellchecksuggestionmodel.h \
    SpellCheck/spellcheckerrorshighlighter.h \
    SpellCheck/spellcheckiteminfo.h \
    Common/itemprocessingworker.h \
    SpellCheck/spellsuggestionsitem.h \
    Connectivity/analyticsuserevent.h \
    Connectivity/telemetryservice.h \
    Connectivity/updatescheckerworker.h \
    Warnings/warningscheckingworker.h \
    Warnings/warningsitem.h \
    MetadataIO/metadataiocoordinator.h \
    Connectivity/testconnection.h \
    Connectivity/ftphelpers.h \
    Plugins/xpiksplugininterface.h \
    Commands/icommandmanager.h \
    Commands/icommandbase.h \
    UndoRedo/ihistoryitem.h \
    UndoRedo/iundoredomanager.h \
    Plugins/pluginmanager.h \
    Plugins/pluginwrapper.h \
    Plugins/ipluginaction.h \
    Plugins/pluginactionsmodel.h \
    Plugins/uiprovider.h \
    Plugins/iuiprovider.h \
    Common/ibasicartwork.h \
    Common/iartworkssource.h \
    Warnings/warningsservice.h \
    Common/iservicebase.h \
    Helpers/loghighlighter.h \
    Warnings/warningsmodel.h \
    Models/languagesmodel.h \
    Helpers/filterhelpers.h \
    Connectivity/iftpcoordinator.h \
    QMLExtensions/triangleelement.h \
    Suggestion/shutterstockqueryengine.h \
    Suggestion/locallibraryqueryengine.h \
    Suggestion/suggestionqueryenginebase.h \
    Helpers/ifilenotavailablemodel.h \
    Suggestion/fotoliaqueryengine.h \
    QMLExtensions/colorsmodel.h \
    Helpers/remoteconfig.h \
    Helpers/localconfig.h \
    Helpers/jsonhelper.h \
    Helpers/comparevaluesjson.h \
    AutoComplete/autocompleteworker.h \
    AutoComplete/completionquery.h \
    AutoComplete/autocompleteservice.h \
    Suggestion/gettyqueryengine.h \
    Models/abstractconfigupdatermodel.h \
    AutoComplete/stocksftplistmodel.h \
    Models/imageartwork.h \
    Common/hold.h \
    Models/proxysettings.h \
    QMLExtensions/imagecachingworker.h \
    QMLExtensions/imagecacherequest.h \
    QMLExtensions/imagecachingservice.h \
    QMLExtensions/cachingimageprovider.h \
    Commands/findandreplacecommand.h \
    Helpers/metadatahighlighter.h \
    Models/findandreplacemodel.h \
    Commands/deletekeywordscommand.h \
    Models/deletekeywordsviewmodel.h \
    Models/artworksviewmodel.h \
    Helpers/keywordshelpers.h \
    Connectivity/uploadwatcher.h \
    Common/iflagsprovider.h \
    Connectivity/telemetryworker.h \
    Warnings/warningssettingsmodel.h \
    Connectivity/simplecurlrequest.h \
    Connectivity/curlinithelper.h \
    Connectivity/simplecurldownloader.h \
    Connectivity/apimanager.h \
    Helpers/updatehelpers.h \
    Common/basicmetadatamodel.h \
    KeywordsPresets/presetkeywordsmodel.h \
    KeywordsPresets/presetkeywordsmodelconfig.h \
    QMLExtensions/folderelement.h \
    Models/artworkproxymodel.h \
    Models/artworkproxybase.h \
    Common/imetadataoperator.h \
    Translation/translationservice.h \
    Translation/translationmanager.h \
    Translation/translationworker.h \
    Translation/translationquery.h \
    Models/uimanager.h \
    Plugins/sandboxeddependencies.h \
    Commands/expandpresetcommand.h \
    QuickBuffer/icurrenteditable.h \
    QuickBuffer/currenteditableartwork.h \
    QuickBuffer/currenteditableproxyartwork.h \
    QuickBuffer/quickbuffer.h \
    KeywordsPresets/ipresetsmanager.h \
    SpellCheck/userdicteditmodel.h \
    Helpers/asynccoordinator.h \
    QMLExtensions/tabsmodel.h \
    Models/recentitemsmodel.h \
    Models/recentfilesmodel.h \
    Models/videoartwork.h \
    Maintenance/maintenanceworker.h \
    Maintenance/maintenanceservice.h \
    Maintenance/imaintenanceitem.h \
    Maintenance/logscleanupjobitem.h \
    Maintenance/updatescleanupjobitem.h \
    Maintenance/launchexiftooljobitem.h \
    Maintenance/initializedictionariesjobitem.h \
    Maintenance/movesettingsjobitem.h \
    QMLExtensions/videocachingservice.h \
    QMLExtensions/videocachingworker.h \
    QMLExtensions/videocacherequest.h \
    QMLExtensions/artworksupdatehub.h \
    QMLExtensions/artworkupdaterequest.h \
    Models/keyvaluelist.h \
    Helpers/filehelpers.h \
    Helpers/artworkshelpers.h \
    Models/sessionmanager.h \
    Maintenance/savesessionjobitem.h \
    Connectivity/switcherconfig.h \
    Models/switchermodel.h \
    Connectivity/requestsworker.h \
    Connectivity/requestsservice.h \
    Warnings/iwarningsitem.h \
    AutoComplete/completionitem.h \
    Helpers/database.h \
    AutoComplete/completionitem.h \
    Common/statefulentity.h \
    QMLExtensions/previewstorage.h \
    QMLExtensions/cachedimage.h \
    QMLExtensions/dbimagecacheindex.h \
    Maintenance/moveimagecachejobitem.h \
    QMLExtensions/dbcacheindex.h \
    QMLExtensions/cachedvideo.h \
    QMLExtensions/dbvideocacheindex.h \
    MetadataIO/cachedartwork.h \
    MetadataIO/metadatacache.h \
    Common/readerwriterqueue.h \
    MetadataIO/metadataioworker.h \
    MetadataIO/metadataiotask.h \
    MetadataIO/metadataioservice.h \
    MetadataIO/originalmetadata.h \
    Suggestion/searchquery.h \
    Suggestion/locallibraryquery.h \
    MetadataIO/artworkssnapshot.h \
    Connectivity/connectivityrequest.h \
    MetadataIO/metadatareadinghub.h \
    AutoComplete/completionenginebase.h \
    AutoComplete/libfacecompletionengine.h \
    Common/wordanalysisresult.h \
    AutoComplete/autocompletemodel.h \
    AutoComplete/keywordsautocompletemodel.h \
    AutoComplete/stringsautocompletemodel.h \
    AutoComplete/presetscompletionengine.h \
    Common/keyword.h \
    SpellCheck/duplicatesreviewmodel.h \
    SpellCheck/duplicateshighlighter.h \
    MetadataIO/csvexportworker.h \
    MetadataIO/csvexportproperties.h \
    MetadataIO/csvexportplansmodel.h \
    MetadataIO/csvexportmodel.h \
    Common/delayedactionentity.h \
    Models/artworkelement.h \
    Models/previewartworkelement.h

DISTFILES += \
    Components/CloseIcon.qml \
    Components/EditableTags.qml \
    StyledControls/SimpleProgressBar.qml \
    StyledControls/StyledButton.qml \
    StyledControls/StyledCheckbox.qml \
    StyledControls/StyledScrollView.qml \
    StyledControls/StyledText.qml \
    StyledControls/StyledTextInput.qml \
    Dialogs/ExportMetadata.qml \
    Dialogs/ImportMetadata.qml \
    Dialogs/SettingsWindow.qml \
    Dialogs/UploadArtworks.qml \
    Dialogs/LogsDialog.qml \
    StyledControls/StyledTextEdit.qml \
    Dialogs/WarningsDialog.qml \
    xpiks-qt.ico \
    Dialogs/AboutWindow.qml \
    StyledControls/StyledAddHostButton.qml \
    Dialogs/EnterMasterPasswordDialog.qml \
    Dialogs/MasterPasswordSetupDialog.qml \
    Components/CheckedComponent.qml \
    Dialogs/ZipArtworksDialog.qml \
    Components/AddIcon.qml \
    Dialogs/KeywordsSuggestion.qml \
    Components/LargeAddIcon.qml \
    StyledControls/StyledTabView.qml \
    Components/CustomBorder.qml \
    StyledControls/StyledBlackButton.qml \
    StyledControls/StyledSlider.qml \
    Components/KeywordWrapper.qml \
    Components/CustomScrollbar.qml \
    Dialogs/UpdateWindow.qml \
    Dialogs/SpellCheckSuggestionsDialog.qml \
    Components/SuggestionWrapper.qml \
    Dialogs/EditKeywordDialog.qml \
    Dialogs/PlainTextKeywordsDialog.qml \
    Dialogs/WhatsNewDialog.qml \
    Dialogs/TermsAndConditionsDialog.qml \
    Dialogs/PluginsDialog.qml \
    Components/LayoutButton.qml \
    Graphics/vector-icon.svg \
    Constants/UIConfig.js \
    Components/SelectedIcon.qml \
    Components/CustomComboBox.qml \
    Components/CompletionBox.qml \
    Components/EditIcon.qml \
    uncrustify.cfg \
    Components/PresentationSlide.qml \
    Graphics/faileduploads.png \
    Graphics/gears.png \
    Graphics/findandreplace.png \
    Graphics/presets.png \
    Graphics/quickbuffer.png \
    Graphics/translator.png \
    Dialogs/ProxySetupDialog.qml \
    Dialogs/WhatsNewMinorDialog.qml \
    Dialogs/FindAndReplace.qml \
    Dialogs/ReplacePreview.qml \
    Dialogs/DeleteKeywordsDialog.qml \
    Dialogs/FailedUploadArtworks.qml \
    Dialogs/InstallUpdateDialog.qml \
    Components/CustomTab.qml \
    StackViews/MainGrid.qml \
    StackViews/ArtworkEditView.qml \
    Components/ZoomAmplifier.qml \
    StackViews/WarningsView.qml \
    Components/GlyphButton.qml \
    Components/BackGlyphButton.qml \
    StackViews/CombinedEditView.qml \
    Dialogs/PresetsEditDialog.qml \
    Dialogs/TranslationPreviewDialog.qml \
    CollapserTabs/FilesFoldersTab.qml \
    CollapserTabs/TranslatorTab.qml \
    CollapserTabs/FilesFoldersIcon.qml \
    CollapserTabs/TranslatorIcon.qml \
    CollapserTabs/QuickBufferIcon.qml \
    CollapserTabs/QuickBufferTab.qml \
    Dialogs/UserDictEditDialog.qml \
    Components/LoaderIcon.qml \
    Graphics/Icon_donate_black.svg \
    Graphics/Icon_donate_slategray.svg \
    Dialogs/DonateDialog.qml \
    Components/DonateComponent.qml \
    StackViews/DuplicatesReView.qml \
    StyledControls/StyledLink.qml \
    Dialogs/CsvExportDialog.qml \
    Components/DotsButton.qml

lupdate_only {
SOURCES += *.qml \
          *.js \
          Components/*.qml \
          Dialogs/*.qml \
          StyledControls/*.qml \
          CollapserTabs/*.qml \
          StackViews/*.qml
}

INCLUDEPATH += ../../vendors/tiny-aes
INCLUDEPATH += ../../vendors/cpp-libface
INCLUDEPATH += ../../vendors/ssdll/src/ssdll
INCLUDEPATH += ../../vendors/hunspell-1.6.0/src
INCLUDEPATH += ../../vendors/libthmbnlr
INCLUDEPATH += ../../vendors/libxpks

CONFIG(debug, debug|release)  {
    LIBS += -L"$$PWD/../../libs/debug"
} else {
    LIBS += -L"$$PWD/../../libs/release"
}

LIBS += -lhunspell
LIBS += -lcurl
LIBS += -lface
LIBS += -lssdll
LIBS += -lquazip
LIBS += -lz
LIBS += -lthmbnlr
LIBS += -lxpks

BUILDNO=$$system(git log -n 1 --pretty=format:"%h")
BRANCH_NAME=$$system(git rev-parse --abbrev-ref HEAD)

CONFIG(debug, debug|release)  {
    message("Building debug")
    DEFINES += WITH_PLUGINS
    DEFINES += WITH_LOGS
    DEFINES += WITH_STDOUT_LOGS
    #QMAKE_CXXFLAGS += -fsanitize=thread
} else {
    DEFINES += WITH_LOGS
    message("Building release")
}

macx {
    INCLUDEPATH += "../../vendors/quazip"
    INCLUDEPATH += "../../vendors/libcurl/include"

    HUNSPELL_DICT_FILES.files = deps/dict/en_US.aff deps/dict/en_US.dic deps/dict/license.txt deps/dict/README_en_US.txt
    HUNSPELL_DICT_FILES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += HUNSPELL_DICT_FILES

    WHATS_NEW.files = deps/whatsnew.txt
    WHATS_NEW.path = Contents/Resources
    QMAKE_BUNDLE_DATA += WHATS_NEW

    TERMS_AND_CONDITIONS.files = deps/terms_and_conditions.txt
    TERMS_AND_CONDITIONS.path = Contents/Resources
    QMAKE_BUNDLE_DATA += TERMS_AND_CONDITIONS

    TRANSLATIONS_FILES_LIST = $$system(ls $$PWD/deps/translations/*.qm)
    XPIKS_TRANSLATIONS.files = $$TRANSLATIONS_FILES_LIST
    XPIKS_TRANSLATIONS.path = Contents/Resources/translations
    QMAKE_BUNDLE_DATA += XPIKS_TRANSLATIONS

    FREQ_TABLES.files = deps/en_wordlist.tsv
    FREQ_TABLES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += FREQ_TABLES
}

win32 {
    DEFINES += QT_NO_PROCESS_COMBINED_ARGUMENT_START
    QT += winextras
    DEFINES += ZLIB_WINAPI \
               ZLIB_DLL
    INCLUDEPATH += "../../vendors/zlib-1.2.11"
    INCLUDEPATH += "../../vendors/quazip"
    INCLUDEPATH += "../../vendors/libcurl/include"

    LIBS -= -lcurl

    CONFIG(debug, debug|release) {
	EXE_DIR = debug
        LIBS += -llibcurl_debug
        LIBS -= -lquazip
        LIBS += -lquazipd
    }

    CONFIG(release, debug|release) {
        EXE_DIR = release
        LIBS += -llibcurl
    }

    LIBS += -lmman

    TR_DIR = translations

    exists($$OUT_PWD/$$EXE_DIR/$$TR_DIR/) {
        message("Translations exist")
    } else {
        createtranslations.commands += $(MKDIR) \"$$shell_path($$OUT_PWD/$$EXE_DIR/$$TR_DIR)\"
        QMAKE_EXTRA_TARGETS += createtranslations
        POST_TARGETDEPS += createtranslations
    }

    AC_SOURCES_DIR = ac_sources

    exists($$OUT_PWD/$$EXE_DIR/$$AC_SOURCES_DIR/) {
        message("ac_sources exist")
    } else {
        create_ac_sources.commands += $(MKDIR) \"$$shell_path($$OUT_PWD/$$EXE_DIR/$$AC_SOURCES_DIR)\"
        QMAKE_EXTRA_TARGETS += create_ac_sources
        POST_TARGETDEPS += create_ac_sources
    }

    copywhatsnew.commands = $(COPY_FILE) \"$$shell_path($$PWD/deps/whatsnew.txt)\" \"$$shell_path($$OUT_PWD/$$EXE_DIR/)\"
    copyterms.commands = $(COPY_FILE) \"$$shell_path($$PWD/deps/terms_and_conditions.txt)\" \"$$shell_path($$OUT_PWD/$$EXE_DIR/)\"
    copydicts.commands = $(COPY_DIR) \"$$shell_path($$PWD/deps/dict)\" \"$$shell_path($$OUT_PWD/$$EXE_DIR/dict)\"

    appveyor {
        DEFINES += WITH_LOGS
        LIBS += -L"$$PWD/../../libs"
        copytranslations.commands = echo "Skip translations"
    } else {
        copytranslations.commands = $(COPY_FILE) \"$$shell_path($$PWD/deps/$$TR_DIR/xpiks_*.qm)\" \"$$shell_path($$OUT_PWD/$$EXE_DIR/$$TR_DIR/)\"
    }

    copyfreqtables.commands = $(COPY_FILE) \"$$shell_path($$PWD/deps/en_wordlist.tsv)\" \"$$shell_path($$OUT_PWD/$$EXE_DIR/$$AC_SOURCES_DIR/)\"

    QMAKE_EXTRA_TARGETS += copywhatsnew copyterms copydicts copytranslations copyfreqtables
    POST_TARGETDEPS += copywhatsnew copyterms copydicts copytranslations copyfreqtables
}

travis-ci {
    message("for Travis CI")
    LIBS += -L"$$PWD/../../libs"
    LIBS -= -lz
    LIBS += /usr/lib/x86_64-linux-gnu/libz.so
    LIBS += -ldl
    DEFINES += TRAVIS_CI
    INCLUDEPATH += "../../vendors/quazip"

    LIBS -= -lthmbnlr
    SOURCES += ../../vendors/libthmbnlr/thumbnailcreator_stub.cpp
}

linux-g++-64 {
    message("for Linux")
    target.path=/usr/bin/

    QML_IMPORT_PATH += /usr/lib/x86_64-linux-gnu/qt5/imports/
    LIBS += -L/lib/x86_64-linux-gnu/
    BUILDNO = $$system($$PWD/buildno.sh)

    UNAME = $$system(cat /proc/version | tr -d \'()\')
    
    contains( UNAME, Debian|Ubuntu ) {
        message("distribution : Debian")
        LIBS -= -lquazip
        LIBS += -lquazip5
    }
    
    contains( UNAME, SUSE ) {
        message("distribution : SUSE")
    }
}

linux-qtcreator {
    message("in QtCreator")
    BUILDNO = $$system($$PWD/buildno.sh)
    LIBS += -L/usr/lib64/
    LIBS += -L/lib/x86_64-linux-gnu/
    copywhatsnew.commands = $(COPY_FILE) "$$PWD/deps/whatsnew.txt" "$$OUT_PWD/"
    copyterms.commands = $(COPY_FILE) "$$PWD/deps/terms_and_conditions.txt" "$$OUT_PWD/"
    QMAKE_EXTRA_TARGETS += copywhatsnew copyterms
    POST_TARGETDEPS += copywhatsnew copyterms
}

linux-static {
    CONFIG += static
    QTPLUGIN += qt5quick
    DEFINES += STATIC
    message("Static build.")
}

DEFINES += BUILDNUMBER=$${BUILDNO}
DEFINES += BRANCHNAME=$${BRANCH_NAME}
