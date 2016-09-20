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
*          Stelzhamerstra�e 23, 4600 Wels / Austria, Email:                           *
* ************************************************************************************/
 
#include "pch.h"
#include "iARangeSliderDiagramData.h"

iARangeSliderDiagramData::iARangeSliderDiagramData( QList<double> m_rangeSliderData, double min, double max ) :
	m_rangeSliderFunction( NULL ),
	m_rangeSliderData( m_rangeSliderData )
{
	m_range[0] = min;
	m_range[1] = max;
}

iARangeSliderDiagramData::~iARangeSliderDiagramData()
{
	delete[] m_rangeSliderFunction;
}

void iARangeSliderDiagramData::updateRangeSliderFunction()
{
	if ( !m_rangeSliderFunction )
	{
		m_rangeSliderFunction = new DataType[m_rangeSliderData.size()];
	}

	for ( int i = 0; i < m_rangeSliderData.size(); ++i )
	{
		m_rangeSliderFunction[i] = m_rangeSliderData.at( i );
	}
}

iAAbstractDiagramData::DataType const * iARangeSliderDiagramData::GetData() const
{
	return m_rangeSliderFunction;
}

size_t iARangeSliderDiagramData::GetNumBin() const
{
	return m_rangeSliderData.size();
}
