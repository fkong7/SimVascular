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

#include "sv4gui_ContourGroup.h"
#include "sv3_ContourGroup.h"
#include "sv3_PathElement.h"
#include "sv4gui_ContourOperation.h"
#include "sv_Math.h"

using sv3::PathElement;
using sv3::Contour;

sv4guiContourGroup::sv4guiContourGroup()
    : ContourGroup()
    , m_CurrentIndexOn2DView(-2)
    , m_ResliceSize(5.0)
    , m_DataModified(false)
{
    this->InitializeEmpty();
}

sv4guiContourGroup::sv4guiContourGroup(const sv4guiContourGroup &other)
    : BaseData(other)
    , ContourGroup(other)
    , m_ResliceSize(other.m_ResliceSize)
    , m_DataModified(true)
{

}

sv4guiContourGroup::~sv4guiContourGroup()
{
    this->ClearData();
}

void sv4guiContourGroup::ClearData()
{
    //may need delele each arrays inside first.
    this->sv3::ContourGroup::ClearData();
    Superclass::ClearData();
}

void sv4guiContourGroup::InitializeEmpty()
{
    this->sv3::ContourGroup::InitializeEmpty();
    Superclass::InitializeTimeGeometry(1);
    m_Initialized = true;
}

void sv4guiContourGroup::Expand( unsigned int timeSteps )
{
    
    if ( timeSteps > sv3::ContourGroup::GetTimeSize() )
    {
        Superclass::Expand( timeSteps );
        
        this->sv3::ContourGroup::Expand(timeSteps);

        this->InvokeEvent( sv4guiContourGroupExtendTimeRangeEvent() );
    }
    
}



sv4guiContour* sv4guiContourGroup::GetContour(int contourIndex, unsigned int t) const
{
    sv3::Contour *contour = this->sv3::ContourGroup::GetContour(contourIndex,t);
    //need to convert contour here
    sv4guiContour* guiContour = dynamic_cast<sv4guiContour*>(contour);
    return guiContour;
}

void sv4guiContourGroup::ContourControlPointsChanged(unsigned int t)
{
    this->Modified();
}

void sv4guiContourGroup::ContoursChanged(unsigned int t)
{
    this->Modified();
}

void sv4guiContourGroup::InsertControlPoint(int contourIndex, int index, mitk::Point3D point, unsigned int t)
{
    sv4guiContour* contour = this->GetContour(contourIndex,t);
    if(contour!=NULL)
    {
        contour->InsertControlPoint(index,point);
        ContourControlPointsChanged(t);
        this->InvokeEvent( sv4guiContourPointInsertEvent() );
    }
}

void sv4guiContourGroup::RemoveControlPoint(int contourIndex, int index, unsigned int t)
{
    sv4guiContour* contour = this->GetContour(contourIndex,t);
    if(contour)
    {
        contour->RemoveControlPoint(index);
        ContourControlPointsChanged(t);
        this->InvokeEvent( sv4guiContourPointRemoveEvent() );

    }
}

void sv4guiContourGroup::SetControlPoint(int contourIndex, int index, mitk::Point3D point, unsigned int t)
{     
    sv4guiContour* contour = this->GetContour(contourIndex,t);
    if(contour)
    {
        contour->SetControlPoint(index,point);
        ContourControlPointsChanged(t);
        this->InvokeEvent( sv4guiContourPointMoveEvent() );
    }
}

void sv4guiContourGroup::SetControlPointSelectedIndex(int contourIndex, int index, unsigned int t)
{

    sv4guiContour* contour = this->GetContour(contourIndex,t);
    if(contour)
    {
        contour->SetControlPointSelectedIndex(index);
        ContourControlPointsChanged(t);
        this->InvokeEvent( sv4guiContourPointSelectEvent() );
    }
}

void sv4guiContourGroup::DeselectControlPoint(unsigned int t)
{
    this->sv3::ContourGroup::DeselectControlPoint(t);

    ContourControlPointsChanged(t);
    this->InvokeEvent( sv4guiContourPointSelectEvent() );
}

