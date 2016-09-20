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
 
#ifndef iADatasetInfo_H__
#define iADatasetInfo_H__

#include "iAPorosityAnalyserModuleInterface.h"

#include <QThread>

class iADatasetInfo : public QThread
{
	Q_OBJECT

public:
	iADatasetInfo( iAPorosityAnalyserModuleInterface * pmi, QObject * parent = 0 ) : m_pmi( pmi ), QThread( parent ) {};
	QStringList getNewGeneratedInfoFiles();

protected:
	virtual void run();
	void calculateInfo();

	iAPorosityAnalyserModuleInterface * m_pmi;

signals:
	void progress( int );

private:
	template<class T> int generateInfo( QString datasetPath, QString datasetName, 
										ImagePointer & image, iAPorosityAnalyserModuleInterface * pmi,
										int filesInfoNbToCreate, int currentFInfoNb );
	
	QStringList m_newGeneratedInfoFilesList;
};
#endif // iADatasetInfo_H__