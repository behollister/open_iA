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
#include "iAThresholding.h"

#include "defines.h"    // for DIM
#include "iAAttributeDescriptor.h"
#include "iAConnector.h"
#include "iAProgress.h"
#include "iATypedCallHelper.h"

#include "itkMaximumDistance.h"

#include <itkAdaptiveOtsuThresholdImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkGradientMagnitudeImageFilter.h>
#include <itkHuangThresholdImageFilter.h>
#include <itkIntermodesThresholdImageFilter.h>
#include <itkIsoDataThresholdImageFilter.h>
#include <itkKittlerIllingworthThresholdImageFilter.h>
#include <itkLiThresholdImageFilter.h>
#include <itkMaximumEntropyThresholdImageFilter.h>
#include <itkMomentsThresholdImageFilter.h>
#include <itkOtsuThresholdImageFilter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <itkRobustAutomaticThresholdImageFilter.h>
#include <itkRemovePeaksOtsuThresholdImageFilter.h>
#include <itkRenyiEntropyThresholdImageFilter.h>
#include <itkShanbhagThresholdImageFilter.h>
#include <itkTriangleThresholdImageFilter.h>
#include <itkYenThresholdImageFilter.h>

#include <QLocale>

// Binary Threshold

template<class T>
void binary_threshold(iAFilter* filter, QMap<QString, QVariant> const & parameters)
{
	typedef itk::Image< T, 3 >   InputImageType;
	typedef itk::Image< T, 3 >   OutputImageType;
	typedef itk::BinaryThresholdImageFilter<InputImageType, OutputImageType> BTIFType;
	auto binThreshFilter = BTIFType::New();
	binThreshFilter->SetLowerThreshold(T(parameters["Lower threshold"].toDouble()));
	binThreshFilter->SetUpperThreshold(T(parameters["Upper threshold"].toDouble()));
	binThreshFilter->SetOutsideValue(T(parameters["Outside value"].toDouble()));
	binThreshFilter->SetInsideValue(T(parameters["Inside value"].toDouble()));
	binThreshFilter->SetInput(dynamic_cast<InputImageType *>(filter->Input()[0]->GetITKImage()));
	filter->Progress()->Observe(binThreshFilter);
	binThreshFilter->Update();
	filter->AddOutput(binThreshFilter->GetOutput());
}

IAFILTER_CREATE(iABinaryThreshold)

void iABinaryThreshold::PerformWork(QMap<QString, QVariant> const & parameters)
{
	ITK_TYPED_CALL(binary_threshold, InputPixelType(), this, parameters);
}

iABinaryThreshold::iABinaryThreshold() :
	iAFilter("Binary Thresholding", "Segmentation/Global Thresholding",
		"Computes a segmented image based on whether the intensity is inside a given interval.<br/>"
		"Two thresholds (lower, upper) can be specified; "
		"if a voxel value is between these two (including the threshold values themselves), "
		"then the output is set to the <em>inside</em> value at this voxel, "
		"otherwise the <em>outside</em> value.<br/>"
		"For more information, see the "
		"<a href=\"https://itk.org/Doxygen/html/classitk_1_1BinaryThresholdImageFilter.html\">"
		"Binary Threshold Filter</a> in the ITK documentation.")
{
	AddParameter("Lower threshold", Continuous, 0);
	AddParameter("Upper threshold", Continuous, 32768);
	AddParameter("Outside value", Continuous, 0);
	AddParameter("Inside value", Continuous, 1);
}


// Robust Automatic Threshold (RAT)

