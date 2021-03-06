/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include "projectexplorer_export.h"

#include "runconfiguration.h"

#include <utils/environment.h>

#include <QList>
#include <QVariantMap>

namespace ProjectExplorer {

class PROJECTEXPLORER_EXPORT EnvironmentAspect : public ProjectConfigurationAspect
{
    Q_OBJECT

public:
    EnvironmentAspect();

    // The environment including the user's modifications.
    Utils::Environment environment() const;

    int baseEnvironmentBase() const;
    void setBaseEnvironmentBase(int base);

    QList<Utils::EnvironmentItem> userEnvironmentChanges() const { return m_userChanges; }
    void setUserEnvironmentChanges(const QList<Utils::EnvironmentItem> &diff);

    void addSupportedBaseEnvironment(const QString &displayName,
                                     const std::function<Utils::Environment()> &getter);
    void addPreferredBaseEnvironment(const QString &displayName,
                                     const std::function<Utils::Environment()> &getter);

    // The environment the user chose as base for his modifications.
    Utils::Environment currentUnmodifiedBaseEnvironment() const;
    QString currentDisplayName() const;

    const QStringList displayNames() const;

    using EnvironmentModifier = std::function<void(Utils::Environment &)>;
    void addModifier(const EnvironmentModifier &);

    bool isLocal() const { return m_isLocal; }

signals:
    void baseEnvironmentChanged();
    void userEnvironmentChangesChanged(const QList<Utils::EnvironmentItem> &diff);
    void environmentChanged();

protected:
    void fromMap(const QVariantMap &map) override;
    void toMap(QVariantMap &map) const override;

    void setIsLocal(bool local) { m_isLocal = local; }

private:
    // One possible choice in the Environment aspect.
    struct BaseEnvironment {
        Utils::Environment unmodifiedBaseEnvironment() const;

        std::function<Utils::Environment()> getter;
        QString displayName;
    };

    QList<Utils::EnvironmentItem> m_userChanges;
    QList<EnvironmentModifier> m_modifiers;
    QList<BaseEnvironment> m_baseEnvironments;
    int m_base = -1;
    bool m_isLocal = false;
};

} // namespace ProjectExplorer
