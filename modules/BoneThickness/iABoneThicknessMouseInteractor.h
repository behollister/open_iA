/*********************************  open_iA 2016 06  ******************************** *
* **********  A tool for scientific visualisation and 3D image processing  ********** *
* *********************************************************************************** *
* Copyright (C) 2016  C. Heinzl, M. Reiter, A. Reh, W. Li, M. Arikan, J. Weissenb�ck, *
*                     Artem & Alexander Amirkhanov, B. Fr�hler                        *
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
* Contact: FH O� Forschungs & Entwicklungs GmbH, Campus Wels, CT-Gruppe,              *
*          Stelzhamerstra�e 23, 4600 Wels / Austria, Email: c.heinzl@fh-wels.at       *
* ************************************************************************************/

#pragma once
// iA

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

#include "iABoneThickness.h"
#include "iABoneThicknessTable.h"

class iABoneThicknessMouseInteractor : public vtkInteractorStyleTrackballCamera
{
public:
	static iABoneThicknessMouseInteractor* New();
	vtkTypeMacro(iABoneThicknessMouseInteractor, vtkInteractorStyleTrackballCamera);

	void set(iABoneThickness* _pBoneThickness, iABoneThicknessTable* _pBoneThicknessTable, vtkActorCollection* _pSpheres)
	{
		m_pBoneThickness = _pBoneThickness;
		m_pBoneThicknessTable = _pBoneThicknessTable;

		m_pSpheres = _pSpheres;
	}

	virtual void OnLeftButtonDown() override
	{
		const int* pClickPos(GetInteractor()->GetEventPosition());

		vtkSmartPointer<vtkPropPicker>  pPicker(vtkSmartPointer<vtkPropPicker>::New());
		pPicker->Pick(pClickPos[0], pClickPos[1], 0, GetDefaultRenderer());

		vtkActor* pPickedActor(pPicker->GetActor());

		if (pPickedActor)
		{
			const vtkIdType idPickedActor(m_pSpheres->IsItemPresent(pPickedActor) - 1);

			if (idPickedActor > -1)
			{
				if (idPickedActor == m_pBoneThicknessTable->selectedRow())
				{
					m_pBoneThicknessTable->setSphereSelected();
				}
				else
				{
					m_pBoneThicknessTable->selectRow(idPickedActor);
				}
			}
			else
			{
				m_pBoneThickness->deSelect();
			}
		}
		else
		{
			m_pBoneThickness->deSelect();
		}

		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}

	private:
		iABoneThickness* m_pBoneThickness = nullptr;
		iABoneThicknessTable* m_pBoneThicknessTable = nullptr;
		vtkActorCollection* m_pSpheres = nullptr;
};
