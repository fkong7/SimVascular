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

#include "sv3_ContourGroup.h"
#include "sv_RepositoryData.h"
#include "sv_Math.h"
#include "sv3_PathElement.h"
#include "sv3_PathGroup.h"

using sv3::ContourGroup;
using sv3::Contour;
using sv3::PathElement;
using sv3::PathGroup;
using sv3::svLoftingParam;

ContourGroup::ContourGroup()
    : cvRepositoryData( CONTOURGROUP_T )
    , m_CalculateBoundingBox(true)
    , m_PathID(-1)
    , m_PathName("")
    , m_GroupID(-1)
    //, m_CurrentIndexOn2DView(-2)
    , m_DataModified(false)
    //, m_ResliceSize(5.0)
{
    this->InitializeEmpty();
    m_LoftingParam=new svLoftingParam();
}

ContourGroup::ContourGroup(const ContourGroup &other)
    : cvRepositoryData( CONTOURGROUP_T)
    , m_PathID(other.m_PathID)
    , m_PathName(other.m_PathName)
    , m_ContourSets(other.GetTimeSize())
    , m_DataModified(true)
    , m_CalculateBoundingBox(true)
    //, m_ResliceSize(other.m_ResliceSize)
    , m_Props(other.m_Props)
{
    for (std::size_t t = 0; t < other.GetTimeSize(); ++t)
    {
        for(std::size_t i=0; i< other.GetSize(t); ++i)
        {
            m_ContourSets[t].push_back(other.GetContour(i,t)->Clone());
        }
    }

    //     m_GroupID=other.GetGroupID();

    m_LoftingParam=new svLoftingParam(*(other.m_LoftingParam));

}

ContourGroup::~ContourGroup()
{
    this->ClearData();
    if(m_LoftingParam)
        delete m_LoftingParam;
}

void ContourGroup::ClearData()
{
    //may need delele each arrays inside first.
    m_ContourSets.clear();
    //Superclass::ClearData();
}

void ContourGroup::InitializeEmpty()
{
    m_ContourSets.resize(1);
    //    m_CalculateBoundingBox = false;

    //Superclass::InitializeTimeGeometry(1);
    //m_Initialized = true;
}

bool ContourGroup::IsEmptyTimeStep(unsigned int t) const
{
//    return IsInitialized() && (GetSize(t) == 0);
    return false;
}

void ContourGroup::Expand( unsigned int timeSteps )
{
    unsigned int oldSize = m_ContourSets.size();

    if ( timeSteps > oldSize )
    {
//        Superclass::Expand( timeSteps );

        m_ContourSets.resize( timeSteps );

        //if the size changes, then compute the bounding box
        m_CalculateBoundingBox = true;

//        this->InvokeEvent( ContourGroupExtendTimeRangeEvent() );
    }
}

unsigned int ContourGroup::GetTimeSize() const
{
    return m_ContourSets.size();
}

int ContourGroup::GetSize( unsigned int t ) const
{
    if ( t < m_ContourSets.size() )
    {
        return m_ContourSets[t].size();
    }
    else
    {
        return 0;
    }
}