void sv4guiContourGroup::InsertContour(int contourIndex, sv4guiContour* guiContour, unsigned int t)
{
    //need to conver contour
    Contour* contour = dynamic_cast<Contour*>(guiContour);

    this->Expand(t+1);
    if(t<m_ContourSets.size())
    {
        if(contourIndex==-1) contourIndex= sv3::ContourGroup::GetSize(t);

        if(contourIndex>-1 && contourIndex<=sv3::ContourGroup::GetSize(t))
        {
            this->sv3::ContourGroup::InsertContour(contourIndex,contour,t);
            ContoursChanged(t);
            this->InvokeEvent( sv4guiContourInsertEvent() );
        }
    }
}

void sv4guiContourGroup::RemoveContour(int contourIndex, unsigned int t)
{
    if(t<m_ContourSets.size() )
    {
        if(contourIndex==-1) contourIndex=sv3::ContourGroup::GetSize(t)-1;

        if(contourIndex>-1 && contourIndex<sv3::ContourGroup::GetSize(t))
        {
            this-> sv3::ContourGroup::RemoveContour(contourIndex,t);
            ContoursChanged(t);
            this->InvokeEvent( sv4guiContourRemoveEvent() );
        }
    }
}

void sv4guiContourGroup::RemoveInvalidContours(unsigned int t)
{
    if(t<sv3::ContourGroup::GetSize(t) )
    {
        for(int i=sv3::ContourGroup::GetSize(t)-1;i>-1;i--)
        {
            sv3::Contour* contour=sv3::ContourGroup::GetContour(i,t);
            if(contour==NULL || contour->GetContourPointNumber()<3)
                RemoveContour(i,t);
        }
    }
}

void sv4guiContourGroup::SetContour(int contourIndex, sv4guiContour* guiContour, unsigned int t)
{
    //need to convert contours here:
    Contour* contour = dynamic_cast<Contour*>(guiContour);

    if(t<m_ContourSets.size())
    {
        if(contourIndex==-1) contourIndex=sv3::ContourGroup::GetSize(t-1);

        if(contourIndex>-1 && contourIndex<sv3::ContourGroup::GetSize(t))
        {
            this->sv3::ContourGroup::SetContour(contourIndex, contour,t);
            ContoursChanged(t);
            this->InvokeEvent( sv4guiContourSetEvent() );
        }
    }
}

bool sv4guiContourGroup::IsContourSelected(int contourIndex, unsigned int t) const
{
    sv4guiContour* contour=GetContour(contourIndex,t);
    if(contour)
    {
        return contour->IsSelected();
    }else{
        return false;
    }
}

void sv4guiContourGroup::SetContourSelected(int contourIndex, bool selected, unsigned int t)
{
    sv4guiContour* contour=GetContour(contourIndex,t);
    if(contour)
    {
        if(contour->IsSelected()!=selected)
        {
            contour->SetSelected(selected);
            ContoursChanged(t);
        }
    }
}

void sv4guiContourGroup::DeselectContours(unsigned int t)
{
    for(int i=0;i<GetSize(t);i++){
        sv4guiContour* contour=GetContour(i,t);
        if(contour) contour->SetSelected(false);
    }
    ContoursChanged(t);
}

int sv4guiContourGroup::GetSelectedContourIndex(unsigned int t)
{
    for(int i=0;i<GetSize(t);i++){
        sv4guiContour* contour=GetContour(i,t);
        if(contour&&contour->IsSelected())
            return i;
    }
    return -2;
}

int sv4guiContourGroup::GetUnplacedContourIndex(unsigned int t)
{
    for(int i=0;i<GetSize(t);i++){
        sv4guiContour* contour=GetContour(i,t);
        if(contour&&!contour->IsPlaced())
            return i;
    }
    return -2;
}

sv4guiContour* sv4guiContourGroup::GetUnplacedContour(unsigned int t)
{
    int contourIndex=GetUnplacedContourIndex(t);
    return GetContour(contourIndex,t);
}

int sv4guiContourGroup::SearchContourByPlane(const mitk::PlaneGeometry *planeGeometry, double precisionFactor, unsigned int t)
{
    if(planeGeometry!=NULL)
    {
        mitk::Point3D center=planeGeometry->GetCenter();
        mitk::Vector3D spacing=planeGeometry->GetSpacing();
        double minDist=sqrt(spacing[0]*spacing[0]+spacing[1]*spacing[1]+spacing[2]*spacing[2]);

        for(int i=0;i<GetSize(t);i++){

            sv4guiContour* contour=GetContour(i,t);
            if(contour==NULL) continue;

            if(contour->IsOnPlane(planeGeometry,precisionFactor)){
                double dist=center.EuclideanDistanceTo(contour->GetPathPosPoint());
                if(dist<2*minDist)
                    return i;

            }
        }
    }

    return -2;
}

