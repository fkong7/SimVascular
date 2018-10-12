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

#ifndef SV3_CONTOURGROUP_H
#define SV3_CONTOURGROUP_H

#include "SimVascular.h"

#include <sv3SegmentationExports.h>

#include "sv3_Contour.h"

#include <map>
#include <sstream>
#include <iostream>
#include <string>

namespace sv3{
struct SV_EXPORT_SEGMENTATION svLoftingParam
{
    std::string method;

    //Spline Lofting
    int numOutPtsInSegs;//sampleDefault
    //std::vector<int> overrides;
    int samplePerSegment;
    int useLinearSampleAlongLength;
    int linearMuliplier;
    int useFFT;
    int numModes;

    int addCaps;
    //int noInterOut;
    int vecFlag;

    //int numSegs=0;
    int numOutPtsAlongLength;//=samplePerSegment*numSegs
    int numPtsInLinearSampleAlongLength;//=linearMuliplier*numOutPtsAlongLength
    int splineType;

    int numSuperPts;//the number of points of the contour with the maximum point number

    double bias;
    double tension;
    double continuity;

    //Nurbs Lofting
    int uDegree;
    int vDegree;
    std::string uKnotSpanType;
    std::string vKnotSpanType;
    std::string uParametricSpanType;
    std::string vParametricSpanType;

    svLoftingParam()
        : method("nurbs")
        , numOutPtsInSegs(60)
        , samplePerSegment(12)
        , useLinearSampleAlongLength(1)
        , linearMuliplier(10)
        , useFFT(0)
        , numModes(20)
        , addCaps(0)
        //, noInterOut(1)
        , vecFlag(0)
        , numOutPtsAlongLength(0)
        , numPtsInLinearSampleAlongLength(0)
        , splineType(0)
        , numSuperPts(0)
        , bias(0)
        , tension(0)
        , continuity(0)
        , uDegree(2)
        , vDegree(2)
        , uKnotSpanType("derivative")
        , vKnotSpanType("average")
        , uParametricSpanType("centripetal")
        , vParametricSpanType("chord")

    {}

    svLoftingParam(const svLoftingParam &other)
        : method(other.method)
        , numOutPtsInSegs(other.numOutPtsInSegs)
        , samplePerSegment(other.samplePerSegment)
        , useLinearSampleAlongLength(other.useLinearSampleAlongLength)
        , linearMuliplier(other.linearMuliplier)
        , useFFT(other.useFFT)
        , numModes(other.numModes)
        , addCaps(other.addCaps)
        //, noInterOut(other.noInterOut)
        , vecFlag(other.vecFlag)
        , numOutPtsAlongLength(other.numOutPtsAlongLength)
        , numPtsInLinearSampleAlongLength(other.numPtsInLinearSampleAlongLength)
        , splineType(other.splineType)
        , numSuperPts(other.numSuperPts)
        , bias(other.bias)
        , tension(other.tension)
        , continuity(other.continuity)
        , uDegree(other.uDegree)
        , vDegree(other.vDegree)
        , uKnotSpanType(other.uKnotSpanType)
        , vKnotSpanType(other.vKnotSpanType)
        , uParametricSpanType(other.uParametricSpanType)
        , vParametricSpanType(other.vParametricSpanType)
    {}


};

class SV_EXPORT_SEGMENTATION ContourGroup : public cvRepositoryData
{
public:

    ContourGroup();
    ContourGroup(const ContourGroup &other);

    void Expand( unsigned int timeSteps );

    unsigned int GetTimeSize() const;

    bool IsEmptyTimeStep(unsigned int t) const;

    int GetSize( unsigned int t = 0 ) const;

    Contour* GetContour(int contourIndex, unsigned int t = 0) const;

    void InsertControlPoint(int contourIndex, int index, std::array<double,3> point, unsigned int t = 0 );

    void RemoveControlPoint(int contourIndex, int index, unsigned int t = 0);

    void SetControlPoint(int contourIndex, int index, std::array<double,3> point, unsigned int t = 0);

    void SetControlPointSelectedIndex(int contourIndex, int index, unsigned int t = 0);

    void DeselectControlPoint(unsigned int t = 0);

    int GetControlPointSelectedIndex(int contourIndex, unsigned int t = 0);

    void InsertContour(int contourIndex, Contour* contour, unsigned int t = 0);

    void RemoveContour(int contourIndex, unsigned int t = 0);

    void RemoveInvalidContours(unsigned int t = 0);

    void SetContour(int contourIndex, Contour* contour, unsigned int t = 0);

    //bool IsContourSelected(int contourIndex, unsigned int t = 0);

    //void SetContourSelected(int contourIndex, bool selected = true, unsigned int t = 0 );

    //int GetSelectedContourIndex(unsigned int t = 0);

    //void DeselectContours(unsigned int t = 0);

    void ContourControlPointsChanged(unsigned t = 0);

    void ContoursChanged(unsigned t = 0);

    void CalculateBoundingBox(double *bounds,unsigned int t = 0 );

    //Contour* GetUnplacedContour(unsigned int t = 0);

    //Contour* GetContourOnPlane(const vtkPlane *planeGeometry, double precisionFactor=0.1, unsigned int t = 0);

    //int GetUnplacedContourIndex(unsigned int t = 0);

    //int SearchContourByPlane(const vtkPlane *planeGeometry, double precisionFactor=0.1, unsigned int t = 0);

    int GetInsertingContourIndexByPathPosPoint(std::array<double,3> posPoint, unsigned int t = 0);

    std::vector<PathElement::PathPoint>  GetContourPathPoints(unsigned int t = 0);

    std::vector<std::array<double,3> > GetContourPathPosPoints(unsigned int t = 0);

    int GetContourIndexByPathPosPoint(std::array<double,3> posPoint, unsigned int t = 0);

    int GetInsertingContourIndexByTagIndex(int tagIndex, unsigned int t = 0);

    std::string GetPathName() const;
    void SetPathName(std::string name);

    int GetPathID() const;
    void SetPathID(int id);

    std::vector<PathElement::PathPoint>  GetPathPoints(unsigned int t =0);

    std::vector<std::array<double,3> > GetPathPosPoints(unsigned int t =0);

    //int GetCurrentIndexOn2DView();

    //void SetCurrentIndexOn2DView(int index);

    std::vector<Contour*> GetContourSet(unsigned int t = 0);

    std::vector<Contour*> GetValidContourSet(unsigned int t = 0);//contour points > 1

    svLoftingParam* GetLoftingParam() const {return m_LoftingParam;}

    bool IsDataModified();
    void SetDataModified(bool modified = true);

    //double GetResliceSize() const;

    //void SetResliceSize(double size);

    void SetProp(const std::string& key, std::string value);
    std::string GetProp(const std::string& key) const;
    std::map<std::string,std::string> GetProps();

  protected:

    virtual ~ContourGroup();

    virtual void ClearData();

    virtual void InitializeEmpty();

    std::vector< std::vector<Contour*> > m_ContourSets;

    bool m_CalculateBoundingBox;

    int m_GroupID;

//    std::string m_ContourGroupName;

    int m_PathID;

    std::string m_PathName;

    //int m_CurrentIndexOn2DView;

    //lofting parameters as public
    svLoftingParam *m_LoftingParam;

    bool m_DataModified;

    //double m_ResliceSize;

    std::map<std::string,std::string> m_Props;
  };

}
#endif // SV3_CONTOURGROUP_H
