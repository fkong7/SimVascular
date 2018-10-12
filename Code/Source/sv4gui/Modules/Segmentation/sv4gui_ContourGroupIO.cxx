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

#include "sv4gui_ContourGroupIO.h"
#include "sv3_ContourGroup.h"
#include "sv4gui_ContourGroup.h"
#include "sv4gui_ContourCircle.h"
#include "sv4gui_ContourEllipse.h"
#include "sv4gui_ContourPolygon.h"
#include "sv4gui_ContourSplinePolygon.h"
#include "sv4gui_ContourTensionPolygon.h"
#include "sv4gui_XmlIOUtil.h"

#include <mitkCustomMimeType.h>
#include <mitkIOMimeTypes.h>

static mitk::CustomMimeType Createsv4guiContourGroupMimeType()
{
    mitk::CustomMimeType mimeType(mitk::IOMimeTypes::DEFAULT_BASE_NAME() + ".svcontourgroup");
    mimeType.SetCategory("SimVascular Files");
    mimeType.AddExtension("ctgr");
    mimeType.SetComment("SimVascular ContourGroup");

    return mimeType;
}

sv4guiContourGroupIO::sv4guiContourGroupIO()
    : mitk::AbstractFileIO(sv4guiContourGroup::GetStaticNameOfClass(), Createsv4guiContourGroupMimeType(), "SimVascular ContourGroup")
{
    this->RegisterService();
}

std::vector<mitk::BaseData::Pointer> sv4guiContourGroupIO::Read()
{
    std::string fileName=GetInputLocation();
    return ReadFile(fileName);
}

std::vector<mitk::BaseData::Pointer> sv4guiContourGroupIO::ReadFile(std::string fileName)
{
    TiXmlDocument document;

    if (!document.LoadFile(fileName))
    {
        mitkThrow() << "Could not open/read/parse " << fileName;
        //        MITK_ERROR << "Could not open/read/parse " << fileName;
        std::vector<mitk::BaseData::Pointer> empty;
        return empty;
    }

    //    TiXmlElement* version = document.FirstChildElement("format");

    TiXmlElement* groupElement = document.FirstChildElement("contourgroup");

    if(!groupElement){
//        MITK_ERROR << "No ContourGroup data in "<< fileName;
        mitkThrow() << "No ContourGroup data in "<< fileName;
    }

    sv4guiContourGroup::Pointer group = sv4guiContourGroup::New();
    sv3::ContourGroupIO* reader = new sv3::ContourGroupIO();
    sv3::ContourGroup* svContourGrp = reader->ReadFile(fileName);
    delete reader;
    
    group->SetPathName(svContourGrp->GetPathName());
    group->SetPathID(svContourGrp->GetPathID());

    double resliceSize=5.0;
    groupElement->QueryDoubleAttribute("reslice_size", &resliceSize);
    group->SetResliceSize(resliceSize);

    std::string point2dsize="",point3dsize="";
    groupElement->QueryStringAttribute("point_2D_display_size", &point2dsize);
    groupElement->QueryStringAttribute("point_size", &point3dsize);
    group->SetProp("point 2D display size",point2dsize);
    group->SetProp("point size",point3dsize);
    
    for (int i=0; i<svContourGrp->GetTimeSize(); i++)
    {
        group->Expand(i+1);
        std::vector<Contour*> ctSet = svContourGrp->GetContourSet(i);
        
        for(int j = 0; j<ctSet.size(); j++)
        {
            std::string type = ctSet[j] ->GetType();
            sv4guiContour* contour;
            
            if(type=="Circle")
            {
                contour=new sv4guiContourCircle();
            }
            else if(type=="Ellipse")
            {
                contour=new sv4guiContourEllipse();
            }
            else if(type=="Polygon")
            {
                contour=new sv4guiContourPolygon();
            }
            else if(type=="SplinePolygon")
            {
                contour=new sv4guiContourSplinePolygon();
            }
            else if(type=="TensionPolygon")
            {
                contour=new sv4guiContourTensionPolygon();
            }
            else
            {
                contour=new sv4guiContour();
            }

//             sv4guiContourEllipse* ce=dynamic_cast<sv4guiContourEllipse*>(contour);
//             if(ce)
//             {
//                 std::string asCircle;
//                 contourElement->QueryStringAttribute("as_circle", &asCircle);
//                 ce->SetAsCircle(asCircle=="true"?true:false);
//             }
// 
//             sv4guiContourTensionPolygon* ct=dynamic_cast<sv4guiContourTensionPolygon*>(contour);
//             if(ct)
//             {
//                 int subdivisionRounds=0;
//                 contourElement->QueryIntAttribute("subdivision_rounds", &subdivisionRounds);
//                 double tensionParam=0.0;
//                 contourElement->QueryDoubleAttribute("tension_param", &tensionParam);
//                 ct->SetSubdivisionRounds(subdivisionRounds);
//                 ct->SetTensionParameter(tensionParam);
//             }
            
            contour->SetMethod(ctSet[j]->GetMethod());
            contour->SetClosed(ctSet[j]->IsClosed());
            contour->SetMinControlPointNumber(ctSet[j]->GetMinControlPointNumber());
            contour->SetMaxControlPointNumber(ctSet[j]->GetMaxControlPointNumber());
            contour->SetSubdivisionType(ctSet[j]->GetSubdivisionType());
            contour->SetSubdivisionNumber(ctSet[j]->GetSubdivisionNumber());
            contour->SetSubdivisionSpacing(ctSet[j]->GetSubdivisionSpacing());
            
            contour->SetPlaced();
            
            contour->SetPathPoint(ctSet[j]->GetPathPoint());
            contour->SetControlPoints(ctSet[j]->GetControlPoints());
            contour->SetContourPoints(ctSet[j]->GetContourPoints());
            
            delete ctSet[j];

            group->InsertContour(-1,contour,i);
        } //contour

    }//timestep

    std::vector<mitk::BaseData::Pointer> result;
    result.push_back(group.GetPointer());
    return result;
}

