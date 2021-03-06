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

#include "iAParameterGenerator.h"

class iARandomParameterGenerator: public iAParameterGenerator
{
public:
	virtual QString GetName() const;
	virtual ParameterSetsPointer GetParameterSets(QSharedPointer<iAAttributes> parameter, int sampleCount);
};

class iALatinHypercubeParameterGenerator: public iAParameterGenerator
{
public:
	virtual QString GetName() const;
	virtual ParameterSetsPointer GetParameterSets(QSharedPointer<iAAttributes> parameter, int sampleCount);
};

//! as all parameter values are supposed to be equally spaced,
//! and the number of values equally distributed among all parameters,
//! this algorithm will typically give less than the specified amount of samples
class iACartesianGridParameterGenerator : public iAParameterGenerator
{
public:
	virtual QString GetName() const;
	virtual ParameterSetsPointer GetParameterSets(QSharedPointer<iAAttributes> parameter, int sampleCount);
};

class iASelectionParameterGenerator : public iAParameterGenerator
{
public:
	iASelectionParameterGenerator(QString const & name, ParameterSetsPointer parameterSets);
	virtual QString GetName() const;
	virtual ParameterSetsPointer GetParameterSets(QSharedPointer<iAAttributes> parameter, int sampleCount);
private:
	QString m_name;
	ParameterSetsPointer m_parameterSets;
};

QVector<QSharedPointer<iAParameterGenerator> > & GetParameterGenerators();
