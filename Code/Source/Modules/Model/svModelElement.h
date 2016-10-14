#ifndef SVMODELELEMENT_H
#define SVMODELELEMENT_H

#include <svModelExports.h>

#include <mitkDataNode.h>

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

class SVMODEL_EXPORT svModelElement
{

public:

    struct svFace
    {
        int id;
        std::string name;
        std::string type;//wall, cap, or empty if unknown

        vtkSmartPointer<vtkPolyData> vpd;

        bool selected;

        bool visible;
        float opacity;
        float color[3];

        svFace()
            : id(0)
            , name("")
            , type("")
            , vpd(NULL)
            , selected(false)
            , visible(true)
            , opacity(1.0f)
        {
            color[0]=1.0f;
            color[1]=1.0f;
            color[2]=1.0f;
        }

    };

    struct svBlendParamRadius
    {
        int faceID1;
        int faceID2;
        double radius;

        svBlendParamRadius()
            : faceID1(0)
            , faceID2(0)
            , radius(0.0)
        {
        }

        svBlendParamRadius(int id1, int id2, double r)
            : faceID1(id1)
            , faceID2(id2)
            , radius(r)
        {
        }

        svBlendParamRadius(const svBlendParamRadius &other)
            : faceID1(other.faceID1)
            , faceID2(other.faceID2)
            , radius(other.radius)
        {
        }
    };

    svModelElement();

    svModelElement(const svModelElement &other);

    virtual ~svModelElement();

    virtual svModelElement* Clone();

    std::string GetType() const;

    void SetType(std::string type);

    std::vector<std::string> GetSegNames() const;

    void SetSegNames(std::vector<std::string> segNames);

    bool HasSeg(std::string segName);

    std::vector<svFace*> GetFaces() const;

    void SetFaces(std::vector<svFace*> faces);

    svFace* GetFace(int id) const;

    svFace* GetFace(std::string name) const;

    int GetFaceIndex(int id) const;

    int GetFaceID(std::string name) const;

    std::string GetFaceName(int id) const;

    void SetFaceName(std::string name, int id); //Todo: make virtual

    vtkSmartPointer<vtkPolyData> GetWholeVtkPolyData() const;

    void SetWholeVtkPolyData(vtkSmartPointer<vtkPolyData> wvpd);

    virtual vtkSmartPointer<vtkPolyData> CreateFaceVtkPolyData(int id) {}

//    int GetSelectedFaceIndex();

    void SetSelectedFaceIndex(int idx);

    void ClearFaceSelection();

    void SetSelectedFace(int id);
    void SetSelectedFace(std::string name);
    bool IsFaceSelected(std::string name);
    bool IsFaceSelected(int id);

    std::vector<svBlendParamRadius*> GetBlendRadii();

    void SetBlendRadii(std::vector<svBlendParamRadius*> blendRadii);

//    double GetBlendRadius(int faceID1, int faceID2);

    svBlendParamRadius* GetBlendParamRadius(int faceID1, int faceID2);

    void AddBlendRadii(std::vector<svBlendParamRadius*> moreBlendRadii);

    void CalculateBoundingBox(double *bounds);

  protected:

    std::string m_Type;

    std::vector<std::string> m_SegNames;

    std::vector<svFace*> m_Faces;

    vtkSmartPointer<vtkPolyData> m_WholeVtkPolyData;

    std::vector<svBlendParamRadius*> m_BlendRadii;

  };


#endif // SVMODELELEMENT_H
