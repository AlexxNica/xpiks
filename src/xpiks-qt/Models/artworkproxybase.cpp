/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2017 Taras Kushnir <kushnirTV@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "artworkproxybase.h"
#include "../Commands/commandmanager.h"
#include "../Suggestion/keywordssuggestor.h"
#include "../QuickBuffer/quickbuffer.h"
#include "../Helpers/stringhelper.h"
#include "../AutoComplete/completionitem.h"
#include "../AutoComplete/keywordsautocompletemodel.h"

namespace Models {
    QString ArtworkProxyBase::getDescription() {
        auto *basicModel = getBasicMetadataModel();
        return basicModel->getDescription();
    }

    QString ArtworkProxyBase::getTitle() {
        auto *basicModel = getBasicMetadataModel();
        return basicModel->getTitle();
    }

    QStringList ArtworkProxyBase::getKeywords() {
        auto *basicModel = getBasicMetadataModel();
        return basicModel->getKeywords();
    }

    int ArtworkProxyBase::getKeywordsCount() {
        auto *basicModel = getBasicMetadataModel();
        return basicModel->getKeywordsCount();
    }

    void ArtworkProxyBase::setDescription(const QString &description) {
        if (doSetDescription(description)) {
            signalDescriptionChanged();
            doJustEdited();
        }
    }

    void ArtworkProxyBase::setTitle(const QString &title) {
        if (doSetTitle(title)) {
            signalTitleChanged();
            doJustEdited();
        }
    }

    void ArtworkProxyBase::setKeywords(const QStringList &keywords) {
        doSetKeywords(keywords);
        signalKeywordsCountChanged();
        doJustEdited();
    }

    bool ArtworkProxyBase::doSetDescription(const QString &description) {
        auto *metadataOperator = getMetadataOperator();
        return metadataOperator->setDescription(description);
    }

    bool ArtworkProxyBase::doSetTitle(const QString &title) {
        auto *metadataOperator = getMetadataOperator();
        return metadataOperator->setTitle(title);
    }

    void ArtworkProxyBase::doSetKeywords(const QStringList &keywords) {
        auto *metadataOperator = getMetadataOperator();
        metadataOperator->setKeywords(keywords);

        spellCheckKeywords();
    }

    Common::ID_t ArtworkProxyBase::getSpecialItemID() {
        return SPECIAL_ID_INVALID;
    }

    bool ArtworkProxyBase::doEditKeyword(int index, const QString &replacement) {
        LOG_INFO << "index:" << index << "replacement:" << replacement;
        auto *metadataOperator = getMetadataOperator();
        bool result = metadataOperator->editKeyword(index, replacement);
        if (result) {
            auto *basicModel = getBasicMetadataModel();
            m_CommandManager->submitKeywordForSpellCheck(basicModel, index);
            doJustEdited();
        } else {
            LOG_INFO << "Failed to edit to" << replacement;
        }

        return result;
    }

    bool ArtworkProxyBase::doRemoveKeywordAt(int keywordIndex, QString &keyword) {
        LOG_INFO << "keyword index:" << keywordIndex;
        auto *metadataOperator = getMetadataOperator();
        bool result = metadataOperator->removeKeywordAt(keywordIndex, keyword);
        if (result) {
            signalKeywordsCountChanged();
            LOG_INFO << "Removed keyword:" << keyword << "keywords count:" << getKeywordsCount();
            doJustEdited();
        }

        return result;
    }

    bool ArtworkProxyBase::doRemoveLastKeyword(QString &keyword) {
        LOG_DEBUG << "#";
        auto *metadataOperator = getMetadataOperator();
        bool result = metadataOperator->removeLastKeyword(keyword);
        if (result) {
            signalKeywordsCountChanged();
            LOG_INFO << "Removed keyword:" << keyword << "keywords count:" << getKeywordsCount();
            doJustEdited();
        }

        return result;
    }