mitk::IFileIO::ConfidenceLevel sv4guiContourGroupIO::GetReaderConfidenceLevel() const
{
    if (mitk::AbstractFileIO::GetReaderConfidenceLevel() == mitk::IFileIO::Unsupported)
    {
        return mitk::IFileIO::Unsupported;
    }
    return Supported;
}

void sv4guiContourGroupIO::Write()
{
    ValidateOutputLocation();

    const sv4guiContourGroup* group = dynamic_cast<const sv4guiContourGroup*>(this->GetInput());
    if(!group) return;

    TiXmlDocument document;
    auto  decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
    document.LinkEndChild( decl );

    auto  version = new TiXmlElement("format");
    version->SetAttribute("version",  "1.0" );
    document.LinkEndChild(version);

    auto  groupElement = new TiXmlElement("contourgroup");
    groupElement->SetAttribute("path_name", group->GetPathName());
    groupElement->SetAttribute("path_id", group->GetPathID());
    groupElement->SetDoubleAttribute("reslice_size", group->GetResliceSize());
    groupElement->SetAttribute("point_2D_display_size",group->GetProp("point 2D display size"));
    groupElement->SetAttribute("point_size",group->GetProp("point size"));
    document.LinkEndChild(groupElement);

    for(int t=0;t<group->GetTimeSize();t++)
    {
        auto  timestepElement = new TiXmlElement("timestep");
        timestepElement->SetAttribute("id",t);
        groupElement->LinkEndChild(timestepElement);
        
        const sv3::ContourGroup* svCtGrp = dynamic_cast<const sv3::ContourGroup*> (group);
        
        //need ellipse here still
        this->sv3::ContourGroupIO::WriteContourGroup(svCtGrp, timestepElement, t);
    }

    std::string fileName=GetOutputLocation();
    if (document.SaveFile(fileName) == false)
    {
        mitkThrow() << "Could not write contourgroup to " << fileName;

    }
}

mitk::IFileIO::ConfidenceLevel sv4guiContourGroupIO::GetWriterConfidenceLevel() const
{
    if (mitk::AbstractFileIO::GetWriterConfidenceLevel() == mitk::IFileIO::Unsupported) return mitk::IFileIO::Unsupported;
    const sv4guiContourGroup* input = dynamic_cast<const sv4guiContourGroup*>(this->GetInput());
    if (input)
    {
        return Supported;
    }else{
        return Unsupported;
    }
}

sv4guiContourGroupIO* sv4guiContourGroupIO::IOClone() const
{
    return new sv4guiContourGroupIO(*this);
}