template<class T> 
void rats_threshold(iAFilter* filter, QMap<QString, QVariant> const & parameters)
{
	typedef typename itk::Image< T, 3 >   InputImageType;
	typedef typename itk::Image< T, 3 >   OutputImageType;
	typedef typename itk::Image< float, 3 >   GradientImageType;
	typedef itk::GradientMagnitudeImageFilter< InputImageType, GradientImageType > GMFType;
	typename GMFType::Pointer gmfilter = GMFType::New();
	gmfilter->SetInput( dynamic_cast< InputImageType * >( filter->Input()[0]->GetITKImage() ) );
	filter->Progress()->Observe( gmfilter );
	gmfilter->Update(); 
	typedef typename itk::RobustAutomaticThresholdImageFilter < InputImageType, GradientImageType, OutputImageType > RATType;
	auto ratsFilter = RATType::New();
	ratsFilter->SetInput( dynamic_cast< InputImageType * >( filter->Input()[0]->GetITKImage() ) );
	ratsFilter->SetGradientImage(gmfilter->GetOutput());
	ratsFilter->SetOutsideValue( T(parameters["Outside value"].toDouble()) );
	ratsFilter->SetInsideValue( T(parameters["Inside value"].toDouble()) );
	ratsFilter->SetPow( parameters["Power"].toDouble() );
	filter->Progress()->Observe( ratsFilter );
	ratsFilter->Update();
	filter->AddOutputValue("Rats threshold", (double)ratsFilter->GetThreshold());
	filter->AddOutput(ratsFilter->GetOutput());
}

IAFILTER_CREATE(iARatsThreshold)

void iARatsThreshold::PerformWork(QMap<QString, QVariant> const & parameters)
{
	ITK_TYPED_CALL(rats_threshold, InputPixelType(), this, parameters);
}

iARatsThreshold::iARatsThreshold() :
	iAFilter("RATS", "Segmentation/Global Thresholding",
		"Robust automatic threshold selection. <br/>"
		"This filter determines a global threshold by weighting the pixel "
		"values by their gradient (thus setting the threshold to the value "
		"where the intensities change most).<br/>"
		"For more information, see the "
		"<a href=\"https://itk.org/Doxygen/html/classitk_1_1RobustAutomaticThresholdImageFilter.html\">"
		"RATS Filter</a> in the ITK documentation.")
{
	AddParameter("Power", Continuous, 1);
	AddParameter("Outside value", Continuous, 0);
	AddParameter("Inside value", Continuous, 1);
}


// Otsu's Threshold

template<class T> 
void otsu_threshold(iAFilter* filter, QMap<QString, QVariant> const & parameters)
{
	typedef typename itk::Image< T, 3 >   InputImageType;
	typedef typename itk::Image< T, 3 >   OutputImageType;
	typedef typename itk::OtsuThresholdImageFilter < InputImageType, OutputImageType > OTIFType;
	typedef typename itk::RemovePeaksOtsuThresholdImageFilter < InputImageType, OutputImageType > RPOTIFType;

	if (parameters["Remove peaks"].toBool())
	{
		auto otsuFilter = RPOTIFType::New();
		otsuFilter->SetNumberOfHistogramBins( T (parameters["Number of histogram bins"].toDouble()) );
		otsuFilter->SetOutsideValue( T(parameters["Outside value"].toDouble()) );
		otsuFilter->SetInsideValue( T(parameters["Inside value"].toDouble()) );
		otsuFilter->SetInput( dynamic_cast< InputImageType * >( filter->Input()[0]->GetITKImage() ) );
		filter->Progress()->Observe( otsuFilter );
		otsuFilter->Update();
		filter->AddOutputValue("Otsu threshold", (double)otsuFilter->GetThreshold());
		filter->AddOutput(otsuFilter->GetOutput());
	}
	else
	{
		auto otsuFilter = OTIFType::New();
		otsuFilter->SetNumberOfHistogramBins( T (parameters["Number of histogram bins"].toDouble()) );
		otsuFilter->SetOutsideValue( T(parameters["Outside value"].toDouble()) );
		otsuFilter->SetInsideValue( T(parameters["Inside value"].toDouble()) );
		otsuFilter->SetInput( dynamic_cast< InputImageType * >( filter->Input()[0]->GetITKImage() ) );
		filter->Progress()->Observe( otsuFilter );
		otsuFilter->Update();
		filter->AddOutputValue("Otsu threshold", (double)otsuFilter->GetThreshold());
		filter->AddOutput(otsuFilter->GetOutput());
	}
}

IAFILTER_CREATE(iAOtsuThreshold)

void iAOtsuThreshold::PerformWork(QMap<QString, QVariant> const & parameters)
{
	ITK_TYPED_CALL(otsu_threshold, InputPixelType(), this, parameters);
}

