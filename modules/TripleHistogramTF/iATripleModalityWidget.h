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

#include "BCoord.h"

#include "iASlicerMode.h"

#include <vtkSmartPointer.h>

#include <QWidget>

class iABarycentricTriangleWidget;
class iASimpleSlicerWidget;
class iATriangleRenderer;

class iADiagramFctWidget;
class iAModality;
class iATransferFunction;
class MdiChild;

class vtkColorTransferFunction;
class vtkPiecewiseFunction;

class QComboBox;
class QLabel;
class QSlider;

static const QString DEFAULT_MODALITY_LABELS[3] = { "A", "B", "C" };

class iATripleModalityWidget : public QWidget
{
	Q_OBJECT

public:
	iATripleModalityWidget(QWidget* parent, MdiChild *mdiChild, Qt::WindowFlags f = 0);
	~iATripleModalityWidget();

	bool setWeight(BCoord bCoord);
	bool setSlicerMode(iASlicerMode slicerMode);
	bool setSliceNumber(int sliceNumber);
	bool containsModality(QSharedPointer<iAModality> modality);
	int getModalitiesCount();

	QSharedPointer<iAModality> getModality(int index);
	double getWeight(int index);

	void updateModalities();

	bool isReady();

	// VIRTUAL METHODS
	virtual void initialize() = 0;
	virtual bool isSlicerInteractionEnabled() = 0;
	virtual void setModalityLabel(QString label, int index);

private slots:
	void updateTransferFunction1() { updateTransferFunction(0); }
	void updateTransferFunction2() { updateTransferFunction(1); }
	void updateTransferFunction3() { updateTransferFunction(2); }
	void originalHistogramChanged();

	void triangleWeightChanged(BCoord newWeight);
	void comboBoxIndexChanged(int newIndex);
	void sliderValueChanged(int newValue);

	void setSliceXYScrollBar();
	void setSliceXZScrollBar();
	void setSliceYZScrollBar();
	void setSliceXYScrollBar(int sliceNumberXY);
	void setSliceXZScrollBar(int sliceNumberXZ);
	void setSliceYZScrollBar(int sliceNumberYZ);

signals:
	void transferFunctionChanged();

	void weightChanged(BCoord bCoord);
	void slicerModeChanged(iASlicerMode slicerMode);
	void sliceNumberChanged(int sliceNumber);

protected:
	iADiagramFctWidget* m_histograms[3] = { nullptr, nullptr, nullptr };
	iASimpleSlicerWidget *m_slicerWidgets[3] = { nullptr, nullptr, nullptr };
	QComboBox *m_slicerModeComboBox;
	QSlider *m_sliceSlider;
	iABarycentricTriangleWidget *m_triangleWidget;

private:
	BCoord m_weightCur;
	void updateScrollBars(int newValue);
	void updateTransferFunction(int index);
	void updateCopyTransferFunction(int index);
	void updateOriginalTransferFunction(int index);
	void applyWeights();

	void setWeightPrivate(BCoord weights);
	void setSlicerModePrivate(iASlicerMode slicerMode);
	void setSliceNumberPrivate(int sliceNumber);

	QSharedPointer<iAModality> m_modalitiesActive[3];

	iATriangleRenderer *m_triangleRenderer;

	BCoord getWeight();
	iASlicerMode getSlicerMode();
	iASlicerMode getSlicerModeAt(int comboBoxIndex);
	int getSliceNumber();

	// Background stuff
	iATransferFunction *m_copyTFs[3] = { nullptr, nullptr, nullptr };
	iATransferFunction* createCopyTf(int index, vtkSmartPointer<vtkColorTransferFunction> colorTf, vtkSmartPointer<vtkPiecewiseFunction> opacity);

	// Widgets and stuff
	QLabel *m_disabledLabel;

	// TODO: another pointer to MdiChild... is this really optimal?
	MdiChild *m_mdiChild;
	
};