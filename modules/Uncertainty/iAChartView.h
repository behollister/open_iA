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

#include "iAUncertaintyImages.h"  // for vtkImagePointer

#include <QWidget>

class QCustomPlot;
class QCPColorMap;
class QCPDataSelection;
class QCPCurve;

class iAChartView: public QWidget
{
	Q_OBJECT
public:
	iAChartView();
	void SetDatasets(QSharedPointer<iAUncertaintyImages> imgs);
	vtkImagePointer GetSelectionImage();
private slots:
	void selectionChanged(QCPDataSelection const &);
	void chartMousePress(QMouseEvent *);
	void xAxisChoice();
	void yAxisChoice();
	void colorThemeChanged(int index);
signals:
	void SelectionChanged();
private:
	QSharedPointer<iAUncertaintyImages> m_imgs;
	QCustomPlot* m_plot;
	vtkImagePointer m_selectionImg;
	size_t m_voxelCount;
	QWidget* m_xAxisChooser, * m_yAxisChooser;
	int m_xAxisChoice, m_yAxisChoice;
	QCPColorMap * colorMap;
	
	void AddPlot(vtkImagePointer imgX, vtkImagePointer imgY, QString const & captionX, QString const & captionY);
};