Contour* ContourGroup::GetContour(int contourIndex, unsigned int t) const
{
    if ( t < m_ContourSets.size())
    {
        if(contourIndex==-1) contourIndex=m_ContourSets[t].size()-1;

        if (contourIndex>-1 && contourIndex<m_ContourSets[t].size())
        {
            return m_ContourSets[t][contourIndex];
        }else{
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

void ContourGroup::ContourControlPointsChanged(unsigned int t)
{
    std::cout<<"ContourControlPointsChangedsv3 "<<std::endl;
//    this->Modified();
}

void ContourGroup::ContoursChanged(unsigned int t)
{
//    this->Modified();
}

void ContourGroup::InsertControlPoint(int contourIndex, int index, std::array<double,3>  point, unsigned int t)
{
    std::cout<<"InsertControlPointsv3 "<<std::endl;
    Contour* contour=GetContour(contourIndex,t);
    if(contour)
    {
        contour->InsertControlPoint(index,point);

        ContourControlPointsChanged(t);
 //       this->InvokeEvent( ContourPointInsertEvent() );
    }
}

void ContourGroup::RemoveControlPoint(int contourIndex, int index, unsigned int t)
{
    Contour* contour=GetContour(contourIndex,t);
    if(contour)
    {
        contour->RemoveControlPoint(index);

        ContourControlPointsChanged(t);
//        this->InvokeEvent( ContourPointRemoveEvent() );

    }
}

void ContourGroup::SetControlPoint(int contourIndex, int index, std::array<double,3>  point, unsigned int t)
{
    Contour* contour=GetContour(contourIndex,t);
    if(contour)
    {
        contour->SetControlPoint(index,point);

        ContourControlPointsChanged(t);
//        this->InvokeEvent( ContourPointMoveEvent() );
    }
}

void ContourGroup::SetControlPointSelectedIndex(int contourIndex, int index, unsigned int t)
{
    Contour* contour=GetContour(contourIndex,t);
    if(contour)
    {
        contour->SetControlPointSelectedIndex(index);

        ContourControlPointsChanged(t);
//        this->InvokeEvent( ContourPointSelectEvent() );
    }
}

void ContourGroup::DeselectControlPoint(unsigned int t)
{
    for(int i=0;i<GetSize(t);i++){
        Contour* contour=GetContour(i,t);
        if(contour) contour->DeselectControlPoint();
    }
    ContourControlPointsChanged(t);
//    this->InvokeEvent( ContourPointSelectEvent() );
}

int ContourGroup::GetControlPointSelectedIndex(int contourIndex, unsigned int t)
{
    Contour* contour=GetContour(contourIndex,t);
    if(contour)
    {
        return contour->GetControlPointSelectedIndex();
    }else{
        return -2;
    }
}

void ContourGroup::InsertContour(int contourIndex, Contour* contour, unsigned int t)
{
    this->Expand(t+1);
    if(t<m_ContourSets.size())
    {
        if(contourIndex==-1) contourIndex=m_ContourSets[t].size();

        if(contourIndex>-1 && contourIndex<=m_ContourSets[t].size())
        {
            m_ContourSets[t].insert(m_ContourSets[t].begin()+contourIndex,contour);
            ContoursChanged(t);
//            this->InvokeEvent( ContourInsertEvent() );
        }
    }
}

void ContourGroup::RemoveContour(int contourIndex, unsigned int t)
{
    if(t<m_ContourSets.size() )
    {
        if(contourIndex==-1) contourIndex=m_ContourSets[t].size()-1;

        if(contourIndex>-1 && contourIndex<m_ContourSets[t].size())
        {
            m_ContourSets[t].erase(m_ContourSets[t].begin()+contourIndex);
            ContoursChanged(t);
//            this->InvokeEvent( ContourRemoveEvent() );
        }
    }
}

void ContourGroup::RemoveInvalidContours(unsigned int t)
{
    if(t<m_ContourSets.size() )
    {
        for(int i=m_ContourSets[t].size()-1;i>-1;i--)
        {
            Contour* contour=m_ContourSets[t][i];
            if(contour==NULL || contour->GetContourPointNumber()<3)
                RemoveContour(i,t);
        }
    }
}

void ContourGroup::SetContour(int contourIndex, Contour* contour, unsigned int t)
{
    if(t<m_ContourSets.size())
    {
        if(contourIndex==-1) contourIndex=m_ContourSets[t].size()-1;

        if(contourIndex>-1 && contourIndex<m_ContourSets[t].size())
        {
            m_ContourSets[t][contourIndex]=contour;
            ContoursChanged(t);
//            this->InvokeEvent( ContourSetEvent() );
        }
    }
}

// bool ContourGroup::IsContourSelected(int contourIndex, unsigned int t)
// {
//     Contour* contour=GetContour(contourIndex,t);
//     if(contour)
//     {
//         return contour->IsSelected();
//     }else{
//         return false;
//     }
// }

// void ContourGroup::SetContourSelected(int contourIndex, bool selected, unsigned int t)
// {
//     Contour* contour=GetContour(contourIndex,t);
//     if(contour)
//     {
//         if(contour->IsSelected()!=selected)
//         {
//             contour->SetSelected(selected);
//             ContoursChanged(t);
// //            this->InvokeEvent( ContourEvent() );
// 
//         }
//     }
// }

// void ContourGroup::DeselectContours(unsigned int t)
// {
//     for(int i=0;i<GetSize(t);i++){
//         Contour* contour=GetContour(i,t);
//         if(contour) contour->SetSelected(false);
//     }
//     ContoursChanged(t);
// //    this->InvokeEvent( ContourEvent() );
// }
// 
// int ContourGroup::GetSelectedContourIndex(unsigned int t)
// {
//     for(int i=0;i<GetSize(t);i++){
//         Contour* contour=GetContour(i,t);
//         if(contour&&contour->IsSelected())
//             return i;
//     }
//     return -2;
// }

// int ContourGroup::GetUnplacedContourIndex(unsigned int t)
// {
//     for(int i=0;i<GetSize(t);i++){
//         Contour* contour=GetContour(i,t);
//         if(contour&&!contour->IsPlaced())
//             return i;
//     }
//     return -2;
// }
// 
// Contour* ContourGroup::GetUnplacedContour(unsigned int t)
// {
//     int contourIndex=GetUnplacedContourIndex(t);
//     return GetContour(contourIndex,t);
// }

// int ContourGroup::SearchContourByPlane(const mitk::PlaneGeometry *planeGeometry, double precisionFactor, unsigned int t)
// {
//     if(planeGeometry!=NULL)
//     {
//         std::array<double,3>  center=planeGeometry->GetCenter();
//         mitk::Vector3D spacing=planeGeometry->GetSpacing();
//         double minDist=sqrt(spacing[0]*spacing[0]+spacing[1]*spacing[1]+spacing[2]*spacing[2]);
// 
//         for(int i=0;i<GetSize(t);i++){
// 
//             Contour* contour=GetContour(i,t);
//             if(contour==NULL) continue;
// 
//             if(contour->IsOnPlane(planeGeometry,precisionFactor)){
//                 double dist=center.EuclideanDistanceTo(contour->GetPathPosPoint());
//                 if(dist<2*minDist)
//                     return i;
// 
//             }
//         }
//     }
// 
//     return -2;
// }
// 
// Contour* ContourGroup::GetContourOnPlane(const mitk::PlaneGeometry *planeGeometry, double precisionFactor, unsigned int t)
// {
//     int contourIndex=SearchContourByPlane(planeGeometry,precisionFactor,t);
//     return GetContour(contourIndex,t);
// }

// void ContourGroup::ExecuteOperation( mitk::Operation* operation )
// {
//     int timeStep = -1;
// 
//     ContourOperation* contourOperation = dynamic_cast<ContourOperation*>(operation);
// 
//     if ( contourOperation )
//     {
//         timeStep = contourOperation->GetTimeStep();
// 
//     }else{
//         MITK_ERROR << "No valid Contour Operation for ContourGroup" << std::endl;
//         return;
//     }
// 
//     if ( timeStep < 0 )
//     {
//         MITK_ERROR << "Time step (" << timeStep << ") outside of ContourGroup time bounds" << std::endl;
//         return;
//     }
// 
//     int contourIndex=contourOperation->GetContourIndex();
//     Contour* contour=contourOperation->GetContour();
//     int index = contourOperation->GetIndex();
//     std::array<double,3>  point=contourOperation->GetPoint();
// 
//     switch (contourOperation->GetOperationType())
//     {
//     case ContourOperation::OpINSERTCONTROLPOINT:
//     {
//         InsertControlPoint(contourIndex,index,point,timeStep);
//         m_CalculateBoundingBox = true;
//         m_DataModified=true;
//     }
//         break;
// 
//     case ContourOperation::OpMOVECONTROLPOINT:
//     {
//         SetControlPoint(contourIndex,index, point, timeStep);
//         m_CalculateBoundingBox = true;
//         m_DataModified=true;
//     }
//         break;
// 
//     case ContourOperation::OpREMOVECONTROLPOINT:
//     {
//         RemoveControlPoint(contourIndex,index,timeStep);
//         m_CalculateBoundingBox = true;
//         m_DataModified=true;
//     }
//         break;
// 
//     case ContourOperation::OpINSERTCONTOUR:
//     {
//         InsertContour(contourIndex,contour,timeStep);
//         m_CalculateBoundingBox = true;
//         m_DataModified=true;
//     }
//         break;
// 
//     case ContourOperation::OpREMOVECONTOUR:
//     {
//         RemoveContour(contourIndex,timeStep);
//         m_CalculateBoundingBox = true;
//         m_DataModified=true;
//     }
//         break;
// 
//     case ContourOperation::OpSETCONTOUR:
//     {
//         SetContour(contourIndex,contour,timeStep);
//         m_CalculateBoundingBox = true;
//         m_DataModified=true;
//     }
//         break;
// 
//     default:
//         itkWarningMacro("ContourGroup could not understrand the operation. Please check!");
//         break;
//     }
// 
//     mitk::OperationEndEvent endevent(operation);
//     ((const itk::Object*)this)->InvokeEvent(endevent);
// }

void ContourGroup::CalculateBoundingBox(double *bounds,unsigned int t)
{
    double contourBounds[6]={0};
    bool firstTime=true;
    for(int i=0;i<GetSize(t);i++){
        Contour* contour=GetContour(i,t);
        if(contour)
        {
            contour->CalculateBoundingBox(contourBounds);
            if(firstTime)
            {
                bounds[0]=contourBounds[0];
                bounds[1]=contourBounds[1];
                bounds[2]=contourBounds[2];
                bounds[3]=contourBounds[3];
                bounds[4]=contourBounds[4];
                bounds[5]=contourBounds[5];
                firstTime=false;
            }else{
                if(contourBounds[0]<bounds[0]) bounds[0]=contourBounds[0];
                if(contourBounds[1]>bounds[1]) bounds[1]=contourBounds[1];
                if(contourBounds[2]<bounds[2]) bounds[2]=contourBounds[2];
                if(contourBounds[3]>bounds[3]) bounds[3]=contourBounds[3];
                if(contourBounds[4]<bounds[4]) bounds[4]=contourBounds[4];
                if(contourBounds[5]>bounds[5]) bounds[5]=contourBounds[5];
            }
        }
    }

}

// void ContourGroup::UpdateOutputInformation()
// {
//     if ( this->GetSource( ) )
//     {
//         this->GetSource( )->UpdateOutputInformation( );
//     }
// 
//     mitk::TimeGeometry* timeGeometry = GetTimeGeometry();
//     if ( timeGeometry->CountTimeSteps() != m_ContourSets.size() )
//     {
//         itkExceptionMacro(<<"timeGeometry->CountTimeSteps() != m_ContourSets.size() -- use Initialize(timeSteps) with correct number of timeSteps!");
//     }
// 
//     if (m_CalculateBoundingBox)
//     {
//         for ( unsigned int t = 0 ; t < m_ContourSets.size() ; ++t )
//         {
//             double bounds[6] = {0};
//             CalculateBoundingBox(bounds,t);
//             this->GetGeometry(t)->SetFloatBounds(bounds);
//         }
// 
//         m_CalculateBoundingBox = false;
//     }
// 
//     this->GetTimeGeometry()->Update();
// }

// void ContourGroup::SetRequestedRegionToLargestPossibleRegion()
// {
// }
// 
// bool ContourGroup::RequestedRegionIsOutsideOfTheBufferedRegion()
// {
//     return false;
// }
// 
// bool ContourGroup::VerifyRequestedRegion()
// {
//     return true;
// }
// 
// void ContourGroup::SetRequestedRegion(const DataObject * )
// {
// }
// 
// void ContourGroup::PrintSelf( std::ostream& os, itk::Indent indent ) const
// {
//     Superclass::PrintSelf(os, indent);
// 
//     os << indent << "Number timesteps: " << m_ContourSets.size() << "\n";
// 
//     for ( unsigned int t = 0 ; t < m_ContourSets.size() ; ++t )
//     {
// 
//             os << indent << "Timestep " << t << ": \n";
//             itk::Indent nextIndent = indent.GetNextIndent();
// 
//             for(int i=0;i<m_ContourSets[t].size();i++)
//             {
//                 if(m_ContourSets[t][i])
//                 {
//                     os << nextIndent << "Contour " << i << ": ";
//                     os << "selected: " << m_ContourSets[t][i]->IsSelected() << "\n";
//                 }else{
//                     os << nextIndent << "Contour " << i << ": doesn't exist \n";
//                 }
// 
//             }
// 
//     }
// }

std::string ContourGroup::GetPathName() const
{
    return m_PathName;
}

void ContourGroup::SetPathName(std::string name)
{
    m_PathName=name;
}

int ContourGroup::GetPathID() const
{
    return m_PathID;
}

void ContourGroup::SetPathID(int id)
{
    m_PathID=id;
}

std::vector<PathElement::PathPoint>  ContourGroup::GetContourPathPoints(unsigned int t)
{
    std::vector<PathElement::PathPoint> pathPoints;
    for(int i=0;i<m_ContourSets[t].size();i++)
        pathPoints.push_back(m_ContourSets[t][i]->GetPathPoint());

    return pathPoints;
}

std::vector<std::array<double,3> > ContourGroup::GetContourPathPosPoints(unsigned int t)
{
    std::vector<std::array<double,3> > points;
    for(int i=0;i<m_ContourSets[t].size();i++)
        points.push_back(m_ContourSets[t][i]->GetPathPoint().pos);

    return points;
}

int ContourGroup::GetInsertingContourIndexByPathPosPoint(std::array<double,3>  posPoint, unsigned int t)
{
    if(GetSize(t)==0)
        return 0;

    cvMath* math= new cvMath();
    int index = math->GetInsertintIndexByDistance(GetContourPathPosPoints(t),posPoint);
    delete math;
    return index;
}

int ContourGroup::GetInsertingContourIndexByTagIndex(int tagIndex, unsigned int t)
{
    for(int i=0;i<GetSize(t);i++)
    {
      Contour* contour=GetContour(i,t);
      if(!contour)
          continue;

      if(tagIndex<=contour->GetTagIndex())
          return i;
    }

    if(GetSize(t)==0)
        return 0;
    else
        return GetSize(t);
}

int ContourGroup::GetContourIndexByPathPosPoint(std::array<double,3>  posPoint, unsigned int t)
{
    for(int i=0;i<GetSize(t);i++)
    {
      Contour* contour=GetContour(i,t);
      if(contour&&contour->GetPathPosPoint()==posPoint)
          return i;
    }

    return -2;
}

// int ContourGroup::GetCurrentIndexOn2DView()
// {
//     return m_CurrentIndexOn2DView;
// }
// 
// void ContourGroup::SetCurrentIndexOn2DView(int index)
// {
//     m_CurrentIndexOn2DView=index;
// }

std::vector<Contour*> ContourGroup::GetContourSet(unsigned int t)
{
    return m_ContourSets[t];
}

std::vector<Contour*> ContourGroup::GetValidContourSet(unsigned int t)
{
    std::vector<Contour*> contourSet;
    for(int i=0;i<m_ContourSets[t].size();i++)
    {
        Contour* contour=m_ContourSets[t][i];
        if(contour && contour->GetContourPointNumber()>2)
            contourSet.push_back(contour);
    }

    return contourSet;
}

void ContourGroup::SetProp(const std::string& key, std::string value)
{
    m_Props[key]=value;
}

std::string ContourGroup::GetProp(const std::string& key) const
{
    std::map<std::string,std::string>* p=const_cast<std::map<std::string,std::string>*>(&m_Props);
    return (*p)[key];
}

bool ContourGroup::IsDataModified(){return m_DataModified;}

void ContourGroup::SetDataModified(bool modified){m_DataModified=modified;}

//double ContourGroup::GetResliceSize() const {return m_ResliceSize;}

//void ContourGroup::SetResliceSize(double size) {m_ResliceSize=size;}

std::map<std::string,std::string> ContourGroup::GetProps() {return m_Props;}
    
