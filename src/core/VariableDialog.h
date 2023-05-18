#pragma once

#include <QDialog>

class VariableModel;
class QComboBox;
class QPushButton;
class RealGenerator;
class Engine;

class VariableDialog : public QDialog {
    public:
        VariableDialog(QWidget* parent = nullptr);

        RealGenerator* getCurrentVariable() const;

        void setModel(VariableModel* model);

        static RealGenerator* getVariable(Engine* engine, QWidget* parent = nullptr);

    private:
        void createUi();

        QComboBox* varCombo;
        QPushButton* okButton;
        QPushButton* cancelButton;

        VariableModel* varModel = nullptr;
};
