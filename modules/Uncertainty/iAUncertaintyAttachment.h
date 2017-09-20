/*************************************  open_iA  ************************************ *
* **********  A tool for scientific visualisation and 3D image processing  ********** *
* *********************************************************************************** *
* Copyright (C) 2016-2017  C. Heinzl, M. Reiter, A. Reh, W. Li, M. Arikan,            *
*                          J. Weissenböck, Artem & Alexander Amirkhanov, B. Fröhler   *
* *********************************************************************************** *
* This program is free software: you can redistribute it and/or modify it under the   *
* terms of the GNU General Public License as published by the Free Software           *
* Foundation, either version 3 of the License, or (at your option) any later version. *
*                                                                                     *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY     *
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A     *
* PARTICULAR PURPOSE.  See the GNU General Public License for more details.           *
*                                                                                     *
* You should have received a copy of the GNU General Public License along with this   *
* program.  If not, see http://www.gnu.org/licenses/                                  *
* *********************************************************************************** *
* Contact: FH OÖ Forschungs & Entwicklungs GmbH, Campus Wels, CT-Gruppe,              *
*          Stelzhamerstraße 23, 4600 Wels / Austria, Email: c.heinzl@fh-wels.at       *
* ************************************************************************************/
#pragma once

#include "iAModuleAttachmentToChild.h"

#include "iAITKIO.h"

#include <QSharedPointer>
#include <QVector>

class iADockWidgetWrapper;
class iAEnsemble;
class iAEnsembleView;
class iAHistogramView;
class iAMemberView;
class iAScatterPlotView;
class iASpatialView;

class iAUncertaintyAttachment : public iAModuleAttachmentToChild
{
	Q_OBJECT
public:
	static iAUncertaintyAttachment* Create(MainWindow * mainWnd, iAChildData childData);
	void ToggleDockWidgetTitleBars();
	void ToggleSettings();
	void CalculateNewSubEnsemble();
	bool LoadEnsemble(QString const & fileName);
private slots:
	void ChartSelectionChanged();
	void MemberSelected(int memberIdx);
	void EnsembleSelected(QSharedPointer<iAEnsemble> ensemble);
private:
	iAUncertaintyAttachment(MainWindow * mainWnd, iAChildData childData);
	iAHistogramView* m_histogramView;
	iAMemberView* m_memberView;
	iAScatterPlotView* m_scatterplotView;
	iASpatialView* m_spatialView;
	iAEnsembleView* m_ensembleView;
	QVector<iADockWidgetWrapper*> m_dockWidgets;
	QVector<iAITKIO::ImagePointer> m_shownMembers;
	QSharedPointer<iAEnsemble> m_currentEnsemble;
	int m_newSubEnsembleID;
};
