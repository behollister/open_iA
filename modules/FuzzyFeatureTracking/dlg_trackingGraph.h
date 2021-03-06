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

// Qt
#include <QDockWidget>
#include <QWidget>
// iA
#include "ui_TrackingGraph.h"
#include "iATrackingGraphItem.h"
// VTK
#include <QtGlobal>
#include <vtkVersion.h>
#if (VTK_MAJOR_VERSION >= 8 && defined(VTK_OPENGL2_BACKEND) && QT_VERSION >= 0x050400 )
#include <QVTKOpenGLWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#else
#include <QVTKWidget.h>
#include <vtkRenderWindow.h>
#endif
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkContextInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkContextTransform.h>
#include <vtkContextActor.h>
#include <vtkMutableDirectedGraph.h>
#include <vtkContextScene.h>
// std
#include <map>

class dlg_trackingGraph : public QDockWidget, private Ui_TrackingGraph
{
	Q_OBJECT

public:
	dlg_trackingGraph(QWidget* parent);

	void updateGraph(vtkMutableDirectedGraph* g, int nunRanks, std::map<vtkIdType, int> nodesToLayers, std::map<int, std::map<vtkIdType, int>> graphToTableId);

private:
#if (VTK_MAJOR_VERSION >= 8 && defined(VTK_OPENGL2_BACKEND))
	QVTKOpenGLWidget* graphWidget;
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
#else
	QVTKWidget*		graphWidget;
	vtkSmartPointer<vtkRenderWindow> m_renderWindow;
#endif

	vtkSmartPointer<vtkMutableDirectedGraph>	m_graph;
	vtkSmartPointer<iATrackingGraphItem>		m_graphItem;
	vtkSmartPointer<vtkContextActor>			m_actor;
	vtkSmartPointer<vtkContextTransform>		m_trans;
	vtkSmartPointer<vtkRenderer>				m_renderer;
	vtkSmartPointer<vtkContextScene>			m_contextScene;
	vtkSmartPointer<vtkContextInteractorStyle>	m_interactorStyle;
	vtkSmartPointer<vtkRenderWindowInteractor>	m_interactor;

	std::map<vtkIdType, int>					m_nodesToLayers;
	std::map<int, std::map<vtkIdType, int>>		m_graphToTableId;
};
