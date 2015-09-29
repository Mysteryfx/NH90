#ifndef inter_edterrain_IRenderPass_h__
#define inter_edterrain_IRenderPass_h__


namespace render 
{
class IRenderContext;
class IParseContext;

class IRenderPass
{
public:
	// Перед выборкой объектов для рендера
	virtual void preDraw(IParseContext* ctx)=0;
	// Перед началом рендера
	virtual void startFrame(IRenderContext* ctx)=0;
	// Для окончания рендера
	virtual void endFrame(IRenderContext* ctx)=0;

	virtual ~IRenderPass() {};
};


} // namespace render


#endif // inter_edterrain_IRenderPass_h__