iAOtsuThreshold::iAOtsuThreshold() :
	iAFilter("Otsu Threshold", "Segmentation/Global Thresholding",
		"Creates an segmented image using Otsu's method.<br/>"
		"The result is segmented into foreground (inside value) and background (outside value), "
		"the method tries to maximize the between-class variance using a histogram.<br/>"
		"For more information, see the "
		"<a href=\"https://itk.org/Doxygen/html/classitk_1_1OtsuThresholdImageFilter.html\">"
		"Otsu Threshold Filter</a> in the ITK documentation.")
{
	AddParameter("Number of histogram bins", Discrete, 128, 2);
	AddParameter("Outside value", Continuous, 0);
	AddParameter("Inside value", Continuous, 1);
	AddParameter("Remove peaks", Boolean, false);
}


// Adaptive Otsu

template<class T>
void adaptive_otsu_threshold(iAFilter* filter, QMap<QString, QVariant> const & parameters)
{
	typedef itk::Image< T, 3 >   InputImageType;
	typedef itk::Image< T, 3 >   OutputImageType;
	typename InputImageType::SizeType radius;
	radius.Fill(T(parameters["Radius"].toDouble()));
	typedef itk::AdaptiveOtsuThresholdImageFilter< InputImageType, OutputImageType > AOTIFType;
	typename AOTIFType::Pointer adotFilter = AOTIFType::New();
	adotFilter->SetInput(dynamic_cast< InputImageType * >(filter->Input()[0]->GetITKImage()));
	adotFilter->SetOutsideValue(T(parameters["Outside value"].toDouble()));
	adotFilter->SetInsideValue(T(parameters["Inside value"].toDouble()));
	adotFilter->SetNumberOfHistogramBins(parameters["Number of histogram bins"].toUInt());
	adotFilter->SetNumberOfControlPoints(parameters["Control points"].toUInt());
	adotFilter->SetNumberOfLevels(parameters["Levels"].toUInt());
	adotFilter->SetNumberOfSamples(parameters["Samples"].toUInt());
	adotFilter->SetRadius(radius);
	adotFilter->SetSplineOrder(parameters["Spline order"].toUInt());
	adotFilter->Update();
	filter->Progress()->Observe(adotFilter);
	adotFilter->Update();
	filter->AddOutput(adotFilter->GetOutput());
}

IAFILTER_CREATE(iAAdaptiveOtsuThreshold)

void iAAdaptiveOtsuThreshold::PerformWork(QMap<QString, QVariant> const & parameters)
{
	ITK_TYPED_CALL(adaptive_otsu_threshold, InputPixelType(), this, parameters);
}

iAAdaptiveOtsuThreshold::iAAdaptiveOtsuThreshold() :
	iAFilter("Adaptive Otsu", "Segmentation/Local Thresholding",
		"A local thresholding filter based on Otsu's method.<br/>"
		"For more information, see the "
		"<a href=\"https://github.com/ITKTools/ITKTools/blob/master/src/thresholdimage/itkAdaptiveOtsuThresholdImageFilter.h\">"
		"Adaptive Otsu Threshold source code</a> in the ITKTools.")
{
	AddParameter("Number of histogram bins", Discrete, 256, 2);
	AddParameter("Outside value", Continuous, 0);
	AddParameter("Inside value", Continuous, 1);
	AddParameter("Radius", Continuous, 8);
	AddParameter("Samples", Discrete, 5000);
	AddParameter("Levels", Discrete, 3);
	AddParameter("Control points", Discrete, 50, 1);
	AddParameter("Spline order", Discrete, 3, 2);
}


// Otsu Multiple Threshold

template<class T>
void otsu_multiple_threshold(iAFilter* filter, QMap<QString, QVariant> const & parameters)
{
	typedef typename itk::Image< T, 3 >   InputImageType;
	typedef typename itk::Image< T, 3 >   OutputImageType;
	typedef typename itk::OtsuMultipleThresholdsImageFilter < InputImageType, OutputImageType > OMTIFType;
	auto otsumultiFilter = OMTIFType::New();
	otsumultiFilter->SetNumberOfHistogramBins( T (parameters["Number of histogram bins"].toDouble()) );
	otsumultiFilter->SetNumberOfThresholds( T (parameters["Number of thresholds"].toDouble()) );
	otsumultiFilter->SetInput( dynamic_cast< InputImageType * >( filter->Input()[0]->GetITKImage() ) );
	otsumultiFilter->SetValleyEmphasis( parameters["Valley emphasis"].toBool() );
	filter->Progress()->Observe( otsumultiFilter );
	otsumultiFilter->Update();
	for (int i = 0; i< otsumultiFilter->GetThresholds().size(); i++)
		filter->AddOutputValue(QString("Otsu multiple threshold %1").arg(i), otsumultiFilter->GetThresholds()[i]);
	filter->AddOutput(otsumultiFilter->GetOutput());
}