sv4guiContour* sv4guiContourGroup::GetContourOnPlane(const mitk::PlaneGeometry *planeGeometry, double precisionFactor, unsigned int t)
{
    int contourIndex=SearchContourByPlane(planeGeometry,precisionFactor,t);
    return GetContour(contourIndex,t);
}

void sv4guiContourGroup::ExecuteOperation( mitk::Operation* operation )
{
    int timeStep = -1;

    sv4guiContourOperation* contourOperation = dynamic_cast<sv4guiContourOperation*>(operation);

    if ( contourOperation )
    {
        timeStep = contourOperation->GetTimeStep();

    }else{
        MITK_ERROR << "No valid Contour Operation for sv4guiContourGroup" << std::endl;
        return;
    }

    if ( timeStep < 0 )
    {
        MITK_ERROR << "Time step (" << timeStep << ") outside of sv4guiContourGroup time bounds" << std::endl;
        return;
    }

    int contourIndex=contourOperation->GetContourIndex();
    sv4guiContour* contour=contourOperation->GetContour();
    int index = contourOperation->GetIndex();
    mitk::Point3D point=contourOperation->GetPoint();

    switch (contourOperation->GetOperationType())
    {
    case sv4guiContourOperation::OpINSERTCONTROLPOINT:
    {
        InsertControlPoint(contourIndex,index,point,timeStep);
        m_CalculateBoundingBox = true;
        m_DataModified=true;
    }
        break;

    case sv4guiContourOperation::OpMOVECONTROLPOINT:
    {
        SetControlPoint(contourIndex,index, point, timeStep);
        m_CalculateBoundingBox = true;
        m_DataModified=true;
    }
        break;

    case sv4guiContourOperation::OpREMOVECONTROLPOINT:
    {
        RemoveControlPoint(contourIndex,index,timeStep);
        m_CalculateBoundingBox = true;
        m_DataModified=true;
    }
        break;

    case sv4guiContourOperation::OpINSERTCONTOUR:
    {
        InsertContour(contourIndex,contour,timeStep);
        m_CalculateBoundingBox = true;
        m_DataModified=true;
    }
        break;

    case sv4guiContourOperation::OpREMOVECONTOUR:
    {
        RemoveContour(contourIndex,timeStep);
        m_CalculateBoundingBox = true;
        m_DataModified=true;
    }
        break;

    case sv4guiContourOperation::OpSETCONTOUR:
    {
        SetContour(contourIndex,contour,timeStep);
        m_CalculateBoundingBox = true;
        m_DataModified=true;
    }
        break;

    default:
        itkWarningMacro("sv4guiContourGroup could not understrand the operation. Please check!");
        break;
    }

    mitk::OperationEndEvent endevent(operation);
    ((const itk::Object*)this)->InvokeEvent(endevent);
}

void sv4guiContourGroup::UpdateOutputInformation()
{
    if ( this->GetSource( ) )
    {
        this->GetSource( )->UpdateOutputInformation( );
    }

    mitk::TimeGeometry* timeGeometry = GetTimeGeometry();
    if ( timeGeometry->CountTimeSteps() != m_ContourSets.size() )
    {
        std::cout<<"timeGeomtry: "<<timeGeometry->CountTimeSteps()<<std::endl;
        std::cout<<"contoursets: "<<m_ContourSets.size() <<std::endl;
        itkExceptionMacro(<<"timeGeometry->CountTimeSteps() != m_ContourSets.size() -- use Initialize(timeSteps) with correct number of timeSteps!");
    }

    if (m_CalculateBoundingBox)
    {
        for ( unsigned int t = 0 ; t < m_ContourSets.size() ; ++t )
        {
            double bounds[6] = {0};
            CalculateBoundingBox(bounds,t);
            this->GetGeometry(t)->SetFloatBounds(bounds);
        }

        m_CalculateBoundingBox = false;
    }

    this->GetTimeGeometry()->Update();
}

void sv4guiContourGroup::SetRequestedRegionToLargestPossibleRegion()
{
}

bool sv4guiContourGroup::RequestedRegionIsOutsideOfTheBufferedRegion()
{
    return false;
}

