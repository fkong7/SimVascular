/* Copyright (c) Stanford University, The Regents of the University of
 *               California, and others.
 *
 * All Rights Reserved.
 *
 * See Copyright-SimVascular.txt for additional details.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SV4GUI_CONTOURGROUP_H
#define SV4GUI_CONTOURGROUP_H

#include "SimVascular.h"

#include <sv4guiModuleSegmentationExports.h>

#include "sv4gui_Contour.h"
#include "sv3_ContourGroup.h"

#include "mitkBaseData.h"
#include "mitkPoint.h"

#include <map>
#include <sstream>
#include <iostream>
#include <string>

using sv3::svLoftingParam;

class SV4GUIMODULESEGMENTATION_EXPORT sv4guiContourGroup : public mitk::BaseData, public sv3::ContourGroup
{
public:

    mitkClassMacro(sv4guiContourGroup, mitk::BaseData);
    itkFactorylessNewMacro(Self)
    itkCloneMacro(Self)

    virtual void Expand( unsigned int timeSteps ) override;

    virtual void ExecuteOperation(mitk::Operation* operation) override;

    virtual sv4guiContour* GetContour(int contourIndex, unsigned int t = 0) const;

    void InsertControlPoint(int contourIndex, int index, mitk::Point3D point, unsigned int t = 0 );

    void RemoveControlPoint(int contourIndex, int index, unsigned int t = 0);

    void SetControlPoint(int contourIndex, int index, mitk::Point3D point, unsigned int t = 0);

    void SetControlPointSelectedIndex(int contourIndex, int index, unsigned int t = 0);

    void DeselectControlPoint(unsigned int t = 0);

    void InsertContour(int contourIndex, sv4guiContour* contour, unsigned int t = 0);

    void RemoveContour(int contourIndex, unsigned int t = 0);

    void RemoveInvalidContours(unsigned int t = 0);

    void SetContour(int contourIndex, sv4guiContour* contour, unsigned int t = 0);

    bool IsContourSelected(int contourIndex, unsigned int t = 0) const;

    void SetContourSelected(int contourIndex, bool selected = true, unsigned int t = 0 );

    int GetSelectedContourIndex(unsigned int t = 0);

    void DeselectContours(unsigned int t = 0);

    void ContourControlPointsChanged(unsigned t = 0);

    void ContoursChanged(unsigned t = 0);

    sv4guiContour* GetUnplacedContour(unsigned int t = 0);

    sv4guiContour* GetContourOnPlane(const mitk::PlaneGeometry *planeGeometry, double precisionFactor=0.1, unsigned int t = 0);

    int GetUnplacedContourIndex(unsigned int t = 0);

    int SearchContourByPlane(const mitk::PlaneGeometry *planeGeometry, double precisionFactor=0.1, unsigned int t = 0);

    int GetInsertingContourIndexByPathPosPoint(mitk::Point3D posPoint, unsigned int t = 0);

    std::vector<sv4guiPathElement::sv4guiPathPoint>  GetContourPathPoints(unsigned int t = 0);

    std::vector<mitk::Point3D> GetContourPathPosPoints(unsigned int t = 0);

    int GetContourIndexByPathPosPoint(mitk::Point3D posPoint, unsigned int t = 0);

    //virtual methods, that need to be implemented
    virtual void UpdateOutputInformation() override;
    virtual void SetRequestedRegionToLargestPossibleRegion() override;
    virtual bool RequestedRegionIsOutsideOfTheBufferedRegion() override;
    virtual bool VerifyRequestedRegion() override;
    virtual void SetRequestedRegion(const itk::DataObject *data) override;

    int GetCurrentIndexOn2DView();

    void SetCurrentIndexOn2DView(int index);

    std::vector<sv4guiContour*> GetContourSet(unsigned int t = 0);

    std::vector<sv4guiContour*> GetValidContourSet(unsigned int t = 0);//contour points > 1
    
    double GetResliceSize() const {return m_ResliceSize;}
    
    void SetResliceSize(double size) {m_ResliceSize=size;}
    
    bool IsDataModified();
    
    void SetDataModified(bool modified = true);

  protected:

    mitkCloneMacro(Self);

    sv4guiContourGroup();
    sv4guiContourGroup(const sv4guiContourGroup &other);
    virtual ~sv4guiContourGroup();

    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const override;
    virtual void ClearData() override;

    virtual void InitializeEmpty() override;

    int m_CurrentIndexOn2DView;

    bool m_DataModified;

    double m_ResliceSize;
  };

itkEventMacro( sv4guiContourGroupAllEvent, itk::AnyEvent );

itkEventMacro( sv4guiContourGroupEvent, sv4guiContourGroupAllEvent );
itkEventMacro( sv4guiContourEvent, sv4guiContourGroupAllEvent );

itkEventMacro( sv4guiContourSelectEvent, sv4guiContourGroupEvent );
itkEventMacro( sv4guiContourGroupChangeEvent, sv4guiContourGroupEvent );
itkEventMacro( sv4guiContourSetEvent, sv4guiContourGroupChangeEvent );
itkEventMacro( sv4guiContourGroupSizeChangeEvent, sv4guiContourGroupChangeEvent );
itkEventMacro( sv4guiContourInsertEvent, sv4guiContourGroupSizeChangeEvent );
itkEventMacro( sv4guiContourRemoveEvent, sv4guiContourGroupSizeChangeEvent );
itkEventMacro( sv4guiContourGroupExtendTimeRangeEvent, sv4guiContourGroupChangeEvent );

itkEventMacro( sv4guiContourPointSelectEvent, sv4guiContourEvent );
itkEventMacro( sv4guiContourChangeEvent, sv4guiContourEvent );
itkEventMacro( sv4guiContourPointMoveEvent, sv4guiContourChangeEvent );
itkEventMacro( sv4guiContourSizeChangeEvent, sv4guiContourChangeEvent );
itkEventMacro( sv4guiContourPointInsertEvent, sv4guiContourSizeChangeEvent );
itkEventMacro( sv4guiContourPointRemoveEvent, sv4guiContourSizeChangeEvent );

#endif // SV4GUI_CONTOURGROUP_H
