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
#include "iAFileUtils.h"

#include "iAConsole.h"

#include <QCollator>
#include <QDir>
#include <QString>

QString MakeAbsolute(QString const & baseDir, QString const & fileName)
{
#if _WIN32
	if (fileName.contains(":"))
#else
	if (fileName.startsWith("/"))
#endif
	{
		return fileName;
	}
	QDir dir(baseDir);
	// TODO : use canonicalFilePath here? drawback: empty if file doesn't exist!
	return dir.absoluteFilePath(fileName);
}

QString MakeRelative(QString const & baseDir,  QString const & fileName)
{
	QDir dir(baseDir);
	return  dir.relativeFilePath(fileName);
	if (fileName.startsWith(baseDir))
	{                                                               // for '/'
		return fileName.right(fileName.length() - baseDir.length() - 1);
	}
	return fileName;
}

void FindFiles(QString const & directory, QStringList const & filters, bool recurse,
	QStringList & filesOut, QFlags<FilesFolders> filesFolders)
{
	QDir dir(directory);
	dir.setSorting(QDir::NoSort);
	QDir::Filters flags = QDir::Files;
	if (recurse || filesFolders.testFlag(Folders))
		flags = QDir::Files | QDir::AllDirs;
	QStringList entryList = dir.entryList(filters, flags);
	QCollator collator;
	collator.setNumericMode(true);	// natural sorting
	std::sort(entryList.begin(), entryList.end(), collator);
	for (QString fileName : entryList)
	{
		if (fileName == "." || fileName == "..")
			continue;
		QFileInfo fi(directory + "/" + fileName);
		if (fi.isDir())
		{
			if (filesFolders.testFlag(Folders))
				filesOut.append(fi.absoluteFilePath());
			if (recurse)
				FindFiles(fi.absoluteFilePath(), filters, recurse, filesOut, filesFolders);
		}
		else
		{
			if (filesFolders.testFlag(Files))
				filesOut.append(fi.absoluteFilePath());
		}
	}
}

std::string getLocalEncodingFileName(QString const & fileName)
{
	QByteArray fileNameEncoded = fileName.toLocal8Bit();
	if (fileNameEncoded.contains('?'))
	{
		DEBUG_LOG(QString("File name '%1' not convertible to a system encoding string. "
			"Please specify a filename without special characters!").arg(fileName));
		return std::string();
	}
	return std::string(fileNameEncoded.constData());
}