    bool ArtworkProxyBase::doAppendKeyword(const QString &keyword) {
        LOG_INFO << keyword;
        auto *metadataOperator = getMetadataOperator();
        bool result = metadataOperator->appendKeyword(keyword);
        if (result) {
            signalKeywordsCountChanged();
            auto *basicModel = getBasicMetadataModel();
            m_CommandManager->submitKeywordForSpellCheck(basicModel, basicModel->rowCount() - 1);
            doJustEdited();
        } else {
            LOG_INFO << "Failed to append:" << keyword;
        }

        return result;
    }

    size_t ArtworkProxyBase::doAppendKeywords(const QStringList &keywords) {
        LOG_INFO << keywords.length() << "keyword(s)" << "|" << keywords;
        auto *metadataOperator = getMetadataOperator();
        const size_t appendedCount = metadataOperator->appendKeywords(keywords);
        LOG_INFO << "Appended" << appendedCount << "keywords";

        if (appendedCount > 0) {
            signalKeywordsCountChanged();

            auto *basicModel = getBasicMetadataModel();
            m_CommandManager->submitItemForSpellCheck(basicModel, Common::SpellCheckFlags::Keywords);
            doJustEdited();
        }

        return appendedCount;
    }

    bool ArtworkProxyBase::doRemoveKeywords(const QSet<QString> &keywords, bool caseSensitive) {
        LOG_INFO << "case sensitive:" << caseSensitive;
        auto *metadataOperator = getMetadataOperator();
        bool result = metadataOperator->removeKeywords(keywords, caseSensitive);
        if (result) {
            signalKeywordsCountChanged();

            // to update fix spelling link
            spellCheckKeywords();
            doJustEdited();
        }

        return result;
    }

    bool ArtworkProxyBase::doClearKeywords() {
        LOG_DEBUG << "#";
        auto *metadataOperator = getMetadataOperator();
        bool result = metadataOperator->clearKeywords();
        if (result) {
            signalKeywordsCountChanged();
        }

        // to update fix spelling link
        spellCheckKeywords();

        // do not request backup

        return result;
    }

    QString ArtworkProxyBase::doGetKeywordsString() {
        auto *metadataOperator = getMetadataOperator();
        return metadataOperator->getKeywordsString();
    }

    void ArtworkProxyBase::doSuggestCorrections() {
        LOG_DEBUG << "#";
        auto *metadataOperator = getMetadataOperator();
        m_CommandManager->setupSpellCheckSuggestions(metadataOperator, -1, Common::SuggestionFlags::All);
    }

    void ArtworkProxyBase::doSetupDuplicatesModel() {
        LOG_DEBUG << "#";
        auto *basicModel = getBasicMetadataModel();
        m_CommandManager->setupDuplicatesModel(basicModel);
    }

    QSyntaxHighlighter *ArtworkProxyBase::doCreateDescriptionHighligher(QQuickTextDocument *document) {
        auto *keywordsModel = getBasicMetadataModel();
        SpellCheck::SpellCheckItemInfo *info = keywordsModel->getSpellCheckInfo();

        QMLExtensions::ColorsModel *colorsModel = m_CommandManager->getColorsModel();
        auto *highlighter = info->createHighlighterForDescription(document->textDocument(), colorsModel, nullptr);
        return highlighter;
    }

    QSyntaxHighlighter *ArtworkProxyBase::doCreateTitleHighlighter(QQuickTextDocument *document) {
        auto *keywordsModel = getBasicMetadataModel();
        SpellCheck::SpellCheckItemInfo *info = keywordsModel->getSpellCheckInfo();

        QMLExtensions::ColorsModel *colorsModel = m_CommandManager->getColorsModel();
        auto *highlighter = info->createHighlighterForTitle(document->textDocument(), colorsModel, nullptr);
        return highlighter;
    }

    void ArtworkProxyBase::doSpellCheckDescription() {
        LOG_DEBUG << "#";
        auto *keywordsModel = getBasicMetadataModel();
        if (!keywordsModel->getDescription().trimmed().isEmpty()) {
            m_CommandManager->submitItemForSpellCheck(keywordsModel, Common::SpellCheckFlags::Description);
        } else {
            keywordsModel->notifySpellCheckResults(Common::SpellCheckFlags::Description);
        }
    }

