#ifndef _OFFSHORE_HISTORY_H__
#define _OFFSHORE_HISTORY_H__

#include "../offshore_config.h"

#include "offshore/scene.h"


namespace offshore
{
	struct History
	{
		// history
		struct IHistoryPage
		{
			virtual ~IHistoryPage(){};
			virtual void redo(Scene& scene, ed::list<int>& updatednodes){};
		};
		struct HistoryTweak : public IHistoryPage
		{
		protected:
			ed::list<int> splinesinds;
			ed::list< Spline > splines;

			ed::list<int> meshesinds;
			ed::list< Mesh > meshes;

			ed::list<int> transformnodeinds;
			ed::list< osg::Matrixd > transformvalues;
		public:
			OFFSHORE_API void redo(Scene& scene, ed::list<int>& updatednodes);
		public:
			OFFSHORE_API bool empty();
			OFFSHORE_API void add(Scene& scene, int nodeindex);
//			OFFSHORE_API void add(int nodeindex, const ed::vector<osg::Vec3d>& verts);
			OFFSHORE_API void add(int nodeindex, const osg::Matrixd& verts);
		};
		struct HistoryNodeOp : public IHistoryPage
		{
			ed::list<int> nodeinds;
			ed::list<offshore::TreeNode> values;

			OFFSHORE_API void redo(Scene& scene, ed::list<int>& updatednodes);
		public:
			OFFSHORE_API void add(int index, const offshore::TreeNode& node);
			OFFSHORE_API bool empty();
		};
	protected:
		ed::list<IHistoryPage* > history;
		ed::list<IHistoryPage* >::iterator currenthistorypage;

		void eraseHistory( ed::list<IHistoryPage* >::iterator from, ed::list<IHistoryPage* >::iterator to);

	public:
		OFFSHORE_API History();
		OFFSHORE_API void clear();
		OFFSHORE_API void addHistoryPage(IHistoryPage* page);
		OFFSHORE_API void undo(Scene& scene, ed::list<int>& updatednodes);
		OFFSHORE_API void redo(Scene& scene, ed::list<int>& updatednodes);
	};

}

#endif