IAFILTER_CREATE(iAOtsuMultipleThreshold)

void iAOtsuMultipleThreshold::PerformWork(QMap<QString, QVariant> const & parameters)
{
	ITK_TYPED_CALL(otsu_multiple_threshold, InputPixelType(), this, parameters);
}

iAOtsuMultipleThreshold::iAOtsuMultipleThreshold() :
	iAFilter("Otsu Multiple Thresholds", "Segmentation/Global Thresholding",
		"Performs Otsu's method with multiple thresholds.<br/>"
		"For more information, see the "
		"<a href=\"https://itk.org/Doxygen/html/classitk_1_1OtsuMultipleThresholdsImageFilter.html\">"
		"Otsu Multiple Threshold Filter</a> in the ITK documentation.")
{
	AddParameter("Number of histogram bins", Discrete, 256, 2);
	AddParameter("Number of thresholds", Discrete, 2, 1);
	AddParameter("Valley emphasis", Boolean, 1);
}


// Maximum Distance Threshold

template<class T>
void maximum_distance(iAFilter* filter, QMap<QString, QVariant> const & parameters)
{
	typedef itk::Image< T, 3 >   InputImageType;
	typedef itk::Image< T, 3 >   OutputImageType;
	typedef itk::MaximumDistance< InputImageType > MaximumDistanceType;
	auto maxFilter = MaximumDistanceType::New();
	maxFilter->SetInput(dynamic_cast< InputImageType * >(filter->Input()[0]->GetITKImage()));
	maxFilter->SetBins(parameters["Number of histogram bins"].toDouble());
	if (parameters["Use low intensity"].toBool())
		maxFilter->SetCentre(parameters["Low intensity"].toDouble());
	else
		maxFilter->SetCentre(32767);
	filter->Progress()->Observe(maxFilter);
	maxFilter->Update();
	filter->AddOutput(maxFilter->GetOutput());
	filter->AddOutputValue("Maximum distance threshold", static_cast<double>(maxFilter->GetThreshold()));
	filter->AddOutputValue("Maximum distance low peak",  maxFilter->GetLowIntensity());
	filter->AddOutputValue("Maximum distance high peak", maxFilter->GetHighIntensity());
}

void iAMaximumDistance::PerformWork(QMap<QString, QVariant> const & parameters)
{
	ITK_TYPED_CALL(maximum_distance, InputPixelType(), this, parameters);
}

IAFILTER_CREATE(iAMaximumDistance)

iAMaximumDistance::iAMaximumDistance() :
	iAFilter("Maximum Distance", "Segmentation/Global Thresholding",
		"A global threshold based on the maximum distance of peaks in the histogram, for voids segmentation.")
{
	AddParameter("Number of histogram bins", Discrete, 256, 2);
	AddParameter("Low intensity", Continuous, 0);
	AddParameter("Use low intensity", Boolean, false);
}


// Parameterless Thresholding Methods

namespace
{
	enum ParameterlessThresholdingIDs
	{
		P_OTSU_THRESHOLD,
		P_ISODATA_THRESHOLD,
		P_MAXENTROPY_THRESHOLD,
		P_MOMENTS_THRESHOLD,
		P_YEN_THRESHOLD,
		P_RENYI_THRESHOLD,
		P_SHANBHAG_THRESHOLD,
		P_INTERMODES_THRESHOLD,
		P_HUANG_THRESHOLD,
		P_LI_THRESHOLD,
		P_KITTLERILLINGWORTH_THRESHOLD,
		P_TRIANGLE_THRESHOLD,
		P_MINIMUM_THRESHOLD
	};