    void ArtworkProxyBase::doSpellCheckTitle() {
        LOG_DEBUG << "#";
        auto *keywordsModel = getBasicMetadataModel();
        if (!keywordsModel->getTitle().trimmed().isEmpty()) {
            m_CommandManager->submitItemForSpellCheck(keywordsModel, Common::SpellCheckFlags::Title);
        } else {
            keywordsModel->notifySpellCheckResults(Common::SpellCheckFlags::Title);
        }
    }

    void ArtworkProxyBase::doPlainTextEdit(const QString &rawKeywords, bool spaceIsSeparator) {
        LOG_DEBUG << "#";

        QVector<QChar> separators;
        separators << QChar(',');
        if (spaceIsSeparator) { separators << QChar(' '); }
        QStringList keywords;
        Helpers::splitKeywords(rawKeywords.trimmed(), separators, keywords);

        auto *metadataOperator = getMetadataOperator();
        metadataOperator->setKeywords(keywords);

        spellCheckKeywords();

        signalKeywordsCountChanged();
        doJustEdited();
    }

    bool ArtworkProxyBase::getHasTitleWordSpellError(const QString &word) {
        auto *keywordsModel = getBasicMetadataModel();
        return keywordsModel->hasTitleWordSpellError(word);
    }

    bool ArtworkProxyBase::getHasDescriptionWordSpellError(const QString &word) {
        auto *keywordsModel = getBasicMetadataModel();
        return keywordsModel->hasDescriptionWordSpellError(word);
    }

    bool ArtworkProxyBase::doExpandPreset(int keywordIndex, int presetIndex) {
        bool success = false;
        LOG_INFO << "keyword" << keywordIndex << "preset" << presetIndex;
        auto *presetsModel = m_CommandManager->getPresetsModel();
        QStringList keywords;

        if (presetsModel->tryGetPreset(presetIndex, keywords)) {
            auto *metadataOperator = getMetadataOperator();
            if (metadataOperator->expandPreset(keywordIndex, keywords)) {
                signalKeywordsCountChanged();
                spellCheckKeywords();
                doJustEdited();
                success = true;
            }
        }

        return success;
    }

    bool ArtworkProxyBase::doAppendPreset(int presetIndex) {
        bool success = false;
        LOG_INFO << "preset" << presetIndex;
        auto *presetsModel = m_CommandManager->getPresetsModel();
        QStringList keywords;

        if (presetsModel->tryGetPreset(presetIndex, keywords)) {
            auto *metadataOperator = getMetadataOperator();
            if (metadataOperator->appendPreset(keywords)) {
                signalKeywordsCountChanged();
                spellCheckKeywords();
                doJustEdited();
                success = true;
            }
        }

        return success;
    }

    bool ArtworkProxyBase::doExpandLastKeywordAsPreset() {
        LOG_DEBUG << "#";
        bool success = false;

        auto *keywordsModel = getBasicMetadataModel();
        int keywordIndex = keywordsModel->getKeywordsCount() - 1;
        QString lastKeyword = keywordsModel->retrieveKeyword(keywordIndex);
        auto *presetsModel = m_CommandManager->getPresetsModel();
        int presetIndex = -1;

        if (presetsModel->tryFindSinglePresetByName(lastKeyword, false, presetIndex)) {
            success = doExpandPreset(keywordIndex, presetIndex);
        } else {
            LOG_DEBUG << "Preset not found";
        }

        return success;
    }

    bool ArtworkProxyBase::doAddPreset(int presetIndex) {
        LOG_INFO << presetIndex;
        bool success = false;
        auto *presetsModel = m_CommandManager->getPresetsModel();
        QStringList keywords;

        if (presetsModel->tryGetPreset(presetIndex, keywords)) {
            success = doAppendKeywords(keywords) > 0;
        }

        return success;
    }

    bool ArtworkProxyBase::doRemovePreset(int presetIndex) {
        LOG_INFO << presetIndex;
        bool success = false;
        auto *presetsModel = m_CommandManager->getPresetsModel();
        QStringList keywords;

        if (presetsModel->tryGetPreset(presetIndex, keywords)) {
            for (auto &keyword: keywords) {
                keyword = keyword.toLower();
            }

            success = doRemoveKeywords(keywords.toSet(), false);
        }

        return success;
    }

