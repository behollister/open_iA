/*************************************  open_iA  ************************************ *
* **********   A tool for visual analysis and processing of 3D CT images   ********** *
* *********************************************************************************** *
* Copyright (C) 2016-2018  C. Heinzl, M. Reiter, A. Reh, W. Li, M. Arikan,            *
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

#include <vtkSmartPointer.h>

#include <QDockWidget>

class QStackedLayout;
class QLabel;

class MdiChild;
class iABarycentricTriangleWidget;
class iATripleModalityWidget;
class BCoord;

class vtkSmartVolumeMapper;
class vtkVolume;
class vtkRenderer;

//typedef iAQTtoUIConnector<QDockWidget, Ui_dlg_TripleHistogramTF> TripleHistogramTFConnector;

class dlg_TripleHistogramTF : public QDockWidget//public TripleHistogramTFConnector
{
	Q_OBJECT

public:
	dlg_TripleHistogramTF(MdiChild* parent, Qt::WindowFlags f = 0);
	~dlg_TripleHistogramTF();

public slots:
	void updateTransferFunction();
	void updateModalities();

private slots:

private:
	void updateDisabledLabel();

	// TODO: is it really good to keep the mdiChild as a member variable?
	MdiChild *m_mdiChild;

	// Widgets and stuff
	QStackedLayout *m_stackedLayout;
	QLabel *m_disabledLabel;
	iATripleModalityWidget *m_histogramStack;
	
	vtkSmartPointer<vtkSmartVolumeMapper> combinedVolMapper;
	vtkSmartPointer<vtkRenderer> combinedVolRenderer;
	vtkSmartPointer<vtkVolume> combinedVol;
};