	QStringList const & GetParameterlessThresholdingNames()
	{
		static QStringList ParameterlessThresholdingNames;
		if (ParameterlessThresholdingNames.empty())
		{
			ParameterlessThresholdingNames.push_back("Otsu Threshold");
			ParameterlessThresholdingNames.push_back("Isodata Thresold");
			ParameterlessThresholdingNames.push_back("Maximum Entropy  Thresold");
			ParameterlessThresholdingNames.push_back("Yen Threshold");
			ParameterlessThresholdingNames.push_back("Renyi Entropy Thresold");
			ParameterlessThresholdingNames.push_back("Shanbhag Threshold");
			ParameterlessThresholdingNames.push_back("Intermodes Threshold");
			ParameterlessThresholdingNames.push_back("Huang Threshold");
			ParameterlessThresholdingNames.push_back("Li Threshold");
			ParameterlessThresholdingNames.push_back("Kittler Illingworth Threshold");
			ParameterlessThresholdingNames.push_back("Triangle Threshold");
		}
		return ParameterlessThresholdingNames;
	}

	int MapMethodNameToID(QString const & name)
	{
		return GetParameterlessThresholdingNames().indexOf(name);
	}
}

IAFILTER_CREATE(iAParameterlessThresholding)

iAParameterlessThresholding::iAParameterlessThresholding() :
	iAFilter("Parameterless Thresholding", "Segmentation/Global Thresholding",
		"Performs a \"parameterless\" global thresholding, that is, a thresholding"
		"where the threshold is determined automatically based on the histogram."
		"Several different <em>Method</em>s for determining the threshold are available, "
		"you can also set the <em>Number of histogram")
{
	AddParameter("Method", Categorical, GetParameterlessThresholdingNames());
	AddParameter("Number of histogram bins", Discrete, 128, 2);
	AddParameter("Outside value", Continuous, 0);
	AddParameter("Inside value", Continuous, 1);
}

template <typename T>
void parameterless(iAFilter* filter, QMap<QString, QVariant> const & params)
{
	typedef itk::Image<T, DIM > InputImageType;
	typedef itk::Image<unsigned char, DIM> MaskImageType;
	typedef itk::HistogramThresholdImageFilter<InputImageType, MaskImageType> parameterFreeThrFilterType;
	typename parameterFreeThrFilterType::Pointer plFilter;
	switch (MapMethodNameToID(params["Method"].toString()))
	{
	case P_OTSU_THRESHOLD:
		plFilter = itk::OtsuThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_ISODATA_THRESHOLD:
		plFilter = itk::IsoDataThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_MAXENTROPY_THRESHOLD:
		plFilter = itk::MaximumEntropyThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_MOMENTS_THRESHOLD:
		plFilter = itk::MomentsThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_YEN_THRESHOLD:
		plFilter = itk::YenThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_RENYI_THRESHOLD:
		plFilter = itk::RenyiEntropyThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_SHANBHAG_THRESHOLD:
		plFilter = itk::ShanbhagThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_INTERMODES_THRESHOLD:
		plFilter = itk::IntermodesThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_HUANG_THRESHOLD:
		plFilter = itk::HuangThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_LI_THRESHOLD:
		plFilter = itk::LiThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_KITTLERILLINGWORTH_THRESHOLD:
		plFilter = itk::KittlerIllingworthThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_TRIANGLE_THRESHOLD:
		plFilter = itk::TriangleThresholdImageFilter <InputImageType, MaskImageType>::New();
		break;
	case P_MINIMUM_THRESHOLD:
	{
		typedef itk::IntermodesThresholdImageFilter <InputImageType, MaskImageType> MinimumFilterType;
		typename MinimumFilterType::Pointer minimumFilter = MinimumFilterType::New();
		minimumFilter->SetUseInterMode(false);
		plFilter = minimumFilter;
		break;
	}
	}
	plFilter->SetInput(dynamic_cast<InputImageType*>(filter->Input()[0]->GetITKImage()));
	plFilter->SetNumberOfHistogramBins(params["Number of histogram bins"].toUInt());
	plFilter->SetOutsideValue(params["Outside value"].toDouble());
	plFilter->SetInsideValue(params["Inside value"].toDouble());
	filter->Progress()->Observe(plFilter);
	plFilter->Update();
	filter->AddOutput(plFilter->GetOutput());
	filter->AddOutputValue("Threshold", static_cast<double>(plFilter->GetThreshold()));
}

void iAParameterlessThresholding::PerformWork(QMap<QString, QVariant> const & params)
{
	ITK_TYPED_CALL(parameterless, InputPixelType(), this, params);
}
