#ifndef CustomMaterialImpl_H
#define CustomMaterialImpl_H
#include "ICustomMaterial.h"
#include "DXRendererAPI.h"
#include "rwrappers/RenderUserAPI.h"
#include "renderer/RendererAPI.h"
#include "vector.h"
#include "Common.h"

namespace Graphics
{

	// ��������� ��� ����������, ������� ����� ���������� �����������
	class DXRENDERER_API CustomMaterial_Impl: public CustomMaterial
	{
	public:
		CustomMaterial_Impl();

		// ��������� capabilities
		virtual MaterialCaps GetMaterialCaps();

		// ������� ������������� ���������
		virtual void SetAmbientColor(const Vector3& ambientColor);
		virtual void SetDiffuseColor(const Vector3& diffuseColor);
		virtual void SetSpecularColor(const Vector3& specularColor);
		virtual void SetRadianceColor(const Vector3& radianceColor);
		virtual void SetOpacity(float opacity);
		virtual void SetGlossiness(float glossiness);

		// ������� ������� ���������	
		virtual const render::Texture	GetRenderTexture(int index);
		virtual void					SetRenderTexture(int index, const char *name);
		virtual	void					SetRenderTexture(int index, render::Texture& t);
		virtual unsigned				getTexturesCount();


		// ������� �������������, �������� ��� ������ ���������
		virtual void SetTwoSided(bool twoSided);
		virtual void SetLighted(bool lighted);
		virtual void SetWireframe(bool wire);

		// ��������� �������������
		virtual const Vector3& GetAmbientColor();
		virtual const Vector3& GetDiffuseColor();
		virtual const Vector3& GetSpecularColor();
		virtual const Vector3& GetRadianceColor();
		virtual float GetOpacity();
		virtual float GetGlossiness();	


		// ������� �������������, �������� ��� ������ ���������
		virtual bool GetTwoSided();
		virtual bool GetLighted();
		virtual bool GetWireframe();

		// ������������
		virtual void serialize(Serializer& serializer);

		// ��������� ���� ���������� �� ������� ���������
		virtual void GetDataFrom(CustomMaterial *mat);

	protected:
		Vector3 c_ambient, c_diffuse, c_specular, c_radiance;
		float opacity, gloss;
		bool two_sided, lighted, wire;

		ed::vector<render::Texture> _maps;
	};

}

#endif // CustomMaterial_H