    void ArtworkProxyBase::doInitSuggestion() {
#ifndef CORE_TESTS
        auto *suggestor = m_CommandManager->getKeywordsSuggestor();
        auto *metadataModel = getBasicMetadataModel();
        suggestor->setExistingKeywords(metadataModel->getKeywordsSet());
#endif
    }

    void ArtworkProxyBase::doRegisterAsCurrentItem() {
        LOG_DEBUG << "#";
        m_CommandManager->registerCurrentItem(this);
    }

    void ArtworkProxyBase::doHandleUserDictChanged(const QStringList &keywords, bool overwritten) {
        LOG_DEBUG << "#";
        Q_ASSERT(!keywords.isEmpty());

        auto *metadataModel = getBasicMetadataModel();

        SpellCheck::SpellCheckItemInfo *info = metadataModel->getSpellCheckInfo();

        if (!overwritten) {
            info->removeWordsFromErrors(keywords);
            // special case after words added to dict
            std::vector<Common::BasicKeywordsModel *> items;
            items.push_back(metadataModel);
            m_CommandManager->submitForSpellCheck(items, keywords);
        }
        else {
            info->clear();
            m_CommandManager->submitItemForSpellCheck(metadataModel);
        }
    }

    void ArtworkProxyBase::doHandleUserDictCleared() {
        LOG_DEBUG << "#";
        auto *metadataModel = getBasicMetadataModel();
        m_CommandManager->submitItemForSpellCheck(metadataModel);
    }

    void ArtworkProxyBase::doCopyToQuickBuffer() {
        LOG_DEBUG << "#";
        auto *metadataModel = getBasicMetadataModel();
        auto *quickBuffer = m_CommandManager->getQuickBuffer();
        quickBuffer->setFromBasicModel(metadataModel);
    }

    bool ArtworkProxyBase::hasKeywords(const QStringList &keywordsList) {
        auto *metadataOperator = getMetadataOperator();
        bool result = metadataOperator->hasKeywords(keywordsList);
        return result;
    }

    void ArtworkProxyBase::doJustEdited() {
        auto *metadataOperator = getMetadataOperator();
        metadataOperator->justEdited();
    }

    void ArtworkProxyBase::doCheckSemanticDuplicates() {
        auto *basicArwork = getBasicMetadataModel();
        m_CommandManager->checkSemanticDuplicates(basicArwork);
    }

    void ArtworkProxyBase::doGenerateCompletions(const QString &prefix) {
        auto *basicArwork = getBasicMetadataModel();
        m_CommandManager->generateCompletions(prefix, basicArwork);
    }

    bool ArtworkProxyBase::doAcceptCompletionAsPreset(int completionID) {
        bool accepted = false;

        AutoComplete::KeywordsAutoCompleteModel *acModel = m_CommandManager->getAutoCompleteModel();
        std::shared_ptr<AutoComplete::CompletionItem> completionItem = acModel->getAcceptedCompletion(completionID);
        if (!completionItem) {
            LOG_WARNING << "Completion is not available anymore";
            return false;
        }

        const int presetIndex = completionItem->getPresetIndex();

        if (completionItem->isPreset() || (completionItem->canBePreset() && completionItem->shouldExpandPreset())) {
            accepted = doAddPreset(presetIndex);
        }/* --------- this is handled in the edit field -----------
            else if (completionItem->isKeyword()) {
            doAppendKeyword(completionItem->getCompletion());
            accepted = true;
        }*/

        return accepted;
    }

    void ArtworkProxyBase::spellCheckEverything() {
        auto *basicModel = getBasicMetadataModel();
        m_CommandManager->submitItemForSpellCheck(basicModel);
    }

    void ArtworkProxyBase::spellCheckKeywords() {
        auto *basicModel = getBasicMetadataModel();
        m_CommandManager->submitItemForSpellCheck(basicModel, Common::SpellCheckFlags::Keywords);
    }
}
