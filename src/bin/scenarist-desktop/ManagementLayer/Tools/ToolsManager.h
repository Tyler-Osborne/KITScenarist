#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <QObject>

namespace BusinessLogic {
    struct BackupInfo;
    class RestoreFromBackupTool;
    class ScenarioDocument;
}

namespace UserInterface {
    class ToolsView;
}


namespace ManagementLayer
{
    /**
     * @brief Управляющий инструментами
     */
    class ToolsManager : public QObject
    {
        Q_OBJECT

    public:
        explicit ToolsManager(QObject* _parent, QWidget* _parentWidget);

        QWidget* view() const;

    private:
        /**
         * @brief Загрузить список доступных бэкапов из файла
         */
        void loadBackupsList();

        /**
         * @brief Отобразить информацию о списке бэкапов
         */
        void showBackupInfo(const BusinessLogic::BackupInfo& _backupInfo);

        /**
         * @brief Загрузить выбранный пользователем бэкап
         */
        void loadBackup(const QModelIndex& _backupItemIndex);

        /**
         * @brief Отобразить содержимое бэкапа
         */
        void showBackup(const QString& _backup);

    private:
        /**
         * @brief Настроить представление
         */
        void initView();

        /**
         * @brief Настроить соединения
         */
        void initConnections();

    private:
        /**
         * @brief Представление страницы инструментов
         */
        UserInterface::ToolsView* m_view = nullptr;

        /**
         * @brief Документ сценария
         */
        BusinessLogic::ScenarioDocument* m_script = nullptr;

        /**
         * @brief Инструмент восстановления из бэкапа
         */
        BusinessLogic::RestoreFromBackupTool* m_restoreFromBackupTool = nullptr;
    };
}

#endif // TOOLSMANAGER_H