bool sv4guiContourGroup::VerifyRequestedRegion()
{
    return true;
}

void sv4guiContourGroup::SetRequestedRegion(const DataObject * )
{
}

void sv4guiContourGroup::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
    Superclass::PrintSelf(os, indent);

    os << indent << "Number timesteps: " << m_ContourSets.size() << "\n";

    for ( unsigned int t = 0 ; t < m_ContourSets.size() ; ++t )
    {

            os << indent << "Timestep " << t << ": \n";
            itk::Indent nextIndent = indent.GetNextIndent();

            for(int i=0;i<m_ContourSets[t].size();i++)
            {
                if(m_ContourSets[t][i])
                {
                    os << nextIndent << "Contour " << i << ": ";
                    os << "selected: " << this->IsContourSelected(i,t) << "\n";
                }else{
                    os << nextIndent << "Contour " << i << ": doesn't exist \n";
                }

            }

    }
}

std::vector<sv4guiPathElement::sv4guiPathPoint>  sv4guiContourGroup::GetContourPathPoints(unsigned int t)
{
    std::vector<PathElement::PathPoint> pthPtVec = sv3::ContourGroup::GetContourPathPoints(t);
    //need to convert pathPoints here
    std::vector<sv4guiPathElement::sv4guiPathPoint> guiPthPtVec(pthPtVec.size());
    for (int j=0; j<pthPtVec.size(); j++)
    {
        for (int i = 0; i<3; i++)
        {
            guiPthPtVec[j].pos[i]=pthPtVec[j].pos[i];
            guiPthPtVec[j].tangent[i] = pthPtVec[j].tangent[i];
            guiPthPtVec[j].rotation[i] = pthPtVec[j].rotation[i];
        }
            guiPthPtVec[j].id = pthPtVec[j].id;
    }
    
    return guiPthPtVec;
}

std::vector<mitk::Point3D> sv4guiContourGroup::GetContourPathPosPoints(unsigned int t)
{
    std::vector<std::array<double,3> > ptVec = sv3::ContourGroup::GetContourPathPosPoints(t);
    std::vector<mitk::Point3D> points;
    for (int j=0; j<ptVec.size(); j++)
    {
        for (int i=0; i<3; i++)
            points[j][i] = ptVec[j][i];
    }
    return points;
}

int sv4guiContourGroup::GetInsertingContourIndexByPathPosPoint(mitk::Point3D posPoint, unsigned int t)
{
    std::array<double,3> pt;
    for (int i; i<3; i++)
        pt[i] = posPoint[i];
    return this->sv3::ContourGroup::GetInsertingContourIndexByPathPosPoint(pt,t);

}


int sv4guiContourGroup::GetContourIndexByPathPosPoint(mitk::Point3D posPoint, unsigned int t)
{
    std::array<double,3> pt;
    for (int i=0; i<3; i++)
        pt[i] = posPoint[i];
    return this->sv3::ContourGroup::GetContourIndexByPathPosPoint(pt,t);

}

int sv4guiContourGroup::GetCurrentIndexOn2DView()
{
    return m_CurrentIndexOn2DView;
}

void sv4guiContourGroup::SetCurrentIndexOn2DView(int index)
{
    m_CurrentIndexOn2DView=index;
}

std::vector<sv4guiContour*> sv4guiContourGroup::GetContourSet(unsigned int t)
{
    std::vector<sv3::Contour*> cts = this->sv3::ContourGroup::GetContourSet(t);
    //need to convert contours here
    std::vector<sv4guiContour*> guiCts(cts.size());
    for (int i=0; i<cts.size(); i++)
    {
        guiCts[i] = dynamic_cast<sv4guiContour*>(cts[i]);
    }
    return guiCts;
}

std::vector<sv4guiContour*> sv4guiContourGroup::GetValidContourSet(unsigned int t)
{
    std::vector<sv3::Contour*> cts = this->sv3::ContourGroup::GetValidContourSet(t);
    //need to convert contours here
    std::vector<sv4guiContour*> guiCts(cts.size());
    for (int i=0; i<cts.size(); i++)
    {
        guiCts[i] = dynamic_cast<sv4guiContour*>(cts[i]);
    }
    
    return guiCts;

}

bool sv4guiContourGroup::IsDataModified(){return m_DataModified;}

void sv4guiContourGroup::SetDataModified(bool modified){m_DataModified=modified;}

