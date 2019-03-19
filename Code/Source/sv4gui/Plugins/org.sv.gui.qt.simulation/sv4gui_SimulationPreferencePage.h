/* Copyright (c) Stanford University, The Regents of the University of
 *               California, and others.
 *
 * All Rights Reserved.
 *
 * See Copyright-SimVascular.txt for additional details.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SV4GUI_SIMULATIONPREFERENCEPAGE_H
#define SV4GUI_SIMULATIONPREFERENCEPAGE_H

#include <sv4gui_SimulationPreferences.h>

#include <berryIPreferences.h>
#include <berryIQtPreferencePage.h>

namespace Ui {
class sv4guiSimulationPreferencePage;
}

class sv4guiSimulationPreferencePage : public QObject, public berry::IQtPreferencePage
{
    Q_OBJECT
    Q_INTERFACES(berry::IPreferencePage)

public:
    sv4guiSimulationPreferencePage();
    ~sv4guiSimulationPreferencePage();

    void CreateQtControl(QWidget* parent) override;
    QWidget* GetQtControl() const override;
    void Init(berry::IWorkbench::Pointer) override;
    void PerformCancel() override;
    bool PerformOk() override;
    void Update() override;
    void InitializeSolverLocations();

private slots:
  void SetPresolverPath();
  void SetFlowsolverPath();
  void SetMPIExecPath();
  void SetCustomTemplatePath();
  void SetPostsolverPath();

private:
  void SetMpiExec(const QString& solverPathBin, const QString& applicationPath);
  void SetPreSolver(const QString& solverPathBin, const QString& applicationPath);
  void SetSolver(const QString& solverPathBin, const QString& applicationPath);
  void SetPostSolver(const QString& solverPathBin, const QString& applicationPath);
  void SetMpiImplementation();

  berry::IPreferences::Pointer m_Preferences;
  QScopedPointer<Ui::sv4guiSimulationPreferencePage> m_Ui;
  QWidget* m_Control;
  sv4guiSimulationPreferences m_DefaultPrefs;

};

#endif // SV4GUI_SIMULATIONPREFERENCEPAGE_H
